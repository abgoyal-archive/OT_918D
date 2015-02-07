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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

//For mt6573_evb
#include <mach/mt6573_pll.h>
#include "cust_matv.h"
#include "cust_matv_comm.h"


int cust_matv_power_on(void)
{  
	MATV_LOGE("[MATV] mt5193 cust_matv_power_on Start\n");
    //set GPIO94 for power
    mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN,GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN, GPIO_DIR_OUT);
    mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN,GPIO_PULL_DISABLE);
    mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN, GPIO_OUT_ONE);
    
    
   

    if(TRUE != hwPowerOn(MT65XX_POWER_LDO_VCAMA,VOL_2800,"MT5192"))
    {
        MATV_LOGE("[MATV] Fail to enable analog gain VCAMA\n");
        return -EIO;
    }  
    if(TRUE != hwPowerOn(MT65XX_POWER_LDO_VCAMA2,VOL_2800,"MT5192"))
    {
        MATV_LOGE("[MATV] Fail to enable analog gain VCAMA2\n");
        return -EIO;
    } 
    if(TRUE != hwPowerOn(MT65XX_POWER_LDO_VCAMD,VOL_1500,"MT5192"))
    {
        MATV_LOGE("[MATV] Fail to enable analog gain VCAMD\n");
        return -EIO;
    }

#ifdef CAMERA_IO_DRV_1800	
    if(TRUE != hwPowerOn(MT65XX_POWER_LDO_VCAMD2,VOL_1800,"MT5192"))
    {
        MATV_LOGE("[MATV] Fail to enable analog gain VCAMD2\n");
        return -EIO;
    }
#else
    if(TRUE != hwPowerOn(MT65XX_POWER_LDO_VCAMD2,VOL_2800,"MT5192"))
    {
        MATV_LOGE("[MATV] Fail to enable analog gain VCAMD2\n");
        return -EIO;
    }
#endif

    //disable main sensor
    mt_set_gpio_mode(GPIO_CAMERA_CMRST_PIN,GPIO_MODE_00);
    mt_set_gpio_mode(GPIO_CAMERA_CMPDN_PIN,GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_CAMERA_CMRST_PIN,GPIO_DIR_OUT);
    mt_set_gpio_dir(GPIO_CAMERA_CMPDN_PIN,GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CAMERA_CMRST_PIN,GPIO_OUT_ZERO);
    mt_set_gpio_out(GPIO_CAMERA_CMPDN_PIN,GPIO_OUT_ONE);
    //disable sub sensor        
    mt_set_gpio_mode(GPIO_CAMERA_CMPDN1_PIN, GPIO_MODE_00);
    mt_set_gpio_mode(GPIO_CAMERA_CMRST1_PIN ,GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_CAMERA_CMPDN1_PIN, GPIO_DIR_OUT);
    mt_set_gpio_dir(GPIO_CAMERA_CMRST1_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CAMERA_CMPDN1_PIN, GPIO_OUT_ONE);
    mt_set_gpio_out(GPIO_CAMERA_CMRST1_PIN, GPIO_OUT_ZERO); 

    
    return 0;
}


int cust_matv_power_off(void)
{  	
	MATV_LOGE("[MATV] mt5193 cust_matv_power_off Start\n");
    if(TRUE != hwPowerDown(MT65XX_POWER_LDO_VCAMA,"MT5192"))
    {
        MATV_LOGE("[MATV] Fail to disable analog gain VCAMA\n");
        return -EIO;
    }  
    if(TRUE != hwPowerDown(MT65XX_POWER_LDO_VCAMA2,"MT5192"))
    {
        MATV_LOGE("[MATV] Fail to disable analog gain VCAMA2\n");
        return -EIO;
    } 
    if(TRUE != hwPowerDown(MT65XX_POWER_LDO_VCAMD,"MT5192"))
    {
        MATV_LOGE("[MATV] Fail to disable analog gain VCAMD\n");
        return -EIO;
    } 
    if(TRUE != hwPowerDown(MT65XX_POWER_LDO_VCAMD2,"MT5192"))
    {
        MATV_LOGE("[MATV] Fail to disable analog gain VCAMD2\n");
        return -EIO;
    }    
    return 0;
}


int cust_matv_gpio_on(void)
{
	MATV_LOGE("[MATV] mt5193 cust_matv_gpio_on Start\n");
#if 0
	mt_set_gpio_mode(GPIO_I2S1_CK_PIN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_I2S1_CK_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_I2S1_CK_PIN, GPIO_OUT_ZERO);

    mt_set_gpio_mode(GPIO_I2S1_WS_PIN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_I2S1_WS_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_I2S1_WS_PIN, GPIO_OUT_ZERO);

    mt_set_gpio_mode(GPIO_I2S1_DAT_PIN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_I2S1_DAT_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_I2S1_DAT_PIN, GPIO_OUT_ZERO);

	mt_set_gpio_mode(GPIO_I2S0_CK_PIN, GPIO_I2S0_CK_PIN_M_I2S0_CK);
    mt_set_gpio_mode(GPIO_I2S0_DAT_PIN, GPIO_I2S0_DAT_PIN_M_I2S0_DAT);
	mt_set_gpio_mode(GPIO_I2S0_WS_PIN, GPIO_I2S0_WS_PIN_M_I2S0_WS);
#endif
}

int cust_matv_gpio_off(void)
{
	MATV_LOGE("[MATV] mt5193 cust_matv_gpio_off Start\n");
#if 0
	mt_set_gpio_mode(GPIO_I2S0_CK_PIN, GPIO_MODE_00);
    mt_set_gpio_mode(GPIO_I2S0_WS_PIN, GPIO_MODE_00);
    mt_set_gpio_mode(GPIO_I2S0_DAT_PIN, GPIO_MODE_00);
	
    mt_set_gpio_dir(GPIO_I2S0_CK_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_I2S0_CK_PIN, GPIO_OUT_ZERO);
	mt_set_gpio_dir(GPIO_I2S0_WS_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_I2S0_WS_PIN, GPIO_OUT_ZERO);
	mt_set_gpio_dir(GPIO_I2S0_DAT_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_I2S0_DAT_PIN, GPIO_OUT_ZERO);
#endif
}


