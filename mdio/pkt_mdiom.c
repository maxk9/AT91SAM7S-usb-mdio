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


#include <drv_mdiom.h>
#include <pkt_define.h>

#if PKT_MDIOM_SUPPORT

#include "pkt_mdiom.h"

extern PKT_MDIOM_T mdio_m;

//uint16_t PKT_MDIOM_Inspect(void)
//{
//    PKT_SetPayloadSize(0x00);
//    return PKT_REPLY_Done_ExecNoError;
//}

uint16_t PKT_MDIOM_Probe(void)
{
   // PKT_MDIOM_T  *pPayload = (PKT_MDIOM_T  *)PKT_GetPayload();
	PKT_MDIOM_T  *pPayload = &mdio_m;
    uint16_t vData;

    if (!DRV_MDIOM_FrameRead(pPayload->vPortAddr,
                             pPayload->vDevAddr,
                             &vData))
    {
        return PKT_REPLY_Fail_MdioNoAck;
    }

    return PKT_REPLY_Done_ExecNoError;
}

uint16_t PKT_MDIOM_Read(void)
{
    //PKT_MDIOM_T  *pPayload = (PKT_MDIOM_T  *)PKT_GetPayload();
	PKT_MDIOM_T  *pPayload = &mdio_m;

    if (!DRV_MDIOM_Read(pPayload->vPortAddr,
                        pPayload->vDevAddr,
                        pPayload->vOffset,
                        pPayload->vDataCount,
                        pPayload->aData))
    {
        return PKT_REPLY_Fail_MdioNoAck;
    }

    return PKT_REPLY_Done_ExecNoError;
}

uint16_t PKT_MDIOM_Write(void)
{
	PKT_MDIOM_T  *pPayload = &mdio_m;
    //PKT_MDIOM_T  *pPayload = (PKT_MDIOM_T  *)PKT_GetPayload();

    if (!DRV_MDIOM_Write(pPayload->vPortAddr,
                         pPayload->vDevAddr,
                         pPayload->vOffset,
                         pPayload->vDataCount,
                         pPayload->aData))
    {
        return PKT_REPLY_Fail_MdioNoAck;
    }

    return PKT_REPLY_Done_ExecNoError;
}

#endif

