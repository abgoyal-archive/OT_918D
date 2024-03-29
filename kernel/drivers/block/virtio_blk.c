/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

//#define DEBUG
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>
#include <linux/virtio.h>
#include <linux/virtio_blk.h>
#include <linux/scatterlist.h>

#define PART_BITS 4

static int major, index;

struct virtio_blk
{
	spinlock_t lock;

	struct virtio_device *vdev;
	struct virtqueue *vq;

	/* The disk structure for the kernel. */
	struct gendisk *disk;

	/* Request tracking. */
	struct list_head reqs;

	mempool_t *pool;

	/* What host tells us, plus 2 for header & tailer. */
	unsigned int sg_elems;

	/* Scatterlist: can be too big for stack. */
	struct scatterlist sg[/*sg_elems*/];
};

struct virtblk_req
{
	struct list_head list;
	struct request *req;
	struct virtio_blk_outhdr out_hdr;
	struct virtio_scsi_inhdr in_hdr;
	u8 status;
};

static void blk_done(struct virtqueue *vq)
{
	struct virtio_blk *vblk = vq->vdev->priv;
	struct virtblk_req *vbr;
	unsigned int len;
	unsigned long flags;

	spin_lock_irqsave(&vblk->lock, flags);
	while ((vbr = virtqueue_get_buf(vblk->vq, &len)) != NULL) {
		int error;

		switch (vbr->status) {
		case VIRTIO_BLK_S_OK:
			error = 0;
			break;
		case VIRTIO_BLK_S_UNSUPP:
			error = -ENOTTY;
			break;
		default:
			error = -EIO;
			break;
		}

		if (blk_pc_request(vbr->req)) {
			vbr->req->resid_len = vbr->in_hdr.residual;
			vbr->req->sense_len = vbr->in_hdr.sense_len;
			vbr->req->errors = vbr->in_hdr.errors;
		}
		if (blk_special_request(vbr->req))
			vbr->req->errors = (error != 0);

		__blk_end_request_all(vbr->req, error);
		list_del(&vbr->list);
		mempool_free(vbr, vblk->pool);
	}
	/* In case queue is stopped waiting for more buffers. */
	blk_start_queue(vblk->disk->queue);
	spin_unlock_irqrestore(&vblk->lock, flags);
}

static bool do_req(struct request_queue *q, struct virtio_blk *vblk,
		   struct request *req)
{
	unsigned long num, out = 0, in = 0;
	struct virtblk_req *vbr;

	vbr = mempool_alloc(vblk->pool, GFP_ATOMIC);
	if (!vbr)
		/* When another request finishes we'll try again. */
		return false;

	vbr->req = req;
	switch (req->cmd_type) {
	case REQ_TYPE_FS:
		vbr->out_hdr.type = 0;
		vbr->out_hdr.sector = blk_rq_pos(vbr->req);
		vbr->out_hdr.ioprio = req_get_ioprio(vbr->req);
		break;
	case REQ_TYPE_BLOCK_PC:
		vbr->out_hdr.type = VIRTIO_BLK_T_SCSI_CMD;
		vbr->out_hdr.sector = 0;
		vbr->out_hdr.ioprio = req_get_ioprio(vbr->req);
		break;
	case REQ_TYPE_SPECIAL:
		vbr->out_hdr.type = VIRTIO_BLK_T_GET_ID;
		vbr->out_hdr.sector = 0;
		vbr->out_hdr.ioprio = req_get_ioprio(vbr->req);
		break;
	case REQ_TYPE_LINUX_BLOCK:
		if (req->cmd[0] == REQ_LB_OP_FLUSH) {
			vbr->out_hdr.type = VIRTIO_BLK_T_FLUSH;
			vbr->out_hdr.sector = 0;
			vbr->out_hdr.ioprio = req_get_ioprio(vbr->req);
			break;
		}
		/*FALLTHRU*/
	default:
		/* We don't put anything else in the queue. */
		BUG();
	}

	if (blk_barrier_rq(vbr->req))
		vbr->out_hdr.type |= VIRTIO_BLK_T_BARRIER;

	sg_set_buf(&vblk->sg[out++], &vbr->out_hdr, sizeof(vbr->out_hdr));

	/*
	 * If this is a packet command we need a couple of additional headers.
	 * Behind the normal outhdr we put a segment with the scsi command
	 * block, and before the normal inhdr we put the sense data and the
	 * inhdr with additional status information before the normal inhdr.
	 */
	if (blk_pc_request(vbr->req))
		sg_set_buf(&vblk->sg[out++], vbr->req->cmd, vbr->req->cmd_len);

	num = blk_rq_map_sg(q, vbr->req, vblk->sg + out);

	if (blk_pc_request(vbr->req)) {
		sg_set_buf(&vblk->sg[num + out + in++], vbr->req->sense, 96);
		sg_set_buf(&vblk->sg[num + out + in++], &vbr->in_hdr,
			   sizeof(vbr->in_hdr));
	}

	sg_set_buf(&vblk->sg[num + out + in++], &vbr->status,
		   sizeof(vbr->status));

	if (num) {
		if (rq_data_dir(vbr->req) == WRITE) {
			vbr->out_hdr.type |= VIRTIO_BLK_T_OUT;
			out += num;
		} else {
			vbr->out_hdr.type |= VIRTIO_BLK_T_IN;
			in += num;
		}
	}

	if (virtqueue_add_buf(vblk->vq, vblk->sg, out, in, vbr) < 0) {
		mempool_free(vbr, vblk->pool);
		return false;
	}

	list_add_tail(&vbr->list, &vblk->reqs);
	return true;
}

static void do_virtblk_request(struct request_queue *q)
{
	struct virtio_blk *vblk = q->queuedata;
	struct request *req;
	unsigned int issued = 0;

	while ((req = blk_peek_request(q)) != NULL) {
		BUG_ON(req->nr_phys_segments + 2 > vblk->sg_elems);

		/* If this request fails, stop queue and wait for something to
		   finish to restart it. */
		if (!do_req(q, vblk, req)) {
			blk_stop_queue(q);
			break;
		}
		blk_start_request(req);
		issued++;
	}

	if (issued)
		virtqueue_kick(vblk->vq);
}

static void virtblk_prepare_flush(struct request_queue *q, struct request *req)
{
	req->cmd_type = REQ_TYPE_LINUX_BLOCK;
	req->cmd[0] = REQ_LB_OP_FLUSH;
}

/* return id (s/n) string for *disk to *id_str
 */
static int virtblk_get_id(struct gendisk *disk, char *id_str)
{
	struct virtio_blk *vblk = disk->private_data;
	struct request *req;
	struct bio *bio;

	bio = bio_map_kern(vblk->disk->queue, id_str, VIRTIO_BLK_ID_BYTES,
			   GFP_KERNEL);
	if (IS_ERR(bio))
		return PTR_ERR(bio);

	req = blk_make_request(vblk->disk->queue, bio, GFP_KERNEL);
	if (IS_ERR(req)) {
		bio_put(bio);
		return PTR_ERR(req);
	}

	req->cmd_type = REQ_TYPE_SPECIAL;
	return blk_execute_rq(vblk->disk->queue, vblk->disk, req, false);
}

static int virtblk_ioctl(struct block_device *bdev, fmode_t mode,
			 unsigned cmd, unsigned long data)
{
	struct gendisk *disk = bdev->bd_disk;
	struct virtio_blk *vblk = disk->private_data;

	if (cmd == 0x56424944) { /* 'VBID' */
		void __user *usr_data = (void __user *)data;
		char id_str[VIRTIO_BLK_ID_BYTES];
		int err;

		err = virtblk_get_id(disk, id_str);
		if (!err && copy_to_user(usr_data, id_str, VIRTIO_BLK_ID_BYTES))
			err = -EFAULT;
		return err;
	}
	/*
	 * Only allow the generic SCSI ioctls if the host can support it.
	 */
	if (!virtio_has_feature(vblk->vdev, VIRTIO_BLK_F_SCSI))
		return -ENOTTY;

	return scsi_cmd_ioctl(disk->queue, disk, mode, cmd,
			      (void __user *)data);
}

/* We provide getgeo only to please some old bootloader/partitioning tools */
static int virtblk_getgeo(struct block_device *bd, struct hd_geometry *geo)
{
	struct virtio_blk *vblk = bd->bd_disk->private_data;
	struct virtio_blk_geometry vgeo;
	int err;

	/* see if the host passed in geometry config */
	err = virtio_config_val(vblk->vdev, VIRTIO_BLK_F_GEOMETRY,
				offsetof(struct virtio_blk_config, geometry),
				&vgeo);

	if (!err) {
		geo->heads = vgeo.heads;
		geo->sectors = vgeo.sectors;
		geo->cylinders = vgeo.cylinders;
	} else {
		/* some standard values, similar to sd */
		geo->heads = 1 << 6;
		geo->sectors = 1 << 5;
		geo->cylinders = get_capacity(bd->bd_disk) >> 11;
	}
	return 0;
}

static const struct block_device_operations virtblk_fops = {
	.locked_ioctl = virtblk_ioctl,
	.owner  = THIS_MODULE,
	.getgeo = virtblk_getgeo,
};

static int index_to_minor(int index)
{
	return index << PART_BITS;
}

static int __devinit virtblk_probe(struct virtio_device *vdev)
{
	struct virtio_blk *vblk;
	struct request_queue *q;
	int err;
	u64 cap;
	u32 v, blk_size, sg_elems, opt_io_size;
	u16 min_io_size;
	u8 physical_block_exp, alignment_offset;

	if (index_to_minor(index) >= 1 << MINORBITS)
		return -ENOSPC;

	/* We need to know how many segments before we allocate. */
	err = virtio_config_val(vdev, VIRTIO_BLK_F_SEG_MAX,
				offsetof(struct virtio_blk_config, seg_max),
				&sg_elems);

	/* We need at least one SG element, whatever they say. */
	if (err || !sg_elems)
		sg_elems = 1;

	/* We need an extra sg elements at head and tail. */
	sg_elems += 2;
	vdev->priv = vblk = kmalloc(sizeof(*vblk) +
				    sizeof(vblk->sg[0]) * sg_elems, GFP_KERNEL);
	if (!vblk) {
		err = -ENOMEM;
		goto out;
	}

	INIT_LIST_HEAD(&vblk->reqs);
	spin_lock_init(&vblk->lock);
	vblk->vdev = vdev;
	vblk->sg_elems = sg_elems;
	sg_init_table(vblk->sg, vblk->sg_elems);

	/* We expect one virtqueue, for output. */
	vblk->vq = virtio_find_single_vq(vdev, blk_done, "requests");
	if (IS_ERR(vblk->vq)) {
		err = PTR_ERR(vblk->vq);
		goto out_free_vblk;
	}

	vblk->pool = mempool_create_kmalloc_pool(1,sizeof(struct virtblk_req));
	if (!vblk->pool) {
		err = -ENOMEM;
		goto out_free_vq;
	}

	/* FIXME: How many partitions?  How long is a piece of string? */
	vblk->disk = alloc_disk(1 << PART_BITS);
	if (!vblk->disk) {
		err = -ENOMEM;
		goto out_mempool;
	}

	q = vblk->disk->queue = blk_init_queue(do_virtblk_request, &vblk->lock);
	if (!q) {
		err = -ENOMEM;
		goto out_put_disk;
	}

	q->queuedata = vblk;

	if (index < 26) {
		sprintf(vblk->disk->disk_name, "vd%c", 'a' + index % 26);
	} else if (index < (26 + 1) * 26) {
		sprintf(vblk->disk->disk_name, "vd%c%c",
			'a' + index / 26 - 1, 'a' + index % 26);
	} else {
		const unsigned int m1 = (index / 26 - 1) / 26 - 1;
		const unsigned int m2 = (index / 26 - 1) % 26;
		const unsigned int m3 =  index % 26;
		sprintf(vblk->disk->disk_name, "vd%c%c%c",
			'a' + m1, 'a' + m2, 'a' + m3);
	}

	vblk->disk->major = major;
	vblk->disk->first_minor = index_to_minor(index);
	vblk->disk->private_data = vblk;
	vblk->disk->fops = &virtblk_fops;
	vblk->disk->driverfs_dev = &vdev->dev;
	index++;

	/* If barriers are supported, tell block layer that queue is ordered */
	if (virtio_has_feature(vdev, VIRTIO_BLK_F_FLUSH))
		blk_queue_ordered(q, QUEUE_ORDERED_DRAIN_FLUSH,
				  virtblk_prepare_flush);
	else if (virtio_has_feature(vdev, VIRTIO_BLK_F_BARRIER))
		blk_queue_ordered(q, QUEUE_ORDERED_TAG, NULL);

	/* If disk is read-only in the host, the guest should obey */
	if (virtio_has_feature(vdev, VIRTIO_BLK_F_RO))
		set_disk_ro(vblk->disk, 1);

	/* Host must always specify the capacity. */
	vdev->config->get(vdev, offsetof(struct virtio_blk_config, capacity),
			  &cap, sizeof(cap));

	/* If capacity is too big, truncate with warning. */
	if ((sector_t)cap != cap) {
		dev_warn(&vdev->dev, "Capacity %llu too large: truncating\n",
			 (unsigned long long)cap);
		cap = (sector_t)-1;
	}
	set_capacity(vblk->disk, cap);

	/* We can handle whatever the host told us to handle. */
	blk_queue_max_segments(q, vblk->sg_elems-2);

	/* No need to bounce any requests */
	blk_queue_bounce_limit(q, BLK_BOUNCE_ANY);

	/* No real sector limit. */
	blk_queue_max_hw_sectors(q, -1U);

	/* Host can optionally specify maximum segment size and number of
	 * segments. */
	err = virtio_config_val(vdev, VIRTIO_BLK_F_SIZE_MAX,
				offsetof(struct virtio_blk_config, size_max),
				&v);
	if (!err)
		blk_queue_max_segment_size(q, v);
	else
		blk_queue_max_segment_size(q, -1U);

	/* Host can optionally specify the block size of the device */
	err = virtio_config_val(vdev, VIRTIO_BLK_F_BLK_SIZE,
				offsetof(struct virtio_blk_config, blk_size),
				&blk_size);
	if (!err)
		blk_queue_logical_block_size(q, blk_size);
	else
		blk_size = queue_logical_block_size(q);

	/* Use topology information if available */
	err = virtio_config_val(vdev, VIRTIO_BLK_F_TOPOLOGY,
			offsetof(struct virtio_blk_config, physical_block_exp),
			&physical_block_exp);
	if (!err && physical_block_exp)
		blk_queue_physical_block_size(q,
				blk_size * (1 << physical_block_exp));

	err = virtio_config_val(vdev, VIRTIO_BLK_F_TOPOLOGY,
			offsetof(struct virtio_blk_config, alignment_offset),
			&alignment_offset);
	if (!err && alignment_offset)
		blk_queue_alignment_offset(q, blk_size * alignment_offset);

	err = virtio_config_val(vdev, VIRTIO_BLK_F_TOPOLOGY,
			offsetof(struct virtio_blk_config, min_io_size),
			&min_io_size);
	if (!err && min_io_size)
		blk_queue_io_min(q, blk_size * min_io_size);

	err = virtio_config_val(vdev, VIRTIO_BLK_F_TOPOLOGY,
			offsetof(struct virtio_blk_config, opt_io_size),
			&opt_io_size);
	if (!err && opt_io_size)
		blk_queue_io_opt(q, blk_size * opt_io_size);


	add_disk(vblk->disk);
	return 0;

out_put_disk:
	put_disk(vblk->disk);
out_mempool:
	mempool_destroy(vblk->pool);
out_free_vq:
	vdev->config->del_vqs(vdev);
out_free_vblk:
	kfree(vblk);
out:
	return err;
}

static void __devexit virtblk_remove(struct virtio_device *vdev)
{
	struct virtio_blk *vblk = vdev->priv;

	/* Nothing should be pending. */
	BUG_ON(!list_empty(&vblk->reqs));

	/* Stop all the virtqueues. */
	vdev->config->reset(vdev);

	del_gendisk(vblk->disk);
	blk_cleanup_queue(vblk->disk->queue);
	put_disk(vblk->disk);
	mempool_destroy(vblk->pool);
	vdev->config->del_vqs(vdev);
	kfree(vblk);
}

static const struct virtio_device_id id_table[] = {
	{ VIRTIO_ID_BLOCK, VIRTIO_DEV_ANY_ID },
	{ 0 },
};

static unsigned int features[] = {
	VIRTIO_BLK_F_BARRIER, VIRTIO_BLK_F_SEG_MAX, VIRTIO_BLK_F_SIZE_MAX,
	VIRTIO_BLK_F_GEOMETRY, VIRTIO_BLK_F_RO, VIRTIO_BLK_F_BLK_SIZE,
	VIRTIO_BLK_F_SCSI, VIRTIO_BLK_F_FLUSH, VIRTIO_BLK_F_TOPOLOGY
};

/*
 * virtio_blk causes spurious section mismatch warning by
 * simultaneously referring to a __devinit and a __devexit function.
 * Use __refdata to avoid this warning.
 */
static struct virtio_driver __refdata virtio_blk = {
	.feature_table = features,
	.feature_table_size = ARRAY_SIZE(features),
	.driver.name =	KBUILD_MODNAME,
	.driver.owner =	THIS_MODULE,
	.id_table =	id_table,
	.probe =	virtblk_probe,
	.remove =	__devexit_p(virtblk_remove),
};

static int __init init(void)
{
	major = register_blkdev(0, "virtblk");
	if (major < 0)
		return major;
	return register_virtio_driver(&virtio_blk);
}

static void __exit fini(void)
{
	unregister_blkdev(major, "virtblk");
	unregister_virtio_driver(&virtio_blk);
}
module_init(init);
module_exit(fini);

MODULE_DEVICE_TABLE(virtio, id_table);
MODULE_DESCRIPTION("Virtio block driver");
MODULE_LICENSE("GPL");
