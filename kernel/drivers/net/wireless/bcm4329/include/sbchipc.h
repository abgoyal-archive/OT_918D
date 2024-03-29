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

/*
 * SiliconBackplane Chipcommon core hardware definitions.
 *
 * The chipcommon core provides chip identification, SB control,
 * jtag, 0/1/2 uarts, clock frequency control, a watchdog interrupt timer,
 * gpio interface, extbus, and support for serial and parallel flashes.
 *
 * $Id: sbchipc.h,v 13.103.2.5.4.5.2.9 2009/07/03 14:23:21 Exp $
 *
 * Copyright (C) 1999-2010, Broadcom Corporation
 * 
 *      Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 * 
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 * 
 *      Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
 */


#ifndef	_SBCHIPC_H
#define	_SBCHIPC_H

#ifndef _LANGUAGE_ASSEMBLY


#ifndef PAD
#define	_PADLINE(line)	pad ## line
#define	_XSTR(line)	_PADLINE(line)
#define	PAD		_XSTR(__LINE__)
#endif	

typedef volatile struct {
	uint32	chipid;			
	uint32	capabilities;
	uint32	corecontrol;		
	uint32	bist;

	
	uint32	otpstatus;		
	uint32	otpcontrol;
	uint32	otpprog;
	uint32	PAD;

	
	uint32	intstatus;		
	uint32	intmask;
	uint32	chipcontrol;		
	uint32	chipstatus;		

	
	uint32	jtagcmd;		
	uint32	jtagir;
	uint32	jtagdr;
	uint32	jtagctrl;

	
	uint32	flashcontrol;		
	uint32	flashaddress;
	uint32	flashdata;
	uint32	PAD[1];

	
	uint32	broadcastaddress;	
	uint32	broadcastdata;

	
	uint32	gpiopullup;		
	uint32	gpiopulldown;		
	uint32	gpioin;			
	uint32	gpioout;
	uint32	gpioouten;
	uint32	gpiocontrol;
	uint32	gpiointpolarity;
	uint32	gpiointmask;

	
	uint32	gpioevent;
	uint32	gpioeventintmask;

	
	uint32	watchdog;		

	
	uint32	gpioeventintpolarity;

	
	uint32  gpiotimerval;		
	uint32  gpiotimeroutmask;

	
	uint32	clockcontrol_n;		
	uint32	clockcontrol_sb;	
	uint32	clockcontrol_pci;	
	uint32	clockcontrol_m2;	
	uint32	clockcontrol_m3;	
	uint32	clkdiv;			
	uint32	PAD[2];

	
	uint32	pll_on_delay;		
	uint32	fref_sel_delay;
	uint32	slow_clk_ctl;		
	uint32	PAD[1];

	
	uint32	system_clk_ctl;		
	uint32	clkstatestretch;
	uint32	PAD[13];

	
	uint32	eromptr;

	
	uint32	pcmcia_config;		
	uint32	pcmcia_memwait;
	uint32	pcmcia_attrwait;
	uint32	pcmcia_iowait;
	uint32	ide_config;
	uint32	ide_memwait;
	uint32	ide_attrwait;
	uint32	ide_iowait;
	uint32	prog_config;
	uint32	prog_waitcount;
	uint32	flash_config;
	uint32	flash_waitcount;
	uint32	PAD[4];
	uint32	PAD[40];


	
	uint32	clk_ctl_st;		
	uint32	hw_war;
	uint32	PAD[70];

	
	uint8	uart0data;		
	uint8	uart0imr;
	uint8	uart0fcr;
	uint8	uart0lcr;
	uint8	uart0mcr;
	uint8	uart0lsr;
	uint8	uart0msr;
	uint8	uart0scratch;
	uint8	PAD[248];		

	uint8	uart1data;		
	uint8	uart1imr;
	uint8	uart1fcr;
	uint8	uart1lcr;
	uint8	uart1mcr;
	uint8	uart1lsr;
	uint8	uart1msr;
	uint8	uart1scratch;
	uint32	PAD[126];

	
	uint32	pmucontrol;		
	uint32	pmucapabilities;
	uint32	pmustatus;
	uint32	res_state;
	uint32	res_pending;
	uint32	pmutimer;
	uint32	min_res_mask;
	uint32	max_res_mask;
	uint32	res_table_sel;
	uint32	res_dep_mask;
	uint32	res_updn_timer;
	uint32	res_timer;
	uint32	clkstretch;
	uint32	pmuwatchdog;
	uint32	gpiosel;		
	uint32	gpioenable;		
	uint32	res_req_timer_sel;
	uint32	res_req_timer;
	uint32	res_req_mask;
	uint32	PAD;
	uint32	chipcontrol_addr;	
	uint32	chipcontrol_data;	
	uint32	regcontrol_addr;
	uint32	regcontrol_data;
	uint32	pllcontrol_addr;
	uint32	pllcontrol_data;
	uint32	PAD[102];
	uint16	otp[768];
} chipcregs_t;

#endif 

#define	CC_CHIPID		0
#define	CC_CAPABILITIES		4
#define CC_OTPST		0x10
#define CC_CHIPST		0x2c
#define	CC_JTAGCMD		0x30
#define	CC_JTAGIR		0x34
#define	CC_JTAGDR		0x38
#define	CC_JTAGCTRL		0x3c
#define	CC_WATCHDOG		0x80
#define	CC_CLKC_N		0x90
#define	CC_CLKC_M0		0x94
#define	CC_CLKC_M1		0x98
#define	CC_CLKC_M2		0x9c
#define	CC_CLKC_M3		0xa0
#define	CC_CLKDIV		0xa4
#define	CC_SYS_CLK_CTL		0xc0
#define	CC_CLK_CTL_ST		SI_CLK_CTL_ST
#define	CC_EROMPTR		0xfc
#define	PMU_CTL			0x600
#define	PMU_CAP			0x604
#define	PMU_ST			0x608
#define PMU_RES_STATE		0x60c
#define PMU_TIMER		0x614
#define	PMU_MIN_RES_MASK	0x618
#define	PMU_MAX_RES_MASK	0x61c
#define PMU_REG_CONTROL_ADDR	0x658
#define PMU_REG_CONTROL_DATA	0x65C
#define PMU_PLL_CONTROL_ADDR 	0x660
#define PMU_PLL_CONTROL_DATA 	0x664
#define	CC_OTP			0x800		


#define	CID_ID_MASK		0x0000ffff	
#define	CID_REV_MASK		0x000f0000	
#define	CID_REV_SHIFT		16		
#define	CID_PKG_MASK		0x00f00000	
#define	CID_PKG_SHIFT		20		
#define	CID_CC_MASK		0x0f000000	
#define CID_CC_SHIFT		24
#define	CID_TYPE_MASK		0xf0000000	
#define CID_TYPE_SHIFT		28


#define	CC_CAP_UARTS_MASK	0x00000003	
#define CC_CAP_MIPSEB		0x00000004	
#define CC_CAP_UCLKSEL		0x00000018	
#define CC_CAP_UINTCLK		0x00000008	
#define CC_CAP_UARTGPIO		0x00000020	
#define CC_CAP_EXTBUS_MASK	0x000000c0	
#define CC_CAP_EXTBUS_NONE	0x00000000	
#define CC_CAP_EXTBUS_FULL	0x00000040	
#define CC_CAP_EXTBUS_PROG	0x00000080	
#define	CC_CAP_FLASH_MASK	0x00000700	
#define	CC_CAP_PLL_MASK		0x00038000	
#define CC_CAP_PWR_CTL		0x00040000	
#define CC_CAP_OTPSIZE		0x00380000	
#define CC_CAP_OTPSIZE_SHIFT	19		
#define CC_CAP_OTPSIZE_BASE	5		
#define CC_CAP_JTAGP		0x00400000	
#define CC_CAP_ROM		0x00800000	
#define CC_CAP_BKPLN64		0x08000000	
#define	CC_CAP_PMU		0x10000000	
#define	CC_CAP_ECI		0x20000000	


#define PLL_NONE		0x00000000
#define PLL_TYPE1		0x00010000	
#define PLL_TYPE2		0x00020000	
#define PLL_TYPE3		0x00030000	
#define PLL_TYPE4		0x00008000	
#define PLL_TYPE5		0x00018000	
#define PLL_TYPE6		0x00028000	
#define PLL_TYPE7		0x00038000	


#define	ILP_CLOCK		32000


#define	ALP_CLOCK		20000000


#define	HT_CLOCK		80000000


#define CC_UARTCLKO		0x00000001	
#define	CC_SE			0x00000002	
#define CC_UARTCLKEN		0x00000008	


#define CHIPCTRL_4321A0_DEFAULT	0x3a4
#define CHIPCTRL_4321A1_DEFAULT	0x0a4
#define CHIPCTRL_4321_PLL_DOWN	0x800000	


#define OTPS_OL_MASK		0x000000ff
#define OTPS_OL_MFG		0x00000001	
#define OTPS_OL_OR1		0x00000002	
#define OTPS_OL_OR2		0x00000004	
#define OTPS_OL_GU		0x00000008	
#define OTPS_GUP_MASK		0x00000f00
#define OTPS_GUP_SHIFT		8
#define OTPS_GUP_HW		0x00000100	
#define OTPS_GUP_SW		0x00000200	
#define OTPS_GUP_CI		0x00000400	
#define OTPS_GUP_FUSE		0x00000800	
#define OTPS_READY		0x00001000
#define OTPS_RV(x)		(1 << (16 + (x)))	
#define OTPS_RV_MASK		0x0fff0000


#define OTPC_PROGSEL		0x00000001
#define OTPC_PCOUNT_MASK	0x0000000e
#define OTPC_PCOUNT_SHIFT	1
#define OTPC_VSEL_MASK		0x000000f0
#define OTPC_VSEL_SHIFT		4
#define OTPC_TMM_MASK		0x00000700
#define OTPC_TMM_SHIFT		8
#define OTPC_ODM		0x00000800
#define OTPC_PROGEN		0x80000000


#define OTPP_COL_MASK		0x000000ff
#define OTPP_COL_SHIFT		0
#define OTPP_ROW_MASK		0x0000ff00
#define OTPP_ROW_SHIFT		8
#define OTPP_OC_MASK		0x0f000000
#define OTPP_OC_SHIFT		24
#define OTPP_READERR		0x10000000
#define OTPP_VALUE_MASK		0x20000000
#define OTPP_VALUE_SHIFT	29
#define OTPP_START_BUSY		0x80000000


#define OTPPOC_READ		0
#define OTPPOC_BIT_PROG		1
#define OTPPOC_VERIFY		3
#define OTPPOC_INIT		4
#define OTPPOC_SET		5
#define OTPPOC_RESET		6
#define OTPPOC_OCST		7
#define OTPPOC_ROW_LOCK		8
#define OTPPOC_PRESCN_TEST	9


#define JCMD_START		0x80000000
#define JCMD_BUSY		0x80000000
#define JCMD_STATE_MASK		0x60000000
#define JCMD_STATE_TLR		0x00000000	
#define JCMD_STATE_PIR		0x20000000	
#define JCMD_STATE_PDR		0x40000000	
#define JCMD_STATE_RTI		0x60000000	
#define JCMD0_ACC_MASK		0x0000f000
#define JCMD0_ACC_IRDR		0x00000000
#define JCMD0_ACC_DR		0x00001000
#define JCMD0_ACC_IR		0x00002000
#define JCMD0_ACC_RESET		0x00003000
#define JCMD0_ACC_IRPDR		0x00004000
#define JCMD0_ACC_PDR		0x00005000
#define JCMD0_IRW_MASK		0x00000f00
#define JCMD_ACC_MASK		0x000f0000	
#define JCMD_ACC_IRDR		0x00000000
#define JCMD_ACC_DR		0x00010000
#define JCMD_ACC_IR		0x00020000
#define JCMD_ACC_RESET		0x00030000
#define JCMD_ACC_IRPDR		0x00040000
#define JCMD_ACC_PDR		0x00050000
#define JCMD_ACC_PIR		0x00060000
#define JCMD_ACC_IRDR_I		0x00070000	
#define JCMD_ACC_DR_I		0x00080000	
#define JCMD_IRW_MASK		0x00001f00
#define JCMD_IRW_SHIFT		8
#define JCMD_DRW_MASK		0x0000003f


#define JCTRL_FORCE_CLK		4		
#define JCTRL_EXT_EN		2		
#define JCTRL_EN		1		


#define	CLKD_SFLASH		0x0f000000
#define	CLKD_SFLASH_SHIFT	24
#define	CLKD_OTP		0x000f0000
#define	CLKD_OTP_SHIFT		16
#define	CLKD_JTAG		0x00000f00
#define	CLKD_JTAG_SHIFT		8
#define	CLKD_UART		0x000000ff


#define	CI_GPIO			0x00000001	
#define	CI_EI			0x00000002	
#define	CI_TEMP			0x00000004	
#define	CI_SIRQ			0x00000008	
#define	CI_ECI			0x00000010	
#define	CI_PMU			0x00000020	
#define	CI_UART			0x00000040	
#define	CI_WDRESET		0x80000000	


#define SCC_SS_MASK		0x00000007	
#define	SCC_SS_LPO		0x00000000	
#define	SCC_SS_XTAL		0x00000001	
#define	SCC_SS_PCI		0x00000002	
#define SCC_LF			0x00000200	
#define SCC_LP			0x00000400	
#define SCC_FS			0x00000800	
#define SCC_IP			0x00001000	
#define SCC_XC			0x00002000	
#define SCC_XP			0x00004000	
#define SCC_CD_MASK		0xffff0000	
#define SCC_CD_SHIFT		16


#define	SYCC_IE			0x00000001	
#define	SYCC_AE			0x00000002	
#define	SYCC_FP			0x00000004	
#define	SYCC_AR			0x00000008	
#define	SYCC_HR			0x00000010	
#define SYCC_CD_MASK		0xffff0000	
#define SYCC_CD_SHIFT		16


#define	CF_EN			0x00000001	
#define	CF_EM_MASK		0x0000000e	
#define	CF_EM_SHIFT		1
#define	CF_EM_FLASH		0		
#define	CF_EM_SYNC		2		
#define	CF_EM_PCMCIA		4		
#define	CF_DS			0x00000010	
#define	CF_BS			0x00000020	
#define	CF_CD_MASK		0x000000c0	
#define	CF_CD_SHIFT		6
#define	CF_CD_DIV2		0x00000000	
#define	CF_CD_DIV3		0x00000040	
#define	CF_CD_DIV4		0x00000080	
#define	CF_CE			0x00000100	
#define	CF_SB			0x00000200	


#define	PM_W0_MASK		0x0000003f	
#define	PM_W1_MASK		0x00001f00	
#define	PM_W1_SHIFT		8
#define	PM_W2_MASK		0x001f0000	
#define	PM_W2_SHIFT		16
#define	PM_W3_MASK		0x1f000000	
#define	PM_W3_SHIFT		24


#define	PA_W0_MASK		0x0000003f	
#define	PA_W1_MASK		0x00001f00	
#define	PA_W1_SHIFT		8
#define	PA_W2_MASK		0x001f0000	
#define	PA_W2_SHIFT		16
#define	PA_W3_MASK		0x1f000000	
#define	PA_W3_SHIFT		24


#define	PI_W0_MASK		0x0000003f	
#define	PI_W1_MASK		0x00001f00	
#define	PI_W1_SHIFT		8
#define	PI_W2_MASK		0x001f0000	
#define	PI_W2_SHIFT		16
#define	PI_W3_MASK		0x1f000000	
#define	PI_W3_SHIFT		24


#define	PW_W0_MASK		0x0000001f	
#define	PW_W1_MASK		0x00001f00	
#define	PW_W1_SHIFT		8
#define	PW_W2_MASK		0x001f0000	
#define	PW_W2_SHIFT		16
#define	PW_W3_MASK		0x1f000000	
#define	PW_W3_SHIFT		24

#define PW_W0       		0x0000000c
#define PW_W1       		0x00000a00
#define PW_W2       		0x00020000
#define PW_W3       		0x01000000


#define	FW_W0_MASK		0x0000003f	
#define	FW_W1_MASK		0x00001f00	
#define	FW_W1_SHIFT		8
#define	FW_W2_MASK		0x001f0000	
#define	FW_W2_SHIFT		16
#define	FW_W3_MASK		0x1f000000	
#define	FW_W3_SHIFT		24


#define WATCHDOG_CLOCK		48000000	
#define WATCHDOG_CLOCK_5354 	32000		


#define	PCTL_ILP_DIV_MASK	0xffff0000
#define	PCTL_ILP_DIV_SHIFT	16
#define PCTL_PLL_PLLCTL_UPD	0x00000400	
#define PCTL_NOILP_ON_WAIT	0x00000200	
#define	PCTL_HT_REQ_EN		0x00000100
#define	PCTL_ALP_REQ_EN		0x00000080
#define	PCTL_XTALFREQ_MASK	0x0000007c
#define	PCTL_XTALFREQ_SHIFT	2
#define	PCTL_ILP_DIV_EN		0x00000002
#define	PCTL_LPO_SEL		0x00000001


#define CSTRETCH_HT		0xffff0000
#define CSTRETCH_ALP		0x0000ffff


#define GPIO_ONTIME_SHIFT	16


#define	CN_N1_MASK		0x3f		
#define	CN_N2_MASK		0x3f00		
#define	CN_N2_SHIFT		8
#define	CN_PLLC_MASK		0xf0000		
#define	CN_PLLC_SHIFT		16


#define	CC_M1_MASK		0x3f		
#define	CC_M2_MASK		0x3f00		
#define	CC_M2_SHIFT		8
#define	CC_M3_MASK		0x3f0000	
#define	CC_M3_SHIFT		16
#define	CC_MC_MASK		0x1f000000	
#define	CC_MC_SHIFT		24


#define	CC_F6_2			0x02		
#define	CC_F6_3			0x03		
#define	CC_F6_4			0x05		
#define	CC_F6_5			0x09
#define	CC_F6_6			0x11
#define	CC_F6_7			0x21

#define	CC_F5_BIAS		5		

#define	CC_MC_BYPASS		0x08
#define	CC_MC_M1		0x04
#define	CC_MC_M1M2		0x02
#define	CC_MC_M1M2M3		0x01
#define	CC_MC_M1M3		0x11


#define	CC_T2_BIAS		2		
#define	CC_T2M2_BIAS		3		

#define	CC_T2MC_M1BYP		1
#define	CC_T2MC_M2BYP		2
#define	CC_T2MC_M3BYP		4


#define	CC_T6_MMASK		1		
#define	CC_T6_M0		120000000	
#define	CC_T6_M1		100000000	
#define	SB2MIPS_T6(sb)		(2 * (sb))


#define	CC_CLOCK_BASE1		24000000	
#define CC_CLOCK_BASE2		12500000	


#define	CLKC_5350_N		0x0311
#define	CLKC_5350_M		0x04020009


#define FLASH_NONE		0x000		
#define SFLASH_ST		0x100		
#define SFLASH_AT		0x200		
#define	PFLASH			0x700		


#define	CC_CFG_EN		0x0001		
#define	CC_CFG_EM_MASK		0x000e		
#define	CC_CFG_EM_ASYNC		0x0000		
#define	CC_CFG_EM_SYNC		0x0002		
#define	CC_CFG_EM_PCMCIA	0x0004		
#define	CC_CFG_EM_IDE		0x0006		
#define	CC_CFG_DS		0x0010		
#define	CC_CFG_CD_MASK		0x00e0		
#define	CC_CFG_CE		0x0100		
#define	CC_CFG_SB		0x0200		
#define	CC_CFG_IS		0x0400		


#define	CC_EB_BASE		0x1a000000	
#define	CC_EB_PCMCIA_MEM	0x1a000000	
#define	CC_EB_PCMCIA_IO		0x1a200000	
#define	CC_EB_PCMCIA_CFG	0x1a400000	
#define	CC_EB_IDE		0x1a800000	
#define	CC_EB_PCMCIA1_MEM	0x1a800000	
#define	CC_EB_PCMCIA1_IO	0x1aa00000	
#define	CC_EB_PCMCIA1_CFG	0x1ac00000	
#define	CC_EB_PROGIF		0x1b000000	



#define SFLASH_OPCODE		0x000000ff
#define SFLASH_ACTION		0x00000700
#define	SFLASH_CS_ACTIVE	0x00001000	
#define SFLASH_START		0x80000000
#define SFLASH_BUSY		SFLASH_START


#define	SFLASH_ACT_OPONLY	0x0000		
#define	SFLASH_ACT_OP1D		0x0100		
#define	SFLASH_ACT_OP3A		0x0200		
#define	SFLASH_ACT_OP3A1D	0x0300		
#define	SFLASH_ACT_OP3A4D	0x0400		
#define	SFLASH_ACT_OP3A4X4D	0x0500		
#define	SFLASH_ACT_OP3A1X4D	0x0700		


#define SFLASH_ST_WREN		0x0006		
#define SFLASH_ST_WRDIS		0x0004		
#define SFLASH_ST_RDSR		0x0105		
#define SFLASH_ST_WRSR		0x0101		
#define SFLASH_ST_READ		0x0303		
#define SFLASH_ST_PP		0x0302		
#define SFLASH_ST_SE		0x02d8		
#define SFLASH_ST_BE		0x00c7		
#define SFLASH_ST_DP		0x00b9		
#define SFLASH_ST_RES		0x03ab		
#define SFLASH_ST_CSA		0x1000		


#define SFLASH_ST_WIP		0x01		
#define SFLASH_ST_WEL		0x02		
#define SFLASH_ST_BP_MASK	0x1c		
#define SFLASH_ST_BP_SHIFT	2
#define SFLASH_ST_SRWD		0x80		


#define SFLASH_AT_READ				0x07e8
#define SFLASH_AT_PAGE_READ			0x07d2
#define SFLASH_AT_BUF1_READ
#define SFLASH_AT_BUF2_READ
#define SFLASH_AT_STATUS			0x01d7
#define SFLASH_AT_BUF1_WRITE			0x0384
#define SFLASH_AT_BUF2_WRITE			0x0387
#define SFLASH_AT_BUF1_ERASE_PROGRAM		0x0283
#define SFLASH_AT_BUF2_ERASE_PROGRAM		0x0286
#define SFLASH_AT_BUF1_PROGRAM			0x0288
#define SFLASH_AT_BUF2_PROGRAM			0x0289
#define SFLASH_AT_PAGE_ERASE			0x0281
#define SFLASH_AT_BLOCK_ERASE			0x0250
#define SFLASH_AT_BUF1_WRITE_ERASE_PROGRAM	0x0382
#define SFLASH_AT_BUF2_WRITE_ERASE_PROGRAM	0x0385
#define SFLASH_AT_BUF1_LOAD			0x0253
#define SFLASH_AT_BUF2_LOAD			0x0255
#define SFLASH_AT_BUF1_COMPARE			0x0260
#define SFLASH_AT_BUF2_COMPARE			0x0261
#define SFLASH_AT_BUF1_REPROGRAM		0x0258
#define SFLASH_AT_BUF2_REPROGRAM		0x0259


#define SFLASH_AT_READY				0x80
#define SFLASH_AT_MISMATCH			0x40
#define SFLASH_AT_ID_MASK			0x38
#define SFLASH_AT_ID_SHIFT			3



#define UART_RX		0	
#define UART_TX		0	
#define UART_DLL	0	
#define UART_IER	1	
#define UART_DLM	1	
#define UART_IIR	2	
#define UART_FCR	2	
#define UART_LCR	3	
#define UART_MCR	4	
#define UART_LSR	5	
#define UART_MSR	6	
#define UART_SCR	7	
#define UART_LCR_DLAB	0x80	
#define UART_LCR_WLEN8	0x03	
#define UART_MCR_OUT2	0x08	
#define UART_MCR_LOOP	0x10	
#define UART_LSR_RX_FIFO 	0x80	
#define UART_LSR_TDHR		0x40	
#define UART_LSR_THRE		0x20	
#define UART_LSR_BREAK		0x10	
#define UART_LSR_FRAMING	0x08	
#define UART_LSR_PARITY		0x04	
#define UART_LSR_OVERRUN	0x02	
#define UART_LSR_RXRDY		0x01	
#define UART_FCR_FIFO_ENABLE 1	


#define UART_IIR_FIFO_MASK	0xc0	
#define UART_IIR_INT_MASK	0xf	
#define UART_IIR_MDM_CHG	0x0	
#define UART_IIR_NOINT		0x1	
#define UART_IIR_THRE		0x2	
#define UART_IIR_RCVD_DATA	0x4	
#define UART_IIR_RCVR_STATUS 	0x6	
#define UART_IIR_CHAR_TIME 	0xc	


#define UART_IER_EDSSI	8	
#define UART_IER_ELSI	4	
#define UART_IER_ETBEI  2	
#define UART_IER_ERBFI	1	


#define	PST_INTPEND	0x0040
#define	PST_SBCLKST	0x0030
#define	PST_SBCLKST_ILP	0x0010
#define	PST_SBCLKST_ALP	0x0020
#define	PST_SBCLKST_HT	0x0030
#define	PST_ALPAVAIL	0x0008
#define	PST_HTAVAIL	0x0004
#define	PST_RESINIT	0x0003


#define PCAP_REV_MASK	0x000000ff
#define PCAP_RC_MASK	0x00001f00
#define PCAP_RC_SHIFT	8
#define PCAP_TC_MASK	0x0001e000
#define PCAP_TC_SHIFT	13
#define PCAP_PC_MASK	0x001e0000
#define PCAP_PC_SHIFT	17
#define PCAP_VC_MASK	0x01e00000
#define PCAP_VC_SHIFT	21
#define PCAP_CC_MASK	0x1e000000
#define PCAP_CC_SHIFT	25
#define PCAP5_PC_MASK	0x003e0000	
#define PCAP5_PC_SHIFT	17
#define PCAP5_VC_MASK	0x07c00000
#define PCAP5_VC_SHIFT	22
#define PCAP5_CC_MASK	0xf8000000
#define PCAP5_CC_SHIFT	27



#define	PRRT_TIME_MASK	0x03ff
#define	PRRT_INTEN	0x0400
#define	PRRT_REQ_ACTIVE	0x0800
#define	PRRT_ALP_REQ	0x1000
#define	PRRT_HT_REQ	0x2000


#define PMURES_BIT(bit)	(1 << (bit))


#define PMURES_MAX_RESNUM	30




#define	PMU0_PLL0_PLLCTL0		0
#define	PMU0_PLL0_PC0_PDIV_MASK		1
#define	PMU0_PLL0_PC0_PDIV_FREQ		25000
#define PMU0_PLL0_PC0_DIV_ARM_MASK	0x00000038
#define PMU0_PLL0_PC0_DIV_ARM_SHIFT	3
#define PMU0_PLL0_PC0_DIV_ARM_BASE	8


#define PMU0_PLL0_PC0_DIV_ARM_110MHZ	0
#define PMU0_PLL0_PC0_DIV_ARM_97_7MHZ	1
#define PMU0_PLL0_PC0_DIV_ARM_88MHZ	2
#define PMU0_PLL0_PC0_DIV_ARM_80MHZ	3 
#define PMU0_PLL0_PC0_DIV_ARM_73_3MHZ	4
#define PMU0_PLL0_PC0_DIV_ARM_67_7MHZ	5
#define PMU0_PLL0_PC0_DIV_ARM_62_9MHZ	6
#define PMU0_PLL0_PC0_DIV_ARM_58_6MHZ	7


#define	PMU0_PLL0_PLLCTL1		1
#define	PMU0_PLL0_PC1_WILD_INT_MASK	0xf0000000
#define	PMU0_PLL0_PC1_WILD_INT_SHIFT	28
#define	PMU0_PLL0_PC1_WILD_FRAC_MASK	0x0fffff00
#define	PMU0_PLL0_PC1_WILD_FRAC_SHIFT	8
#define	PMU0_PLL0_PC1_STOP_MOD		0x00000040


#define	PMU0_PLL0_PLLCTL2		2
#define	PMU0_PLL0_PC2_WILD_INT_MASK	0xf
#define	PMU0_PLL0_PC2_WILD_INT_SHIFT	4


#define RES4328_EXT_SWITCHER_PWM	0	
#define RES4328_BB_SWITCHER_PWM		1	
#define RES4328_BB_SWITCHER_BURST	2	
#define RES4328_BB_EXT_SWITCHER_BURST	3	
#define RES4328_ILP_REQUEST		4	
#define RES4328_RADIO_SWITCHER_PWM	5	
#define RES4328_RADIO_SWITCHER_BURST	6	
#define RES4328_ROM_SWITCH		7	
#define RES4328_PA_REF_LDO		8	
#define RES4328_RADIO_LDO		9	
#define RES4328_AFE_LDO			10	
#define RES4328_PLL_LDO			11	
#define RES4328_BG_FILTBYP		12	
#define RES4328_TX_FILTBYP		13	
#define RES4328_RX_FILTBYP		14	
#define RES4328_XTAL_PU			15	
#define RES4328_XTAL_EN			16	
#define RES4328_BB_PLL_FILTBYP		17	
#define RES4328_RF_PLL_FILTBYP		18	
#define RES4328_BB_PLL_PU		19	

#define RES5354_EXT_SWITCHER_PWM	0	
#define RES5354_BB_SWITCHER_PWM		1	
#define RES5354_BB_SWITCHER_BURST	2	
#define RES5354_BB_EXT_SWITCHER_BURST	3	
#define RES5354_ILP_REQUEST		4	
#define RES5354_RADIO_SWITCHER_PWM	5	
#define RES5354_RADIO_SWITCHER_BURST	6	
#define RES5354_ROM_SWITCH		7	
#define RES5354_PA_REF_LDO		8	
#define RES5354_RADIO_LDO		9	
#define RES5354_AFE_LDO			10	
#define RES5354_PLL_LDO			11	
#define RES5354_BG_FILTBYP		12	
#define RES5354_TX_FILTBYP		13	
#define RES5354_RX_FILTBYP		14	
#define RES5354_XTAL_PU			15	
#define RES5354_XTAL_EN			16	
#define RES5354_BB_PLL_FILTBYP		17	
#define RES5354_RF_PLL_FILTBYP		18	
#define RES5354_BB_PLL_PU		19	



#define DOT11MAC_880MHZ_CLK_DIVISOR_SHIFT 8
#define DOT11MAC_880MHZ_CLK_DIVISOR_MASK (0xFF << DOT11MAC_880MHZ_CLK_DIVISOR_SHIFT)
#define DOT11MAC_880MHZ_CLK_DIVISOR_VAL  (0xE << DOT11MAC_880MHZ_CLK_DIVISOR_SHIFT)


#define PMU2_PHY_PLL_PLLCTL		4
#define PMU2_SI_PLL_PLLCTL		10


#define RES4325_BUCK_BOOST_BURST	0	
#define RES4325_CBUCK_BURST		1	
#define RES4325_CBUCK_PWM		2	
#define RES4325_CLDO_CBUCK_BURST	3	
#define RES4325_CLDO_CBUCK_PWM		4	
#define RES4325_BUCK_BOOST_PWM		5	
#define RES4325_ILP_REQUEST		6	
#define RES4325_ABUCK_BURST		7	
#define RES4325_ABUCK_PWM		8	
#define RES4325_LNLDO1_PU		9	
#define RES4325_OTP_PU			10	
#define RES4325_LNLDO3_PU		11	
#define RES4325_LNLDO4_PU		12	
#define RES4325_XTAL_PU			13	
#define RES4325_ALP_AVAIL		14	
#define RES4325_RX_PWRSW_PU		15	
#define RES4325_TX_PWRSW_PU		16	
#define RES4325_RFPLL_PWRSW_PU		17	
#define RES4325_LOGEN_PWRSW_PU		18	
#define RES4325_AFE_PWRSW_PU		19	
#define RES4325_BBPLL_PWRSW_PU		20	
#define RES4325_HT_AVAIL		21	


#define RES4325B0_CBUCK_LPOM		1	
#define RES4325B0_CBUCK_BURST		2	
#define RES4325B0_CBUCK_PWM		3	
#define RES4325B0_CLDO_PU		4	


#define RES4325C1_OTP_PWRSW_PU		10	
#define RES4325C1_LNLDO2_PU		12	


#define CST4325_SPROM_OTP_SEL_MASK	0x00000003
#define CST4325_DEFCIS_SEL		0	
#define CST4325_SPROM_SEL		1	
#define CST4325_OTP_SEL			2	
#define CST4325_OTP_PWRDN		3	
#define CST4325_SDIO_USB_MODE_MASK	0x00000004
#define CST4325_SDIO_USB_MODE_SHIFT	2
#define CST4325_RCAL_VALID_MASK		0x00000008
#define CST4325_RCAL_VALID_SHIFT	3
#define CST4325_RCAL_VALUE_MASK		0x000001f0
#define CST4325_RCAL_VALUE_SHIFT	4
#define CST4325_PMUTOP_2B_MASK 		0x00000200	
#define CST4325_PMUTOP_2B_SHIFT   	9

#define RES4329_RESERVED0		0	
#define RES4329_CBUCK_LPOM		1	
#define RES4329_CBUCK_BURST		2	
#define RES4329_CBUCK_PWM		3	
#define RES4329_CLDO_PU			4	
#define RES4329_PALDO_PU		5	
#define RES4329_ILP_REQUEST		6	
#define RES4329_RESERVED7		7	
#define RES4329_RESERVED8		8	
#define RES4329_LNLDO1_PU		9	
#define RES4329_OTP_PU			10	
#define RES4329_RESERVED11		11	
#define RES4329_LNLDO2_PU		12	
#define RES4329_XTAL_PU			13	
#define RES4329_ALP_AVAIL		14	
#define RES4329_RX_PWRSW_PU		15	
#define RES4329_TX_PWRSW_PU		16	
#define RES4329_RFPLL_PWRSW_PU		17	
#define RES4329_LOGEN_PWRSW_PU		18	
#define RES4329_AFE_PWRSW_PU		19	
#define RES4329_BBPLL_PWRSW_PU		20	
#define RES4329_HT_AVAIL		21	

#define CST4329_SPROM_OTP_SEL_MASK	0x00000003
#define CST4329_DEFCIS_SEL		0	
#define CST4329_SPROM_SEL		1	
#define CST4329_OTP_SEL			2	
#define CST4329_OTP_PWRDN		3	
#define CST4329_SPI_SDIO_MODE_MASK	0x00000004
#define CST4329_SPI_SDIO_MODE_SHIFT	2


#define RES4312_SWITCHER_BURST		0	
#define RES4312_SWITCHER_PWM    	1	
#define RES4312_PA_REF_LDO		2	
#define RES4312_CORE_LDO_BURST		3	
#define RES4312_CORE_LDO_PWM		4	
#define RES4312_RADIO_LDO		5	
#define RES4312_ILP_REQUEST		6	
#define RES4312_BG_FILTBYP		7	
#define RES4312_TX_FILTBYP		8	
#define RES4312_RX_FILTBYP		9	
#define RES4312_XTAL_PU			10	
#define RES4312_ALP_AVAIL		11	
#define RES4312_BB_PLL_FILTBYP		12	
#define RES4312_RF_PLL_FILTBYP		13	
#define RES4312_HT_AVAIL		14	

#define RES4322_RF_LDO			0
#define RES4322_ILP_REQUEST		1
#define RES4322_XTAL_PU			2
#define RES4322_ALP_AVAIL		3
#define RES4322_SI_PLL_ON		4
#define RES4322_HT_SI_AVAIL		5
#define RES4322_PHY_PLL_ON		6
#define RES4322_HT_PHY_AVAIL		7
#define RES4322_OTP_PU			8


#define CST4322_XTAL_FREQ_20_40MHZ	0x00000020
#define CST4322_SPROM_OTP_SEL_MASK	0x000000c0
#define CST4322_SPROM_OTP_SEL_SHIFT	6
#define CST4322_NO_SPROM_OTP		0	
#define CST4322_SPROM_PRESENT		1	
#define CST4322_OTP_PRESENT		2	
#define CST4322_PCI_OR_USB		0x00000100
#define CST4322_BOOT_MASK		0x00000600
#define CST4322_BOOT_SHIFT		9
#define CST4322_BOOT_FROM_SRAM		0	
#define CST4322_BOOT_FROM_ROM		1	
#define CST4322_BOOT_FROM_FLASH		2	
#define CST4322_BOOT_FROM_INVALID	3
#define CST4322_ILP_DIV_EN		0x00000800
#define CST4322_FLASH_TYPE_MASK		0x00001000
#define CST4322_FLASH_TYPE_SHIFT	12
#define CST4322_FLASH_TYPE_SHIFT_ST	0	
#define CST4322_FLASH_TYPE_SHIFT_ATMEL	1	
#define CST4322_ARM_TAP_SEL		0x00002000
#define CST4322_RES_INIT_MODE_MASK	0x0000c000
#define CST4322_RES_INIT_MODE_SHIFT	14
#define CST4322_RES_INIT_MODE_ILPAVAIL	0	
#define CST4322_RES_INIT_MODE_ILPREQ	1	
#define CST4322_RES_INIT_MODE_ALPAVAIL	2	
#define CST4322_RES_INIT_MODE_HTAVAIL	3	
#define CST4322_PCIPLLCLK_GATING	0x00010000
#define CST4322_CLK_SWITCH_PCI_TO_ALP	0x00020000
#define CST4322_PCI_CARDBUS_MODE	0x00040000

#define RES4315_CBUCK_LPOM		1	
#define RES4315_CBUCK_BURST		2	
#define RES4315_CBUCK_PWM		3	
#define RES4315_CLDO_PU			4	
#define RES4315_PALDO_PU		5	
#define RES4315_ILP_REQUEST		6	
#define RES4315_LNLDO1_PU		9	
#define RES4315_OTP_PU			10	
#define RES4315_LNLDO2_PU		12	
#define RES4315_XTAL_PU			13	
#define RES4315_ALP_AVAIL		14	
#define RES4315_RX_PWRSW_PU		15	
#define RES4315_TX_PWRSW_PU		16	
#define RES4315_RFPLL_PWRSW_PU		17	
#define RES4315_LOGEN_PWRSW_PU		18	
#define RES4315_AFE_PWRSW_PU		19	
#define RES4315_BBPLL_PWRSW_PU		20	
#define RES4315_HT_AVAIL		21	

#define CST4315_SPROM_OTP_SEL_MASK	0x00000003	
#define CST4315_DEFCIS_SEL		0x00000000	
#define CST4315_SPROM_SEL		0x00000001	
#define CST4315_OTP_SEL			0x00000002	
#define CST4315_OTP_PWRDN		0x00000003	
#define CST4315_SDIO_MODE		0x00000004	
#define CST4315_RCAL_VALID		0x00000008
#define CST4315_RCAL_VALUE_MASK		0x000001f0
#define CST4315_RCAL_VALUE_SHIFT	4
#define CST4315_PALDO_EXTPNP		0x00000200	
#define CST4315_CBUCK_MODE_MASK		0x00000c00
#define CST4315_CBUCK_MODE_BURST	0x00000400
#define CST4315_CBUCK_MODE_LPBURST	0x00000c00

#define PMU_MAX_TRANSITION_DLY	15000


#define PMURES_UP_TRANSITION	2





#define ECI_BW_20   0x0
#define ECI_BW_25   0x1
#define ECI_BW_30   0x2
#define ECI_BW_35   0x3
#define ECI_BW_40   0x4
#define ECI_BW_45   0x5
#define ECI_BW_50   0x6
#define ECI_BW_ALL  0x7


#define WLAN_NUM_ANT1 TXANT_0
#define WLAN_NUM_ANT2 TXANT_1

#endif	
