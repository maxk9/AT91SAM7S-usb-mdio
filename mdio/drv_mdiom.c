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

#include <board.h>
#include <pio.h>
//#include "cfg.h"
//#include "drv.h"
#include <drv_mdiom.h>
#include <pkt_mdiom.h>

#define TRUE 1
#define FALSE !TRUE

#if DRV_MDIOM_SUPPORT


extern PKT_MDIOM_T mdio_m;
									
///  MDIO pin definition (PA22).
//#define PIN_MDIO_MDC  {AT91C_PIO_PA21, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, (PIO_OPENDRAIN | PIO_PULLUP) }
#define PIN_MDIO_MDC  {AT91C_PIO_PA21, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, (PIO_OPENDRAIN | PIO_PULLUP) }
///  MDIO pin definition (PA23).
#define PIN_MDIO_MDIO  {AT91C_PIO_PA22, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, (PIO_OPENDRAIN | PIO_PULLUP)}									
									
static const Pin MDIO_pins[] = {PIN_MDIO_MDC, PIN_MDIO_MDIO};


/* porting part */
#if 1

/* can support 3MHz MDC rate; (tested on C8051F321 @ 24MHz) */

//#define MDC_PP()        DRV_IO_Write(IO(IO_DUT_MDIOM_SCL_OEn), LOW)
//#define MDC_OD()        DRV_IO_Write(IO(IO_DUT_MDIOM_SCL_OEn), HIGH)
//#define MDC_SET(_v)     DRV_IO_Write(IO(IO_DUT_MDIOM_SCL_OUT), (_v))
//#define MDC_GET()       DRV_IO_Read(IO(IO_DUT_MDIOM_SCL_IN))

//#define MDIO_PP()       DRV_IO_Write(IO(IO_DUT_MDIOM_SDA_OEn), LOW)
//#define MDIO_OD()       DRV_IO_Write(IO(IO_DUT_MDIOM_SDA_OEn), HIGH)
//#define MDIO_SET(_v)    DRV_IO_Write(IO(IO_DUT_MDIOM_SDA_OUT), (_v))
//#define MDIO_GET()      DRV_IO_Read(IO(IO_DUT_MDIOM_SDA_IN))

#define HIGH			1
#define LOW				0
#define ENABLE			1
#define DISABLE			0

#define MDC_PP()        PIO_Clear(&MDIO_pins[0])
#define MDC_OD()        PIO_Set(&MDIO_pins[0])
#define MDC_SET(_v)     (_v) ? PIO_Set(&MDIO_pins[0]) :  PIO_Clear(&MDIO_pins[0])
#define MDC_HIGH()    	PIO_Set(&MDIO_pins[0])
#define MDC_LOW()       PIO_Clear(&MDIO_pins[0])
#define MDC_GET()       PIO_Get(&MDIO_pins[0])

#define MDIO_PP()       PIO_Clear(&MDIO_pins[1])
#define MDIO_OD()       PIO_Set(&MDIO_pins[1])
#define MDIO_SET(_v)    (_v) ? PIO_Set(&MDIO_pins[1]) :  PIO_Clear(&MDIO_pins[1])
#define MDIO_HIGH()    	PIO_Set(&MDIO_pins[1])
#define MDIO_LOW()      PIO_Clear(&MDIO_pins[1])
#define MDIO_GET()      PIO_Get(&MDIO_pins[1])

#endif


#if 1


//uint16_t 	bMdioStartFrame1=1,
//			bMdioStartFrame0=1,
//			bMdioOperationCode1=1,
//			bMdioOperationCode0=1,
//			bMdioDeviceAddr4=1,
//			bMdioDeviceAddr3=1,
//			bMdioDeviceAddr2=1,
//			bMdioDeviceAddr1=1,
//			bMdioDeviceAddr0=1,
//			bMdioTurnAround1=1,
//			bMdioTurnAround0=1,
//			vMdioAddr = 0,
//			vMdioData = 0x0000;

//#define drv_mdiom_TxBit(_v)         do {                                            \
//                                        CY = (_v);                                  \
//                                        MDC_HIGH();                              \
//                                        MDIO_SET(CY);                               \
//                                        MDC_LOW();                               \
//                                    } while (0)
//#define drv_mdiom_RxBit(_v)         do {                                            \
//                                        MDC_LOW();                               \
//                                        CY = MDIO_GET();                            \
//                                        MDC_HIGH();                              \
//                                        (_v) = CY;                                  \
//                                    } while (0)
#define drv_mdiom_TxBit(_v)         do {                                            \
                                        MDC_HIGH();                              \
                                        MDIO_SET(_v);                               \
                                        MDC_LOW();                               \
                                    } while (0)
#define drv_mdiom_RxBit(_v)         do {   										\
										MDIO_OD();								\
                                        MDC_LOW();                               \
                                        (_v) = MDIO_GET();                          \
                                        MDC_HIGH();                              \
                                    } while (0)



/* 32-bit preamble */
#define drv_mdiom_TxPreamble()	{ 													\
									uint16_t i; 									\
									for (i = 0; i < 32; i++) { 						\
										drv_mdiom_TxBit(1); 						\
									}												\
								}

/* 2-bit start frame */
#define drv_mdiom_TxStartFrame()    do {                                            \
                                        drv_mdiom_TxBit(bMdioStartFrame1);          \
                                        drv_mdiom_TxBit(bMdioStartFrame0);          \
                                    } while (0)
#define drv_mdiom_SetStartFrame(_v1,_v0)  do {                                      \
                                        bMdioStartFrame1 = (_v1);                   \
                                        bMdioStartFrame0 = (_v0);                   \
                                    } while (0)

/* 2-bit operation code */
#define drv_mdiom_TxOperationCode() do {                                            \
                                        drv_mdiom_TxBit(bMdioOperationCode1);       \
                                        drv_mdiom_TxBit(bMdioOperationCode0);       \
                                    } while (0)
#define drv_mdiom_SetOperationCode(_v1,_v0) do {                                    \
                                        bMdioOperationCode1 = (_v1);                \
                                        bMdioOperationCode0 = (_v0);                \
                                    } while (0)

/* 5-bit port address */
#define drv_mdiom_TxPortAddress()   do {                                            \
                                        drv_mdiom_TxBit(bMdioPortAddr4);            \
                                        drv_mdiom_TxBit(bMdioPortAddr3);            \
                                        drv_mdiom_TxBit(bMdioPortAddr2);            \
                                        drv_mdiom_TxBit(bMdioPortAddr1);            \
                                        drv_mdiom_TxBit(bMdioPortAddr0);            \
                                    } while (0)
#define drv_mdiom_SetPortAddress(_v) do {                                           \
                                        bMdioPortAddr4 = ((uint8_t)(_v) >> 4) & 1;    \
                                        bMdioPortAddr3 = ((uint8_t)(_v) >> 3) & 1;    \
                                        bMdioPortAddr2 = ((uint8_t)(_v) >> 2) & 1;    \
                                        bMdioPortAddr1 = ((uint8_t)(_v) >> 1) & 1;    \
                                        bMdioPortAddr0 = ((uint8_t)(_v) >> 0) & 1;    \
                                    } while (0)

/* 5-bit device address */
#define drv_mdiom_TxDeviceAddress() do {                                            \
                                        drv_mdiom_TxBit(bMdioDeviceAddr4);          \
                                        drv_mdiom_TxBit(bMdioDeviceAddr3);          \
                                        drv_mdiom_TxBit(bMdioDeviceAddr2);          \
                                        drv_mdiom_TxBit(bMdioDeviceAddr1);          \
                                        drv_mdiom_TxBit(bMdioDeviceAddr0);          \
                                    } while (0)
#define drv_mdiom_SetDeviceAddress(_v) do {                                         \
                                        bMdioDeviceAddr4 = ((uint8_t)(_v) >> 4) & 1;  \
                                        bMdioDeviceAddr3 = ((uint8_t)(_v) >> 3) & 1;  \
                                        bMdioDeviceAddr2 = ((uint8_t)(_v) >> 2) & 1;  \
                                        bMdioDeviceAddr1 = ((uint8_t)(_v) >> 1) & 1;  \
                                        bMdioDeviceAddr0 = ((uint8_t)(_v) >> 0) & 1;  \
                                    } while (0)

/* 2-bit turn around */
#define drv_mdiom_TxTurnAround()    do {                                            \
                                        drv_mdiom_TxBit(bMdioTurnAround1);          \
                                        drv_mdiom_TxBit(bMdioTurnAround0);          \
                                    } while (0)
#define drv_mdiom_RxTurnAround()    do {                                            \
                                        /* turn-around 1 */                         \
                                        __nop();__nop();							\
                                        MDC_HIGH();                              \
                                        MDIO_HIGH();                             \
                                        MDC_LOW();                               \
                                        MDIO_OD();  /* release MDIO */              \
                                        MDC_HIGH();                              \
                                        __nop();__nop();							\
                                                                                    \
                                        /* turn-around 0 */                         \
                                        drv_mdiom_RxBit(bMdioTurnAround0);          \
                                    } while (0)
#define drv_mdiom_SetTurnAround(_v1, _v0) do {                                      \
                                        bMdioTurnAround1 = (_v1);                   \
                                        bMdioTurnAround0 = (_v0);                   \
                                    } while (0)
#define drv_mdiom_IsReadSuccess()    (bMdioTurnAround0 == 0)

/* 16-bit address/data */
#define drv_mdiom_TxAddr()          { uint16_t i=16;							\
										do {
											--i;									\
											drv_mdiom_TxBit(mdio_m.);		\
																			\
											} while (i)							\
									}
//#define drv_mdiom_TxAddr()          do {                                            \
//                                        drv_mdiom_TxBit(bMdioAddr15);               \
//                                        drv_mdiom_TxBit(bMdioAddr14);               \
//                                        drv_mdiom_TxBit(bMdioAddr13);               \
//                                        drv_mdiom_TxBit(bMdioAddr12);               \
//                                        drv_mdiom_TxBit(bMdioAddr11);               \
//                                        drv_mdiom_TxBit(bMdioAddr10);               \
//                                        drv_mdiom_TxBit(bMdioAddr9);                \
//                                        drv_mdiom_TxBit(bMdioAddr8);                \
//                                        drv_mdiom_TxBit(bMdioAddr7);                \
//                                        drv_mdiom_TxBit(bMdioAddr6);                \
//                                        drv_mdiom_TxBit(bMdioAddr5);                \
//                                        drv_mdiom_TxBit(bMdioAddr4);                \
//                                        drv_mdiom_TxBit(bMdioAddr3);                \
//                                        drv_mdiom_TxBit(bMdioAddr2);                \
//                                        drv_mdiom_TxBit(bMdioAddr1);                \
//                                        drv_mdiom_TxBit(bMdioAddr0);                \
//                                        __nop();__nop();							\
//                                    } while (0)
#define drv_mdiom_TxData()          do {                                            \
                                        drv_mdiom_TxBit(bMdioData15);               \
                                        drv_mdiom_TxBit(bMdioData14);               \
                                        drv_mdiom_TxBit(bMdioData13);               \
                                        drv_mdiom_TxBit(bMdioData12);               \
                                        drv_mdiom_TxBit(bMdioData11);               \
                                        drv_mdiom_TxBit(bMdioData10);               \
                                        drv_mdiom_TxBit(bMdioData9);                \
                                        drv_mdiom_TxBit(bMdioData8);                \
                                        drv_mdiom_TxBit(bMdioData7);                \
                                        drv_mdiom_TxBit(bMdioData6);                \
                                        drv_mdiom_TxBit(bMdioData5);                \
                                        drv_mdiom_TxBit(bMdioData4);                \
                                        drv_mdiom_TxBit(bMdioData3);                \
                                        drv_mdiom_TxBit(bMdioData2);                \
                                        drv_mdiom_TxBit(bMdioData1);                \
                                        drv_mdiom_TxBit(bMdioData0);                \							\
                                    } while (0)
#define drv_mdiom_RxData()          do {                                            \
                                        drv_mdiom_RxBit(bMdioData15);               \
                                        drv_mdiom_RxBit(bMdioData14);               \
                                        drv_mdiom_RxBit(bMdioData13);               \
                                        drv_mdiom_RxBit(bMdioData12);               \
                                        drv_mdiom_RxBit(bMdioData11);               \
                                        drv_mdiom_RxBit(bMdioData10);               \
                                        drv_mdiom_RxBit(bMdioData9);                \
                                        drv_mdiom_RxBit(bMdioData8);                \
                                        drv_mdiom_RxBit(bMdioData7);                \
                                        drv_mdiom_RxBit(bMdioData6);                \
                                        drv_mdiom_RxBit(bMdioData5);                \
                                        drv_mdiom_RxBit(bMdioData4);                \
                                        drv_mdiom_RxBit(bMdioData3);                \
                                        drv_mdiom_RxBit(bMdioData2);                \
                                        drv_mdiom_RxBit(bMdioData1);                \
                                        drv_mdiom_RxBit(bMdioData0);                \
                                    } while (0)
#define drv_mdiom_SetAddr(_v)       do { vMdioAddr = (uint16_t)(_v); } while (0)
#define drv_mdiom_SetData(_v)       do { vMdioData = (uint16_t)(_v); } while (0)
//                                        MDC_PP();                                   
//                                        MDIO_PP();
#define drv_mdiom_StartFrame()      do {                                            \
                                        /* set both pin to push-pull mode,          \
                                         *  to stretch MDIO bus.                    \
                                         */                                         \
										MDC_OD();	\
										MDIO_OD();	\
                                    } while (0)
#define drv_mdiom_StopFrame()       do {                                            \
                                        /* set both pin to open-drain mode,         \
                                         *  to release MDIO bus.                    \
                                         */                                         \
                                        MDC_HIGH();                              \
                                        MDIO_HIGH();                             \
                                        MDC_OD();                                   \
                                        MDIO_OD();                                  \
                                    } while (0)
#endif

	


									
#if 1

static void drv_mdiom_TxAddrFrame(void)
{
    drv_mdiom_StartFrame();
    drv_mdiom_TxPreamble();
    drv_mdiom_TxStartFrame();
    drv_mdiom_TxOperationCode();
    drv_mdiom_TxPortAddress();
    drv_mdiom_TxDeviceAddress();
    drv_mdiom_TxTurnAround();
    drv_mdiom_TxAddr();
    drv_mdiom_StopFrame();
}

static void drv_mdiom_TxDataFrame(void)
{
    drv_mdiom_StartFrame();
    drv_mdiom_TxPreamble();
    drv_mdiom_TxStartFrame();
    drv_mdiom_TxOperationCode();
    drv_mdiom_TxPortAddress();
    drv_mdiom_TxDeviceAddress();
    drv_mdiom_TxTurnAround();
    drv_mdiom_TxData();
    drv_mdiom_StopFrame();
}

static void drv_mdiom_RxDataFrame(void)
{
    drv_mdiom_StartFrame();
    drv_mdiom_TxPreamble();
    drv_mdiom_TxStartFrame();
    drv_mdiom_TxOperationCode();
    drv_mdiom_TxPortAddress();
    drv_mdiom_TxDeviceAddress();
    drv_mdiom_RxTurnAround();
    drv_mdiom_RxData();
    drv_mdiom_StopFrame();
}

/* create MDIO frame: Address */
static void drv_mdiom_CreateAddrFrame
(
     uint8_t    vPortAddr,
     uint8_t    vDevAddr,
     uint16_t   vRegAddr
)
{
    drv_mdiom_SetStartFrame(0, 0);
    drv_mdiom_SetOperationCode(0, 0);
    drv_mdiom_SetPortAddress(vPortAddr);
    drv_mdiom_SetDeviceAddress(vDevAddr);
    drv_mdiom_SetTurnAround(1, 0);
    drv_mdiom_SetAddr(vRegAddr);
}

/* create MDIO frame: Write */
static void drv_mdiom_CreateWriteFrame
(
     uint8_t    vPortAddr,
     uint8_t    vDevAddr,
     uint16_t   vRegData
)
{
    drv_mdiom_SetStartFrame(0, 0);
    drv_mdiom_SetOperationCode(0, 1);
    drv_mdiom_SetPortAddress(vPortAddr);
    drv_mdiom_SetDeviceAddress(vDevAddr);
    drv_mdiom_SetTurnAround(1, 0);
    drv_mdiom_SetData(vRegData);
}

/* create MDIO frame: Read */
static void drv_mdiom_CreateReadFrame
(
     uint8_t    vPortAddr,
     uint8_t    vDevAddr
)
{
    drv_mdiom_SetStartFrame(0, 0);
    drv_mdiom_SetOperationCode(1, 1);
    drv_mdiom_SetPortAddress(vPortAddr);
    drv_mdiom_SetDeviceAddress(vDevAddr);
    drv_mdiom_SetTurnAround(1, 1);
    drv_mdiom_SetData(0xFFFF);
}

/* create MDIO frame: Post-Read-Inc-Addr */
static void drv_mdiom_CreatePRIAFrame
(
     uint8_t    vPortAddr,
     uint8_t    vDevAddr
)
{
    drv_mdiom_SetStartFrame(0, 0);
    drv_mdiom_SetOperationCode(1, 0);
    drv_mdiom_SetPortAddress(vPortAddr);
    drv_mdiom_SetDeviceAddress(vDevAddr);
    drv_mdiom_SetTurnAround(1, 1);
    drv_mdiom_SetData(0xFFFF);
}

#endif


/* MDIO Low-Level API */
#if 1

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_FrameAddress
 * DESCRIPTION:
 *   MDIO Low-Level API: Execute Frame - Address.
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
)
{
   // uint8_t	bIntState;

    /* create MDIO frame: Address */
    drv_mdiom_CreateAddrFrame(vPortAddr, vDevAddr, vRegAddr);

    /* lock globally interrupt */
    __disable_fiq();
	__disable_irq();

    /* transmit frame: Address */
    drv_mdiom_TxAddrFrame();

    /* unlock globally interrupt */
	__enable_fiq();
	__enable_irq();

    /* check frame result */
    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_FrameWrite
 * DESCRIPTION:
 *   MDIO Low-Level API: Execute Frame - Write.
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
)
{
   // uint8_t    bIntState;

    /* create MDIO frame: Write */
    drv_mdiom_CreateWriteFrame(vPortAddr, vDevAddr, vData);

    /* lock globally interrupt */
	__disable_fiq();
	__disable_irq();

    /* transmit frame: Write */
    drv_mdiom_TxDataFrame();

    /* unlock globally interrupt */
   	__enable_fiq();
	__enable_irq();

    /* check frame result */
    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_FrameRead
 * DESCRIPTION:
 *   MDIO Low-Level API: Execute Frame - Read.
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
)
{
//    uint8_t    bIntState;

    /* create MDIO frame: Read */
    drv_mdiom_CreateReadFrame(vPortAddr, vDevAddr);

    /* lock globally interrupt */
    __disable_fiq();
	__disable_irq();

    /* transmit frame: Read */
    drv_mdiom_RxDataFrame();

    /* unlock globally interrupt */
	__enable_fiq();
	__enable_irq();
    /* check read status */
    *pData = vMdioData;
    return drv_mdiom_IsReadSuccess();
}


/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_FramePRIA
 * DESCRIPTION:
 *   MDIO Low-Level API: Execute Frame - Post-Read-Inc-Address.
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
     uint16_t	*pData
)
{
//    uint8_t    bIntState;

    /* create MDIO frame: Post-Read-Inc-Address */
    drv_mdiom_CreatePRIAFrame(vPortAddr, vDevAddr);

    /* lock globally interrupt */
    __disable_fiq();
	__disable_irq();

    /* transmit frame: Read */
    drv_mdiom_RxDataFrame();

    /* unlock globally interrupt */
    __enable_fiq();
	__enable_irq();

    /* check read status */
    *pData = vMdioData;
    return drv_mdiom_IsReadSuccess();
}

#endif


/* MDIO High-Level API */
#if 1

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_Read
 * DESCRIPTION:
 *   MDIO Read Words Data.
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
)
{
    uint8_t    bResult = TRUE;
//    uint8_t    bIntState;

    /* create MDIO frame: Address */
    drv_mdiom_CreateAddrFrame(vPortAddr, vDevAddr, vRegAddr);

    /* lock globally interrupt */
    __disable_fiq();
	__disable_irq();

    /* transmit frame: Address */
    drv_mdiom_TxAddrFrame();

    /* create MDIO frame: Post-Read-Increment-Address */
    drv_mdiom_SetOperationCode(1, 0);

    /* transmit frame: Post-Read-Increment-Address */
    while (vDataLen-- != 0)
    {
        drv_mdiom_RxDataFrame();

        /* check read status */
        *pDataBuf++ = vMdioData;
        if (!drv_mdiom_IsReadSuccess())
        {
            bResult = FALSE;
            break;
        }
    }

    /* unlock globally interrupt */
	__enable_fiq();
	__enable_irq();

    return bResult;
}


/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_Write
 * DESCRIPTION:
 *   MDIO Write Words Data.
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
)
{
//    uint8_t    bIntState;

    for (; vDataLen-- != 0; vRegAddr++, pDataBuf++)
    {
        /* prepare MDIO data */
        drv_mdiom_SetData(*pDataBuf);

        /* create MDIO frame: Address */
        drv_mdiom_CreateAddrFrame(vPortAddr, vDevAddr, vRegAddr);

        /* lock globally interrupt */
        __disable_fiq();
		__disable_irq();

        /* transmit frame: Address */
        drv_mdiom_TxAddrFrame();

        /* create MDIO frame: Write */
        drv_mdiom_SetOperationCode(0, 1);

        /* transmit frame: Write */
        drv_mdiom_TxDataFrame();

        /* unlock globally interrupt */
        __enable_fiq();
		__enable_irq();
    }

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *   DRV_MDIOM_Init
 * DESCRIPTION:
 *   MDIO Driver Init.
 * PARAMETERS:
 *   N/A
 * RETURN:
 *   N/A
 * NOTES:
 *   N/A
 * HISTORY:
 *   2016.1.29        Panda.Xiong          Create
 *****************************************************************************/
void DRV_MDIOM_Init(void)
{
	PIO_Configure(MDIO_pins, PIO_LISTSIZE(MDIO_pins));
    /* stop frame */
    drv_mdiom_StopFrame();
}

#endif

#endif

