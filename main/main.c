//-----------------------------------------------------------------------------
/// \dir "USB CDC MDIO converter"
///
/// !!!Purpose
///
/// The USB CDC Serial Project will help you to get familiar with the
/// USB Device Port(UDP) and USART interface on AT91SAM microcontrollers. Also
/// it can help you to be familiar with the USB Framework that is used for
/// rapid development of USB-compliant class drivers such as USB Communication
/// Device class (CDC).
///
/// You can find following information depends on your needs:
/// - Sample usage of USB CDC driver and USART driver.
/// - USB MDIO driver development based on the AT91 USB Framework.
/// - USB enumerate sequence, the standard and class-specific descriptors and
///   requests handling.
/// - The initialize sequence and usage of UDP interface.
/// - The initialize sequence and usage of USART interface with PDC.
///
/// !See
/// - usart: USART interface driver
/// - tc: TIMER/COUNTER interface driver
/// - usb: USB Framework, USB CDC driver and UDP interface driver
///    - "AT91 USB device framework"
///       - "USBD API"
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have both
/// UDP and MDIO interface.
///
/// The current supported board list:
/// - at91sam7s-ek (exclude at91sam7s256)
///
/// !!!Description
///
/// When an EK running this program connected to a host (PC for example), with
/// USB cable, the EK appears as a Seriao COM port for the host, after driver
/// installation with the offered *.inf. Then the host can send or receive
/// data through the port with host software. The data stream from the host is
/// then sent to the EK, and forward to USART port of AT91SAM chips. The USART
/// port of the EK is monitored by the timer and the incoming data will be sent
/// to the host.
///

///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the
/// usb-device-cdc-serial-project
///
/// !Contents
///
/// The code can be roughly broken down as follows:
///    - Configuration functions
///       - VBus_Configure
///       - PIO & Timer configurations in start of main
///    - Interrupt handlers
///       - ISR_Vbus
///       - ISR_Timer0
///       - ISR_Usart0
///    - Callback functions
///       - UsbDataReceived
///    - The main function, which implements the program behavior
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <pio.h>
#include <pio_it.h>
#include <aic.h>
#include <tc.h>
#include <usart.h>
#include <trace.h>
#include <led.h>
#include <CDCDSerialDriver.h>
#include <CDCDSerialDriverDescriptors.h>
#include <pmc.h>
#include <drv_mdiom.h>
#include <pkt_mdiom.h>
#include <pwmc.h>
#include <spid.h>
#include <lcd_siem_s65.h>
#include "microrl.h"
//#include <stdio.h>


//------------------------------------------------------------------------------
//      Definitions
//------------------------------------------------------------------------------
extern void LowLevelInit(void);

/// Size in bytes of the buffer used for reading data from the USB & USART
#define DATABUFFERSIZE \
    BOARD_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_DATAIN)

/// Use for power management
#define STATE_IDLE    0
/// The USB device is in suspend state
#define STATE_SUSPEND 4
/// The USB device is in resume state
#define STATE_RESUME  5



/// PWM frequency in Hz.
#define PWM_FREQUENCY	1100000

/// Maximum duty cycle value.
#define MAX_DUTY_CYCLE	10
#define MIN_DUTY_CYCLE	0



//------------------------------------------------------------------------------
//      Internal variables
//------------------------------------------------------------------------------

 /// Pushbutton \#1 pin instance.
const Pin pinPB1 = PIN_PUSHBUTTON_1;

/// Pushbutton \#2 pin instance.
const Pin pinPB2 = PIN_PUSHBUTTON_2;
/// Pushbutton \#3 pin instance.
const Pin pinPB3 = PIN_PUSHBUTTON_3;
/// Pushbutton \#4 pin instance.
const Pin pinPB4 = PIN_PUSHBUTTON_4;

#define PIN_PWMC_PWM0	{1 << 23, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
//#define PIN_PWMC_PWM0	{1 << 23, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
static const Pin PWM0 = PIN_PWMC_PWM0;


/// State of USB, for suspend and resume
uint8_t USBState = STATE_IDLE;


/// SPI driver instance.
static Spid spid;
/// List of pins that must be configured for use by the application.
static const Pin pins[] = {PIN_USART0_TXD, PIN_USART0_RXD};

/// SPI peripheral pins to configure to access the serial flash.
static Pin pins_spi[] = {PINS_SPI};

/// SPI clock frequency used in Hz.
//#define SPCK            3000000
#define SPCK            30000

/// SPI chip select configuration value.
#define CSR             (AT91C_SPI_NCPHA | \
                         SPID_CSR_DLYBCT(BOARD_MCK, 100) | \
                         SPID_CSR_DLYBS(BOARD_MCK, 5) | \
                         SPID_CSR_SCBR(BOARD_MCK, SPCK) | \
						 AT91C_SPI_BITS_8)	
// AT91C_SPI_CSAAT | \



/// Current USART buffer index.
 uint8_t TXCDC[TXCDC_BUF_SIZE];//главный кольцевой буфер передачи
 uint16_t inTXCDC, outTXCDC;	//индексы буфера TXCDC на ввод и вывод

/// Buffer for storing incoming USB data.
//static uint8_t usbBuffer[DATABUFFERSIZE];

extern uint8_t terminal_connect;
static uint8_t first = 0, flash_led = 0;
	
static uint8_t TXCDCdone = false; //флажок завершения передачи
static uint8_t NeedInitCDCDSerialDriver_Read = false;	//флажок завершения приема

static uint8_t usbCDCBuffer[CDCDATABUFFERSIZE];//временный буфер для приема данных CDCDSerialDriver_Read
	
uint8_t RXCDC[RXCDC_BUF_SIZE];//главный кольцевой буфер приема
	
uint8_t inRXCDC = 0, outRXCDC = 0;//индексы  буфера RXCDC на ввод и вывод

static uint8_t MDIO_read = 0, pr_but=0;
extern uint32_t curr_baudrate;
extern microrl_t *prl;

extern void print(const char *);
extern void init_u_consol(void);

PKT_MDIOM_T mdio_m;

//------------------------------------------------------------------------------
//         VBus monitoring (optional)
//------------------------------------------------------------------------------
#if defined(PIN_USB_VBUS)

#define VBUS_CONFIGURE()  VBus_Configure()

/// VBus pin instance.
static const Pin pinVbus = PIN_USB_VBUS;

//------------------------------------------------------------------------------
/// Handles interrupts coming from PIO controllers.
//------------------------------------------------------------------------------
//static void ISR_Vbus(const Pin *pPin)
//{
//    // Check current level on VBus
//    if (PIO_Get(&pinVbus)) {

//        TRACE_INFO("VBUS conn\n\r");
//        USBD_Connect();
//    }
//    else {

//        TRACE_INFO("VBUS discon\n\r");
//        USBD_Disconnect();
//    }
//}

//------------------------------------------------------------------------------
/// Configures the VBus pin to trigger an interrupt when the level on that pin
/// changes.
//------------------------------------------------------------------------------
static void VBus_Configure( void )
{
    TRACE_INFO("VBus configuration\n\r");

//    // Configure PIO
//    PIO_Configure(&pinVbus, 1);
//    PIO_ConfigureIt(&pinVbus, ISR_Vbus);
//    PIO_EnableIt(&pinVbus);

//    // Check current level on VBus
//    if (PIO_Get(&pinVbus)) {

//        // if VBUS present, force the connect
//        TRACE_INFO("VBUS conn\n\r");
//        USBD_Connect();
//    }
//    else {
//        USBD_Disconnect();
//    }
	USBD_Connect();
}

#else
    #define VBUS_CONFIGURE()    USBD_Connect()
#endif //#if defined(PIN_USB_VBUS)




//------------------------------------------------------------------------------
//         Internal functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Handles interrupts coming from Timer #0. 20Hz 50ms
//------------------------------------------------------------------------------
static void ISR_Timer0() __irq
{
    uint16_t status = AT91C_BASE_TC0->TC_SR;
	static uint16_t lastPress1 = 0, lastPress2 = 0;//, lastPress3 = 0, lastPress4 = 0;

    // Check if the button has been pressed
    if (!PIO_Get(&pinPB1)) 
	{
		if(!lastPress1)
		{
			lastPress1 = true;
			pr_but |= 1;
		}
	}
	else
		lastPress1 = false;
	
	if (!PIO_Get(&pinPB2))
	{
		if(!lastPress2)
		{
			lastPress2 = true;
			pr_but |= 2;
		}
	}
	else
		lastPress2 = false;
	
//	if (!PIO_Get(&pinPB3))
//	{
//		if(!lastPress3)
//		{
//			lastPress3 = true;
//			TXCDC[inTXCDC++] = '3'; 
//			inTXCDC &= TXCDC_BUF_MASK;
//		}
//	}
//	else
//		lastPress3 = false;
//	
//	if (!PIO_Get(&pinPB4))
//	{
//		if(!lastPress4)
//		{
//			lastPress4 = true;
//			TXCDC[inTXCDC++] = '4'; 
//			inTXCDC &= TXCDC_BUF_MASK;
//		}
//	}
//	else
//		lastPress4 = false;
	
	
	*AT91C_AIC_EOICR = 0;                     /* End of Interrupt */
}

//------------------------------------------------------------------------------
/// Handles interrupts coming from Timer #0. 4Hz 250ms
//------------------------------------------------------------------------------
static void ISR_Timer1() __irq
{
	// Clear status bit to acknowledge interrupt
    AT91C_BASE_TC1->TC_SR;
	
	flash_led = true;
	MDIO_read = true;
	*AT91C_AIC_EOICR = 0;                     /* End of Interrupt */
}

//------------------------------------------------------------------------------
/// Interrupt service routine for the SPI peripheral. Forwards the interrupt
/// to the SPI driver.
//------------------------------------------------------------------------------
//static void ISR_Spi(void) __irq
//{
//    SPID_Handler(&spid);
//	
//	*AT91C_AIC_EOICR = 0;                     /* End of Interrupt */
//}

//------------------------------------------------------------------------------
/// Interrupt handler for the PWM controller. Counts the number of periods and
/// updates the duty cycle after a given number.
//------------------------------------------------------------------------------
//static void ISR_Pwmc(void) __irq
//{
//    static unsigned int count = 0;
//    static unsigned int duty = MIN_DUTY_CYCLE;
//    static unsigned char fadeIn = 1;

//    // Interrupt on channel #1
//    if ((AT91C_BASE_PWMC->PWMC_ISR & AT91C_PWMC_CHID1) == AT91C_PWMC_CHID1) {

//        count++;

//        // Fade in/out
//        if (count == (PWM_FREQUENCY / (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE))) {

//            // Fade in
//            if (fadeIn) {

//                duty++;
//                if (duty == MAX_DUTY_CYCLE) {

//                    fadeIn = 0;
//                }
//            }
//            // Fade out
//            else {

//                duty--;
//                if (duty == MIN_DUTY_CYCLE) {

//                    fadeIn = 1;
//                }
//            }

//            // Set new duty cycle
//            count = 0;
//            PWMC_SetDutyCycle(0, duty);
//        }
//    }
//	*AT91C_AIC_EOICR = 0;                     /* End of Interrupt */
//}

//------------------------------------------------------------------------------
/// Callback для CDCDSerialDriver_Write,
/// запускается при завершении передачи
//------------------------------------------------------------------------------
static void TXCDCcompleted (void *pArg,
                            uint8_t status,
                            uint16_t transferred,
                            uint16_t remaining)
{
    //trace_LOG(trace_INFO, "TXCDCcompleted: %u\n\r", transferred);
    TXCDCdone = true;
	LED_Clear(1);
}

uint16_t idxDiff (uint16_t idxIN, uint16_t idxOUT)
{
   return (idxIN - idxOUT);
}
//------------------------------------------------------------------------------
/// Callback, автоматически запускаемый по завершении приема
/// блока данных USB CDC. Принятые данные тупо копируются
/// в главный кольцевой буфер приема по индексу inRXCDC,
/// откуда потом данные могут быть легко прочитаны по индексу
/// outRXCDC.
//------------------------------------------------------------------------------
static void UsbCDCDataReceived(unsigned int unused,
                              unsigned char status,
                              unsigned int received,
                              unsigned int remaining)
{
    char *bufpnt;
    // Check that data has been received successfully
	LED_Set(1);
    if (status == USBD_STATUS_SUCCESS)
    {
        bufpnt = (char*)usbCDCBuffer;
        //trace_LOG(trace_INFO, "UsbDataReceived: %u\n\r", received);
        while (received)
        {
            RXCDC[inRXCDC++] = *bufpnt;
            inRXCDC &= RXCDC_BUF_MASK;
            bufpnt++;
            received--;
        }
        // Check if bytes have been discarded
        if ((received == CDCDATABUFFERSIZE) && (remaining > 0))
        {
            TRACE_DEBUG("UsbDataReceived: %u bytes discarded\n\r",remaining);
        }
        NeedInitCDCDSerialDriver_Read = true;
    }
    else
    {
        TRACE_DEBUG("UsbDataReceived: Transfer error\n\r");
    }
}


//------------------------------------------------------------------------------
/// Initializes drivers and start the USB <-> Serial bridge.
//------------------------------------------------------------------------------

void config_board(void)
{
	unsigned int div;
    unsigned int tcclks=0;
	
	LowLevelInit();
	
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
//    printf("-- USB Device CDC Serial Project %s --\n\r", SOFTPACK_VERSION);
//    printf("-- %s\n\r", BOARD_NAME);
//    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

	
    // Configure USART
//    PIO_Configure(pins, PIO_LISTSIZE(pins));
//    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_US0;
//    AT91C_BASE_US0->US_IDR = 0xFFFFFFFF;
//    USART_Configure(AT91C_BASE_US0,
//                    USART_MODE_ASYNCHRONOUS,
//                    115200,
//                    BOARD_MCK);
//    USART_SetTransmitterEnabled(AT91C_BASE_US0, 1);
//    USART_SetReceiverEnabled(AT91C_BASE_US0, 1);
//		AIC_ConfigureIT(AT91C_ID_US0, AT91C_AIC_PRIOR_LOWEST, ISR_Usart0);
//    AIC_EnableIT(AT91C_ID_US0);


/* Basic PWMC */
	 PIO_Configure(&PWM0, 1);
	 AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PWMC;
	 // Settings:
    // - 100kHz PWM period (PWM_FREQUENCY)
    // - 1s rise/fall time for the LED intensity

    // Set clock A to run at 100kHz * MAX_DUTY_CYCLE (clock B is not used)
    PWMC_ConfigureClocks(PWM_FREQUENCY * MAX_DUTY_CYCLE, 0, BOARD_MCK);

    // Configure PWMC channel for LED0 (left-aligned)
    PWMC_ConfigureChannel(CHANNEL_PWM_0, AT91C_PWMC_CPRE_MCKA, 0, 0);
    PWMC_SetPeriod(CHANNEL_PWM_0, MAX_DUTY_CYCLE);
    PWMC_SetDutyCycle(CHANNEL_PWM_0, 5);
	PWMC_EnableChannel(CHANNEL_PWM_0);



/* Initialize the SPI for LCD siemens S65*/
	PIO_Configure(pins_spi, PIO_LISTSIZE(pins_spi));
	// AIC_ConfigureIT(AT91C_ID_SPI, 0, ISR_Spi);
    SPID_Configure(&spid, AT91C_BASE_SPI, AT91C_ID_SPI);
	// Configure the SPI chip select for the serial flash
    SPID_ConfigureCS(&spid, 0, CSR);
	//WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, 0x74);
	AT91C_BASE_SPI->SPI_MR |= AT91C_SPI_PCS;//cs set is 0 channel
	/* Initialize LCD siemens S65*/

	init_lcd();
	LCD_backcolor = yellow;
	fill_screen(LCD_backcolor);
			   //x, y
	//LCD_put_string(60,66,"example",0, BLACK);
	  


// Configure timer 0
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_TC0); //Peripheral Clock Enable
    TC_FindMckDivisor(20, BOARD_MCK, &div, &tcclks); // Configure TC for a 20Hz frequency and trigger on RC compare
    TC_Configure(AT91C_BASE_TC0, tcclks | AT91C_TC_CPCTRG);
    AT91C_BASE_TC0->TC_RC = (BOARD_MCK / div) / 20; // timerFreq / desiredFreq
	//AT91C_BASE_TC1->TC_RC = 0xFFFF;

    AIC_ConfigureIT(AT91C_ID_TC0, AT91C_AIC_PRIOR_LOWEST, ISR_Timer0); // Configure and enable interrupt on RC compare
    AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;
    AIC_EnableIT(AT91C_ID_TC0);
	
	TC_Start(AT91C_BASE_TC0);

//	My Timer1
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_TC1); // Enable peripheral clock
    TC_FindMckDivisor(4, BOARD_MCK, &div, &tcclks); // Configure TC for a 4Hz frequency and trigger on RC compare
    TC_Configure(AT91C_BASE_TC1, tcclks | AT91C_TC_CPCTRG);
    AT91C_BASE_TC1->TC_RC = (BOARD_MCK / div) / 4; // timerFreq / desiredFreq
	//AT91C_BASE_TC1->TC_RC = 0xFFFF;

    AIC_ConfigureIT(AT91C_ID_TC1, AT91C_AIC_PRIOR_LOWEST, ISR_Timer1); // Configure and enable interrupt on RC compare
    AT91C_BASE_TC1->TC_IER = AT91C_TC_CPCS;
    AIC_EnableIT(AT91C_ID_TC1);
	
	TC_Start(AT91C_BASE_TC1);
	
	DRV_MDIOM_Init();
	
	
    // BOT driver initialization
	CDCDSerialDriver_Initialize();

    // connect if needed
	VBUS_CONFIGURE();
	
   // AIC_EnableIT(AT91C_ID_SPI);
	
	PIO_Configure(&pinPB1, 1);
    PIO_Configure(&pinPB2, 1);
	
	LED_Configure(1);
	LED_Configure(2);
	LED_Configure(3);
	
	LED_Configure(0);
	LED_Set(0);


}

//------------------------------------------------------------------------------
//          Main
//------------------------------------------------------------------------------


int main(void)
{
	
	char str[25] = {0,};
	//временные переменные
	uint16_t txbytes_to_send = 0, txbufcnt = 0, rxbytes_to_read = 0;
	//временный буфер для передачи данных CDCDSerialDriver_Write
	//uint8_t txcdcbuf[TXCDC_BUF_SIZE];BOARD_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_DATAIN)
	uint8_t txcdcbuf[BOARD_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_DATAIN)];
	
	config_board();
	
	LED_Set(3);
	
    
	
	// Connect pull-up, wait for configuration
    USBD_Connect();
	
	mdio_m.vPortAddr = 3;
	snprintf (str, 25, "set PHY %2d",mdio_m.vPortAddr);
	LCD_put_string(2,STR1,str,0, BLACK);
	mdio_m.vDevAddr = 1;
	snprintf (str, 25, "set dev is %s",_SCMD_M_DEV_1);
	LCD_put_string(2,STR2,str,0, BLACK);
	
	
	// Driver loop
	while (1)
	{
		if(terminal_connect)
		{
		//узнаем, сколько есть байт для передачи в главном
		// кольцевом буфере. Код для подпрограммы idxDiff см. в [3].
			txbytes_to_send = (idxDiff(inTXCDC, outTXCDC))&TXCDC_BUF_MASK;
		
			//ограничиваем размер блока передаваемых данных
			// размером конечной точки (хотя, как ни странно,
			// все работает нормально и без этого)
			if (txbytes_to_send > BOARD_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_DATAIN))
				txbytes_to_send = BOARD_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_DATAIN);
			//если можно передавать (TXCDCdone), и есть что передавать (txbytes_to_send),
			// скопируем данные из кольцевого буфера TXCDC в txcdcbuf и запустим передачу
			if (TXCDCdone && (0 != txbytes_to_send))
			{
				LED_Set(1);
				
			  for (txbufcnt = 0; txbufcnt < txbytes_to_send; ++txbufcnt) 
				{
					txcdcbuf[txbufcnt] = TXCDC[outTXCDC++];
					outTXCDC &= TXCDC_BUF_MASK;
				}
			  TXCDCdone = false; //запоминаем, что передача запущена
			  CDCDSerialDriver_Write((void*)txcdcbuf, txbytes_to_send, (TransferCallback)TXCDCcompleted, 0);
			}
			
			//[Обработка приема]
			if (NeedInitCDCDSerialDriver_Read)
			{

				NeedInitCDCDSerialDriver_Read = false;
				CDCDSerialDriver_Read(usbCDCBuffer,
									 CDCDATABUFFERSIZE,
									 (TransferCallback) UsbCDCDataReceived,
									 0);
									 
				LED_Clear(1);
				rxbytes_to_read = (idxDiff(inRXCDC, outRXCDC))&RXCDC_BUF_MASK;
				
				// put received char from stdin to microrl lib
				while(rxbytes_to_read)
				{
						microrl_insert_char (prl, RXCDC[outRXCDC]);
					outRXCDC++;
					outRXCDC &= RXCDC_BUF_MASK;	
					--rxbytes_to_read;
				}  
//LED_Toggle(2);
//outRXCDC = inRXCDC;	//заглушка
			}
		}

		if(!first)
		{
			if(terminal_connect)
			{
				first = 1;
				
				init_u_consol();
				
				
				
				if(USBD_GetState() >= USBD_STATE_CONFIGURED)
				{
					inRXCDC = 0;
					outRXCDC = 0;
					inTXCDC = 0;
					outTXCDC = 0;
					TXCDCdone = true;
					NeedInitCDCDSerialDriver_Read = true;
				}

				txbufcnt += sprintf((char *)txcdcbuf,"Hello from AT91SAM7S!\r\n\n");
				while(!TXCDCdone);
				TXCDCdone = false; //запоминаем, что передача запущена
				CDCDSerialDriver_Write((void*)txcdcbuf, txbufcnt, (TransferCallback)TXCDCcompleted, 0);
				LED_Set(2);
				
				fill_area(CHAR_W,STR0,DISP_H-1,DISP_W-1,LCD_backcolor);
				LCD_put_string(CHAR_W,STR0,"Terminal conected!",0, RED);

			}
		}
		else
		{
			if(!terminal_connect)
			{
				first = 0;
				LED_Clear(2);

				//fill_area(0,0,DISP_H/2,DISP_W/2,yellow);
				fill_area(CHAR_W,STR0,DISP_H-1,DISP_W-1,LCD_backcolor);
				LCD_put_string(CHAR_W,STR0,"Terminal disconected!",0, RED);
			}
		}

		if(flash_led)
		{
			flash_led = false;
			LED_Toggle(3);
		}
		
		if(MDIO_read)
		{
			MDIO_read = false;
//			PKT_MDIOM_Inspect();
//			PKT_MDIOM_Probe();
		}
		
		switch(pr_but)
		{
			case 1:
				pr_but &= ~1;
				fill_area(0,0,DISP_H-1,DISP_W-1,GREEN);
				fill_area(59,0,DISP_H-1,DISP_W-1,BLUE);
				fill_area(117,0,DISP_H-1,DISP_W-1,RED);
				break;
			case 2:
				pr_but &= ~2;
				fill_area(0,0,DISP_H-1,DISP_W-1,yellow);
				fill_area(59,0,DISP_H-1,DISP_W-1,ORANGE);
				fill_area(117,0,DISP_H-1,DISP_W-1,grey);
				break;
			default: break;
//			TXCDC[inTXCDC++] = '2'; 
//			inTXCDC &= TXCDC_BUF_MASK;
		}
    }
}





