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
 *   Simulated MDIO Master driver related.
 * HISTORY:
 *   2016.1.29        Panda.Xiong          Create
 *
 *****************************************************************************/

#ifndef __DRV_MDIOM_H
#define __DRV_MDIOM_H

#include <stdint.h>


#define DRV_MDIOM_SUPPORT	1

#if DRV_MDIOM_SUPPORT

/* MDIO Low-Level API */
#if 1

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_FrameAddress
 * DESCRIPTION:
 *   MDIO Master Low-Level API: Execute Frame - Address.
 * PARAMETERS:
 *   vPortAddr : MDIO Port Address;
 *   vDevAddr  : MDIO Device Address;
 *   vRegAddr  : MDIO Register Address;
 * RETURN:
 *   TRUE  : MDIO Frame Executed Success;
 *   FALSE : MDIO Frame Executed Fail;
 * NOTES:
 *   N/A
 * HISTORY:
 *   2016.1.29        Panda.Xiong          Create
 *****************************************************************************/
uint8_t DRV_MDIOM_FrameAddress
(
    uint8_t	vPortAddr,
	uint8_t	vDevAddr,
	uint16_t	vRegAddr
);

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_FrameWrite
 * DESCRIPTION:
 *   MDIO Master Low-Level API: Execute Frame - Write.
 * PARAMETERS:
 *   vPortAddr : MDIO Port Address;
 *   vDevAddr  : MDIO Device Address;
 *   vData     : MDIO Write Data;
 * RETURN:
 *   TRUE  : MDIO Frame Executed Success;
 *   FALSE : MDIO Frame Executed Fail;
 * NOTES:
 *   N/A
 * HISTORY:
 *   2016.1.29        Panda.Xiong          Create
 *****************************************************************************/
uint8_t DRV_MDIOM_FrameWrite
(
      uint8_t	vPortAddr,
      uint8_t	vDevAddr,
      uint16_t	vData
);

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_FrameRead
 * DESCRIPTION:
 *   MDIO Master Low-Level API: Execute Frame - Read.
 * PARAMETERS:
 *   vPortAddr : MDIO Port Address;
 *   vDevAddr  : MDIO Device Address;
 *   pData     : MDIO Read Data;
 * RETURN:
 *   TRUE  : MDIO Frame Executed Success;
 *   FALSE : MDIO Frame Executed Fail;
 * NOTES:
 *   N/A
 * HISTORY:
 *   2016.1.29        Panda.Xiong          Create
 *****************************************************************************/
uint8_t DRV_MDIOM_FrameRead
(
      uint8_t	vPortAddr,
      uint8_t	vDevAddr,
      uint16_t	*pData
);

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_FramePRIA
 * DESCRIPTION:
 *   MDIO Master Low-Level API: Execute Frame - Post-Read-Inc-Address.
 * PARAMETERS:
 *   vPortAddr : MDIO Port Address;
 *   vDevAddr  : MDIO Device Address;
 *   pData     : MDIO Read Data;
 * RETURN:
 *   TRUE  : MDIO Frame Executed Success;
 *   FALSE : MDIO Frame Executed Fail;
 * NOTES:
 *   N/A
 * HISTORY:
 *   2016.1.29        Panda.Xiong          Create
 *****************************************************************************/
uint8_t DRV_MDIOM_FramePRIA
(
      uint8_t	vPortAddr,
      uint8_t	vDevAddr,
     uint16_t 	*pData
);

#endif

void DRV_MDIO_init(void);
/* MDIO High-Level API */
#if 1

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_ReadWords
 * DESCRIPTION:
 *   MDIO Master Read Words Data.
 * PARAMETERS:
 *   vPortAddr : MDIO Port Address;
 *   vDevAddr  : MDIO Device Address;
 *   vRegAddr  : MDIO Register Address;
 *   vDataLen  : MDIO Read Data Length;
 *   pDataBuf  : MDIO Read Register Buffer;
 * RETURN:
 *   TRUE  : MDIO Read Words Data Success;
 *   FALSE : MDIO Read Words Data Fail;
 * NOTES:
 *   N/A
 * HISTORY:
 *   2016.1.29        Panda.Xiong          Create
 *****************************************************************************/
uint8_t DRV_MDIOM_Read
(
      uint8_t	vPortAddr,
      uint8_t	vDevAddr,
      uint16_t	vRegAddr,
      uint16_t	vDataLen,
     uint16_t	*pDataBuf
);

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_WriteWords
 * DESCRIPTION:
 *   MDIO Master Write Words Data.
 * PARAMETERS:
 *   vPortAddr : MDIO Port Address;
 *   vDevAddr  : MDIO Device Address;
 *   vRegAddr  : MDIO Register Address;
 *   vDataLen  : MDIO Write Data Length;
 *   pDataBuf  : MDIO Write Register Buffer;
 * RETURN:
 *   TRUE  : MDIO Write Words Data Success;
 *   FALSE : MDIO Write Words Data Fail;
 * NOTES:
 *   N/A
 * HISTORY:
 *   2016.1.29        Panda.Xiong          Create
 *****************************************************************************/
uint8_t DRV_MDIOM_Write
(
           uint8_t	vPortAddr,
           uint8_t	vDevAddr,
           uint16_t	vRegAddr,
           uint16_t	vDataLen,
     const uint16_t	*pDataBuf
);

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_Init
 * DESCRIPTION:
 *   MDIO Master Driver Init.
 * PARAMETERS:
 *   N/A
 * RETURN:
 *   N/A
 * NOTES:
 *   N/A
 * HISTORY:
 *   2016.1.29        Panda.Xiong          Create
 *****************************************************************************/
void DRV_MDIOM_Init(void);

#endif

#endif


#endif /* __DRV_MDIOM_H */

