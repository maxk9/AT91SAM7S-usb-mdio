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
 *   USB packet definition.
 * HISTORY:
 *   2016.1.29        Panda.Xiong          Create
 *
 *****************************************************************************/

#ifndef __PKT_DEFINE_H
#define __PKT_DEFINE_H

#include <board.h>

#define PKT_SUPPORT 1
#define PKT_MDIOM_SUPPORT 1




/* frame definition */
#define PKT_MAX_LEN                 32   /* frame maximum length */

#if 1

/* frame content (version 1) */
typedef struct
{
    uint16_t  vCmdReply;                  /* command/reply */
    uint16_t  vPayloadSize;               /* payload size  */
    uint8_t   aPayload[PKT_MAX_LEN-4];    /* payload       */
} CONTENT_REV1_T;

/* frame brief */
typedef struct
{
    /* header */
    uint16_t  vMagicNo;   /* magic number    */
    uint16_t  vVersion;   /* content version */
    uint16_t  vLen;       /* content length  */
    uint16_t  vCRC;       /* content CRC16   */

    /* content */
    union
    {
      uint8_t buf[PKT_MAX_LEN];
      CONTENT_REV1_T  rev1;
    } vContent;
} PKT_T;



/* packet reply related */
#if 1

/* reply state */
#define PKT_GetReplyState(_reply)           (((_reply)>>8)&0xFF)
#define PKT_ReplyState_Idle                 0x00
#define PKT_ReplyState_Done                 0x01
#define PKT_ReplyState_Busy                 0x02
#define PKT_ReplyState_Fail                 0x03

/* reply */
#define PKT_REPLY(_stat, _msg)              (((uint16_t)(_stat)<<8) | ((_msg)<<0))
#define PKT_REPLY_Idle_Ready                PKT_REPLY(PKT_ReplyState_Idle,0x01) /* Standard Reply: Ready to accept host command           */
#define PKT_REPLY_Busy_CmdCaptured          PKT_REPLY(PKT_ReplyState_Busy,0x01) /* Standard Reply: Command is captured but not processed  */
#define PKT_REPLY_Busy_CmdChecking          PKT_REPLY(PKT_ReplyState_Busy,0x02) /* Standard Reply: Command checking is in progress        */
#define PKT_REPLY_Busy_CmdExecuting         PKT_REPLY(PKT_ReplyState_Busy,0x03) /* Standard Reply: Command execution is in progress       */
#define PKT_REPLY_Fail_InvalidContent       PKT_REPLY(PKT_ReplyState_Fail,0x01) /* Standard Reply: Invalid frame content                  */
#define PKT_REPLY_Fail_InvalidCRC           PKT_REPLY(PKT_ReplyState_Fail,0x02) /* Standard Reply: Invalid frame CRC                      */
#define PKT_REPLY_Fail_InvalidCmd           PKT_REPLY(PKT_ReplyState_Fail,0x03) /* Standard Reply: Invalid command                        */
#define PKT_REPLY_Fail_InvalidLen           PKT_REPLY(PKT_ReplyState_Fail,0x04) /* Standard Reply: Invalid payload length                 */
#define PKT_REPLY_Fail_InvalidPwd           PKT_REPLY(PKT_ReplyState_Fail,0x05) /* Standard Reply: Password error                         */
#define PKT_REPLY_Fail_CmdCheckFail         PKT_REPLY(PKT_ReplyState_Fail,0x06) /* Standard Reply: Command checking error without detail  */
#define PKT_REPLY_Fail_CmdCheckTimeout      PKT_REPLY(PKT_ReplyState_Fail,0x07) /* Standard Reply: Command checking timeout               */
#define PKT_REPLY_Fail_CmdExecFail          PKT_REPLY(PKT_ReplyState_Fail,0x10) /* Standard Reply: Command execution error without detail */
#define PKT_REPLY_Done_ExecNoError          PKT_REPLY(PKT_ReplyState_Done,0x40) /* Extended Reply: Command execution without error        */
#define PKT_REPLY_Fail_ExecErrorCode0       PKT_REPLY(PKT_ReplyState_Fail,0x40) /* Extended Reply: Command execution with error code 0    */
#define PKT_REPLY_Fail_ExecErrorCode1       PKT_REPLY(PKT_ReplyState_Fail,0x41) /* Extended Reply: Command execution with error code 1    */
#define PKT_REPLY_Fail_ExecErrorCode2       PKT_REPLY(PKT_ReplyState_Fail,0x42) /* Extended Reply: Command execution with error code 2    */

#endif




#endif




#endif /* __PKT_DEFINE_H */

