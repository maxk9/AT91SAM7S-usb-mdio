/******************************************************************************
 *
 * COPYRIGHT:
 *   Copyright (c) 2016    PANDA(yaxi1984@gmail.com)    All rights reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *   MA 02111-1307 USA
 *
 * DESCRIPTION:
 *   Packet command: MDIO master related.
 * HISTORY:
 *   2016.1.29        Panda.Xiong          Create
 *
 *****************************************************************************/

#ifndef __PKT_MDIOM_H
#define __PKT_MDIOM_H

#include <pkt_define.h>

/*
AVR platform specific implementation routines (for Atmega8, rewrite for your MC)
*/
#define _AVR_DEMO_VER "1.0"

// definition commands word
#define _CMD_HELP		"help"
#define _CMD_CLEAR		"clear"
#define _CMD_S_M_PHY	"set_MDIO_PHY"
#define _CMD_S_M_DEV	"set_MDIO_dev"
#define _CMD_R_M_W		"read_MDIO_word"
#define _CMD_W_M_W		"write_MDIO_word"
#define _CMD_PR_M_W		"post_read_MDIO_inc"
#define _CMD_PRINT_LCD	"print_LCD"
// arguments for set/clear
	#define _SCMD_M_DEV_1	"PMA/PMD"
	#define _SCMD_M_DEV_2	"WIS"
	#define _SCMD_M_DEV_3	"PCS"

#define _NUM_OF_CMD 8
#define _NUM_OF_SETCLEAR_SCMD 3

#define PKT_PayloadMaxLen	2



typedef struct
{
    uint8_t   vPortAddr;	//PHY address
    uint8_t   vDevAddr;
    uint16_t  vOffset;
    uint16_t  vDataCount;
    uint16_t  aData[PKT_PayloadMaxLen];
} PKT_MDIOM_T;
#define PKT_MDIOM_HEADER_LEN    offsetof(PKT_MDIOM_T, aData)





#endif /* __PKT_MDIOM_H */

