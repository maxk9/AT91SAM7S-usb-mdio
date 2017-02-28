/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
/// \unit
/// !Purpose
/// 
/// Definition of AT91SAM7S-EK characteristics, AT91SAM7S-dependant PIOs and
/// external components interfacing.
/// 
/// !Contents
/// This file provide a large number of definitions, which are of three
/// different types.
///
/// PIO definitions are prefixed with #PIN_# or #PINS_#. They are to be used
/// with the pio peripheral to configure the pins required by the application.
///
/// First, additional information about the platform is provided by several
/// constants:
///    - BOARD_NAME is a string containing the board name
///    - The chip family and board (at91sam7s and at91sam7sek) are also
///      provided.
///    - BOARD_MAINOSC and BOARD_MCK contains the standard frequency for the
///      main oscillator and the master clock.
///
/// Contants prefixed with #BOARD_USB_# give information about the USB device
/// peripheral that is provided in the chip.
///
/// Defines prefixed with #PIN_# contain only one pin (and thus can be safely
/// used to initialize a single Pin instance), whereas defines starting with
/// #PINS_# contains either a single Pin instance with multiple pins inside it,
/// or a list of several Pin instances; they must be used as Pin[] array
/// initializer values, otherwise they are not safe.
///
/// Finally, some information about the flash controller is given by definitions
/// prefixed with #BOARD_FLASH_#.
//------------------------------------------------------------------------------

#ifndef BOARD_H 
#define BOARD_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <AT91SAM7S256.H>  
#include <stdint.h>

//------------------------------------------------------------------------------
//         Global Definitions
//------------------------------------------------------------------------------
#define true 1
#define false !true
//------------------------------------------------------------------------------
// Board
//------------------------------------------------------------------------------
/// String containing the name of the board.
#define BOARD_NAME      "AT91SAM7S-EK"
/// Board definition.
#define at91sam7sek
/// Family definition.
#define at91sam7s
//------------------------------------------------------------------------------


#define SetBit(reg,bit)	reg |= (1<<bit) 	
//------------------------------------------------------------------------------
// Clocks
//------------------------------------------------------------------------------
/// Frequency of the board main oscillator, in Hz.
#define BOARD_MAINOSC           18432000UL

/// Master clock frequency (when using board_lowlevel.c), in Hz.
#define BOARD_MCK               48000000UL
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ADC
//------------------------------------------------------------------------------
/// ADC clock frequency, at 10-bit resolution (in Hz)
#define ADC_MAX_CK_10BIT         5000000
/// ADC clock frequency, at 8-bit resolution (in Hz)
#define ADC_MAX_CK_8BIT          8000000
/// Startup time max, return from Idle mode (in �s)
#define ADC_STARTUP_TIME_MAX       20
/// Track and hold Acquisition Time min (in ns)
#define ADC_TRACK_HOLD_TIME_MIN   600

//------------------------------------------------------------------------------
// USB
//------------------------------------------------------------------------------


//размер главного буфера передачи
#define TXCDC_BUF_SIZE 1024
#define TXCDC_BUF_MASK (TXCDC_BUF_SIZE-1)

#define CDCDATABUFFERSIZE BOARD_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_DATAIN)
//размер главного буфера приема
#define RXCDC_BUF_SIZE 256
#define RXCDC_BUF_MASK (RXCDC_BUF_SIZE-1)



/// Indicates the chip has a UDP controller.
#define BOARD_USB_UDP

/// Indicates the D+ pull-up is externally controlled.
#define BOARD_USB_PULLUP_EXTERNAL

/// Number of endpoints in the USB controller.
#define BOARD_USB_NUMENDPOINTS                  4

/// Returns the maximum packet size of the given endpoint.
/// \param i  Endpoint number.
/// \return Maximum packet size in bytes of endpoint. 
#define BOARD_USB_ENDPOINTS_MAXPACKETSIZE(i)	((i == 0) ? 8 : 64)

/// Returns the number of FIFO banks for the given endpoint.
/// \param i  Endpoint number.
/// \return Number of FIFO banks for the endpoint.
#define BOARD_USB_ENDPOINTS_BANKS(i)	(((i == 0) || (i == 3)) ? 1 : 2)

/// USB attributes configuration descriptor (bus or self powered, remote wakeup)
#define BOARD_USB_BMATTRIBUTES		USBConfigurationDescriptor_SELFPOWERED_NORWAKEUP
//#define BOARD_USB_BMATTRIBUTES		USBConfigurationDescriptor_BUSPOWERED_NORWAKEUP
//#define BOARD_USB_BMATTRIBUTES		USBConfigurationDescriptor_SELFPOWERED_RWAKEUP
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Pins
//------------------------------------------------------------------------------
/// DBGU pins definition. Contains DRXD (PA9) and DTXD (PA10).
#define PINS_DBGU  {0x00000600, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// LED #0 pin definition (PA0).
#define PIN_LED_DS1   {AT91C_PIO_PA0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/// LED #1 pin definition (PA1).
#define PIN_LED_DS2   {AT91C_PIO_PA1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/// LED #2 pin definition (PA2).
#define PIN_LED_DS3   {AT91C_PIO_PA2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/// LED #3 pin definition (PA3).
#define PIN_LED_DS4   {AT91C_PIO_PA3, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/// List of the four LED pin definitions (PA0, PA1, PA2 & PA3)
#define PINS_LEDS   PIN_LED_DS1, PIN_LED_DS2, PIN_LED_DS3, PIN_LED_DS4

/// Push button #0 definition (PA19).
#define PIN_PUSHBUTTON_1    {AT91C_PIO_PA19, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// Push button #1 definition (PA20).
#define PIN_PUSHBUTTON_2    {AT91C_PIO_PA20, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// Push button #2 definition (PA15).
#define PIN_PUSHBUTTON_3    {AT91C_PIO_PA15, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// Push button #3 definition (PA14).
#define PIN_PUSHBUTTON_4    {AT91C_PIO_PA14, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// List of all push button definitions (PA19, PA20, PA15 & PA14).
#define PINS_PUSHBUTTONS    PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2, PIN_PUSHBUTTON_3, PIN_PUSHBUTTON_4
/// Push button #1 index.
#define PUSHBUTTON_BP1      0
/// Push button #2 index.
#define PUSHBUTTON_BP2      1
/// Push button #1 index.
#define PUSHBUTTON_BP3      2
/// Push button #2 index.
#define PUSHBUTTON_BP4      3
/// Simulated joystick LEFT index.
#define JOYSTICK_LEFT       0
/// Simulated joystick RIGHT index.
#define JOYSTICK_RIGHT      1

/// USART0 TXD pin definition (PA5).
#define PIN_USART0_RXD  {AT91C_PIO_PA5, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 RXD pin definition (PA6).
#define PIN_USART0_TXD  {AT91C_PIO_PA6, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 RTS pin definition.
#define PIN_USART0_RTS  {AT91C_PIO_PA7, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 CTS pin definition.
#define PIN_USART0_CTS  {AT91C_PIO_PA8, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 SCK pin definition.
#define PIN_USART0_SCK  {AT91C_PIO_PA2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}

/// SPI MISO pin definition (PA12).
#define PIN_SPI_MISO   {AT91C_PIO_PA12, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI MOSI pin definition (PA13).
#define PIN_SPI_MOSI   {AT91C_PIO_PA13, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI SPCK pin definition (PA14).
#define PIN_SPI_SPCK   {AT91C_PIO_PA14, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// SPI chip select 0 pin definition (PA11).
#define PIN_SPI_NPCS0  {AT91C_PIO_PA11, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI chip select 1
#define PIN_SPI_NPCS1  {AT91C_PIO_PA31, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI chip select 2
#define PIN_SPI_NPCS2  {AT91C_PIO_PA10, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
/// SPI chip select 3
#define PIN_SPI_NPCS3  {AT91C_PIO_PA3, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}

/// SPI pins definition. Contains MISO, MOSI & SPCK (PA12, PA13 & PA14).
#define PINS_SPI      PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SPCK, PIN_SPI_NPCS0

/// PWMC PWM0 pin definition (PA0).
//#define PIN_PWMC_PWM0  {1 << 0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// PWMC PWM0 pin definition (PA1).
#define PIN_PWMC_PWM1  {AT91C_PIO_PA1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// PWMC PWM0 pin definition (PA2).
#define PIN_PWMC_PWM2  {AT91C_PIO_PA2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// PWM pin definition for LED0
#define PIN_PWM_LED0 PIN_PWMC_PWM1
/// PWM pin definition for LED1
#define PIN_PWM_LED1 PIN_PWMC_PWM2
/// PWM channel for LED0
#define CHANNEL_PWM_0 0
/// PWM channel for LED1
#define CHANNEL_PWM_LED1 2

/// TWI pins definition. Contains TWD (PA3) and TWCK (PA4).
#define PINS_TWI  {0x00000018, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// ADC_AD0 pin definition.
#define PIN_ADC_AD0 {AT91C_PIO_PA17, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT}
/// ADC_AD1 pin definition.
#define PIN_ADC_AD1 {AT91C_PIO_PA18, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT}
/// ADC_AD2 pin definition. (mixed with PIN_PUSHBUTTON_1)
#define PIN_ADC_AD2 {AT91C_PIO_PA19, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT}
/// ADC_AD3 pin definition. (mixed with PIN_PUSHBUTTON_2)
#define PIN_ADC_AD3 {AT91C_PIO_PA20, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT}
/// Pins ADC
#define PINS_ADC PIN_ADC_AD0, PIN_ADC_AD1, PIN_ADC_AD2, PIN_ADC_AD3

/// USB VBus monitoring pin definition (PA13).
#define PIN_USB_VBUS    {AT91C_PIO_PA13, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT}
/// USB pull-up control pin definition (PA16).
#define PIN_USB_PULLUP  {AT91C_PIO_PA16, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
//------------------------------------------------------------------------------

/// !SD Card SPI
/// - BOARD_SD_SPI_BASE
/// - BOARD_SD_SPI_ID  
/// - BOARD_SD_SPI_PINS
/// - BOARD_SD_NPCS    

/// Not define in our board, but customer can add this feature
/// Base address of the SPI peripheral connected to the SD card.
#define BOARD_SD_SPI_BASE   AT91C_BASE_SPI
/// Identifier of the SPI peripheral connected to the SD card.
#define BOARD_SD_SPI_ID     AT91C_ID_SPI
/// List of pins to configure to access the SD card
#define BOARD_SD_SPI_PINS   PINS_SPI, PIN_SPI_NPCS1
/// NPCS number
#define BOARD_SD_NPCS       1

//------------------------------------------------------------------------------
// Flash
//------------------------------------------------------------------------------
/// Indicates chip has an EFC.
#define BOARD_FLASH_EFC
/// Address of the IAP function in ROM.
#define BOARD_FLASH_IAP_ADDRESS         0x300E08
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7S-EK - External components"
/// This page lists the definitions related to external on-board components
/// located in the board.h file for the SAM7S-EK.
/// 
/// !ISO7816
/// - PIN_SMARTCARD_CONNECT
/// - PIN_ISO7816_RSTMC
/// - PINS_ISO7816

/// Smartcard detection pin
#define PIN_SMARTCARD_CONNECT   {AT91C_PIO_PA5, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT}
/// PIN used for reset the smartcard
#define PIN_ISO7816_RSTMC       {AT91C_PIO_PA7, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
/// Pins used for connect the smartcard
#define PINS_ISO7816            PIN_USART0_TXD, PIN_USART0_SCK, PIN_ISO7816_RSTMC
//------------------------------------------------------------------------------

#endif //#ifndef BOARD_H

