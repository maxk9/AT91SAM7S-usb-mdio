

#include <board.h>
#include <spid.h>
#include <pio.h>
#include <lcd_siem_s65.h>




// ascii table, starting with character blank (32)
// size is 8x14
const uint8_t ascii_tab[96][14]={
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // space (32)
{ 0x00, 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x3c, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00}, //!
{ 0x00, 0x66, 0x66, 0x66, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //"
{ 0x00, 0x00, 0x00, 0x6c, 0x6c, 0xfe, 0x6c, 0x6c, 0x6c, 0xfe, 0x6c, 0x6c, 0x00, 0x00}, //#
{ 0x00, 0x18, 0x18, 0x7c, 0xc6, 0xc2, 0xc0, 0x7c, 0x06, 0x86, 0xc6, 0x7c, 0x18, 0x18}, //$
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0xc2, 0xc6, 0x0c, 0x18, 0x30, 0x66, 0xc6, 0x00, 0x00}, // %
{ 0x00, 0x00, 0x00, 0x38, 0x6c, 0x6c, 0x38, 0x76, 0xdc, 0xcc, 0xcc, 0x76, 0x00, 0x00}, // &
{ 0x00, 0x18, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // '
{ 0x00, 0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x00, 0x00}, // (
{ 0x00, 0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30, 0x00, 0x00}, // )
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00, 0x00, 0x00}, // *
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00}, // +
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00}, // ґ
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // -
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00}, // .
{ 0x00, 0x00, 0x00, 0x02, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00}, // /
{ 0x00, 0x00, 0x00, 0x38, 0x6c, 0xc6, 0xc6, 0xd6, 0xc6, 0xc6, 0x6c, 0x38, 0x00, 0x00}, // 0 (48-32)
{ 0x00, 0x00, 0x00, 0x18, 0x38, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc6, 0xfe, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x06, 0x06, 0x3c, 0x06, 0x06, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x0c, 0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x0c, 0x1e, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xfe, 0xc0, 0xc0, 0xc0, 0xfc, 0x06, 0x06, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x38, 0x60, 0xc0, 0xc0, 0xfc, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xfe, 0xc6, 0x06, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0x06, 0x0c, 0x78, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0x0c, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xde, 0xde, 0xde, 0xdc, 0xc0, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x66, 0x66, 0x66, 0xfc, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0, 0xc0, 0xc0, 0xc2, 0x66, 0x3c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xf8, 0x6c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xfe, 0x66, 0x62, 0x68, 0x78, 0x68, 0x62, 0x66, 0xfe, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xfe, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x60, 0xf0, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0, 0xc0, 0xde, 0xc6, 0x66, 0x3a, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xe6, 0x66, 0x6c, 0x6c, 0x78, 0x6c, 0x6c, 0x66, 0xe6, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xf0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x62, 0x66, 0xfe, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xc6, 0xee, 0xfe, 0xd6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xc6, 0xe6, 0xf6, 0xfe, 0xde, 0xce, 0xc6, 0xc6, 0xc6, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x60, 0x60, 0x60, 0xf0, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xd6, 0xde, 0x7c, 0x0e, 0x00},
{ 0x00, 0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x6c, 0x66, 0x66, 0xe6, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0x60, 0x38, 0x0c, 0xc6, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x5a, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x10, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xd6, 0xd6, 0xfe, 0x6c, 0x6c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x7c, 0x38, 0x7c, 0xc6, 0xc6, 0xc6, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xfe, 0xc6, 0x8c, 0x18, 0x30, 0x60, 0xc2, 0xc6, 0xfe, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0x0e, 0x06, 0x02, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c, 0x00, 0x00},
{ 0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff},
{ 0x00, 0x30, 0x18, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xe0, 0x60, 0x60, 0x78, 0x6c, 0x66, 0x66, 0x66, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc0, 0xc0, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x1c, 0x0c, 0x0c, 0x3c, 0x6c, 0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xfe, 0xc0, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x1c, 0x36, 0x32, 0x30, 0x7c, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0xcc, 0x78},
{ 0x00, 0x00, 0x00, 0xe0, 0x60, 0x60, 0x6c, 0x76, 0x66, 0x66, 0x66, 0xe6, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x0e, 0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3c},
{ 0x00, 0x00, 0x00, 0xe0, 0x60, 0x60, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0xe6, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xec, 0xfe, 0xd6, 0xd6, 0xd6, 0xd6, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x66, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0x0c, 0x1e},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x76, 0x66, 0x60, 0x60, 0xf0, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x70, 0x1c, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x10, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x36, 0x1c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x10, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xd6, 0xd6, 0xfe, 0x6c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0x6c, 0x38, 0x38, 0x6c, 0xc6, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0x0c, 0x78},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xcc, 0x18, 0x30, 0x66, 0xfe, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x0e, 0x18, 0x18, 0x18, 0x70, 0x18, 0x18, 0x18, 0x0e, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x70, 0x18, 0x18, 0x18, 0x0e, 0x18, 0x18, 0x18, 0x70, 0x00, 0x00},
{ 0x00, 0x76, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe, 0x00, 0x00, 0x00},
};

uint16_t LCD_backcolor=0xFFFF;    // color of background
uint16_t LCD_textcolor=0x0000;





/* ------------------------------------------------------------------------- */
//интерфейс к lCD

/// LED #0 pin definition (PA0).
#define PIN_LCD_RS   {AT91C_PIO_PA7, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
/// LED #1 pin definition (PA31).
#define PIN_LCD_RESET   {AT91C_PIO_PA31, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}

#define PINS_LCD_ctrl   PIN_LCD_RS, PIN_LCD_RESET

static const Pin pinsLCD[] = {PINS_LCD_ctrl};



//------------------------------------------------------------------------------
/// Wait time in ms
//------------------------------------------------------------------------------
 void UTIL_Loop(unsigned int loop)
{
    while(loop--);	
}


 void UTIL_WaitTimeInMs(unsigned int mck, unsigned int time_ms)
{
    register unsigned int i = 0;
    i = (mck / 1000) * time_ms;
    i = i / 3;
	 while(i--);	
    //UTIL_Loop(i);
}

//------------------------------------------------------------------------------
/// Wait time in us
//------------------------------------------------------------------------------
 void UTIL_WaitTimeInUs(unsigned int mck, unsigned int time_us)
{
    volatile unsigned int i = 0;
    i = (mck / 1000000) * time_us;
    i = i / 3;
	 while(i--);	
    //UTIL_Loop(i);
}



/*посылка команды на LCD*/
void lcd_wr_com(uint16_t val)
{
	while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
	WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, 0x74);
	while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
	WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, 0x00);
	while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
	WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, (val&0xFF));
	
	UTIL_WaitTimeInUs(BOARD_MCK,1);
	
}

/*посылка команды на LCD*/
void lcd_wr_dat(uint8_t val1,uint8_t val2)
{

	while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
	WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, 0x76);
	while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
	WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, val1);
	while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
	WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, val2);
	UTIL_WaitTimeInUs(BOARD_MCK,1);
	
}

/*инициализация LCD*/

 void init_lcd(void)
 {
	 uint8_t ct1[6]={	0x07,		0x02,		0x04,		0x0C,	0x0D,		0x0E};
	 uint8_t cd1[12]={0x00,0x00, 0x04,0x00, 0x00,0x00, 0x00,0x01, 0x06,0x16, 0x10,0x10};
	 uint8_t ct2[2]={	0x03,		0x0E};
	 uint8_t cd2[4]={0x00,0x0C, 0x2D,0x1F};
	 uint8_t ct3[1]={	0x0D};
	 uint8_t cd3[2]={0x06,0x16};
	 uint8_t ct4[21]={	0x01,		0x0F,		0x0B,	 0x11,		0x06,		0x05,		0x14,	0x15,		0x16,		0x17,		0x20,	0x30,		0x31,		0x32,		0x33,		0x34,	0x35,		0x36,		0x37,		0x3A,	0x3B};
	uint8_t cd4[42]={0x02,0x15, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x30, 0xAF,0x00, 0x00,0x00, 0x83,0x00, 0xAF,0x00,	0x00,0x00, 0x00,0x00, 0x04,0x00, 0x02,0x07, 0x07,0x00, 0x00,0x05, 0x07,0x03, 0x07,0x07, 0x00, 0x07, 0x12,0x00, 0x00,0x09};
	// uint8_t cd4[42]={0x02,0x15, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x30, 0xAF,0x00, 0x00,0x00, 0x83,0x00, 0xAF,0x00,	0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00, 0x00, 0x00,0x00, 0x00,0x00};		 

	 uint8_t i,j;

	 // /*Инициализация LCD*/
	 PIO_Configure(pinsLCD, 2);
	UTIL_WaitTimeInMs(BOARD_MCK,50);	
	 PIO_Clear(&pinsLCD[1]);	//PIN_LCD_RESET	
	 PIO_Clear(&pinsLCD[0]);	//PIN_LCD_RS	RS is not used, set it to high	
	UTIL_WaitTimeInMs(BOARD_MCK,2);
	 PIO_Set(&pinsLCD[1]);	//PIN_LCD_RESET 
	UTIL_WaitTimeInMs(BOARD_MCK,10);

	 while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
	 WRITE_SPI(AT91C_BASE_SPI, SPI_MR, AT91C_SPI_MSTR | AT91C_SPI_MODFDIS | AT91C_SPI_PS_FIXED);

	 // starts at 58.8ms
	 lcd_wr_com(0x00);
	 lcd_wr_dat(0x00,0x01);
	  
	  UTIL_WaitTimeInMs(BOARD_MCK,10);
	 // starts at 125ms
	 j=0;
	 for (i=0; i<6; i++)
	 {
		 lcd_wr_com(ct1[i]);
		 lcd_wr_dat(cd1[j],cd1[j+1]);
		 j+=2;
	 }
	 UTIL_WaitTimeInMs(BOARD_MCK,10);
	 // start at 240ms
	 j=0;
	 for (i=0; i<2; i++)
	 {
		 lcd_wr_com(ct2[i]);
		 lcd_wr_dat(cd2[j],cd2[j+1]);
		 j+=2;
	 }
	 UTIL_WaitTimeInMs(BOARD_MCK,10);
	 // start at 355ms
	 j=0;
	 for (i=0; i<1; i++)
	 {
		 lcd_wr_com(ct3[i]);
		 lcd_wr_dat(cd3[j],cd3[j+1]);
		 j+=2;
	 }
	 // start at 458ms
	 j=0;
	 for (i=0; i<21; i++)
	 {
		 lcd_wr_com(ct4[i]);
		 lcd_wr_dat(cd4[j],cd4[j+1]);
		 j+=2;
	 }
	 lcd_wr_com(0x07);
	 lcd_wr_dat(0x00, 0x05);
	 lcd_wr_com(0x07);
	 lcd_wr_dat(0x00, 0x25);
	 lcd_wr_com(0x07);
	 lcd_wr_dat(0x00, 0x27);
	 lcd_wr_com(0x07);
	 lcd_wr_dat(0x00, 0x37);



	 /*Инициализация LCD конец*/
 }
const uint8_t LCD_fill_masiv[7][3] = {                 //masiv dlya zapolneniya ekrana
                                      0x74,0x00,0x16,
                                      0x76,0x83,0x00,
                                      0x74,0x00,0x17,
                                      0x76,0xAF,0x00,
                                      0x74,0x00,0x21,
                                      0x76,0x00,0x00,
                                      0x74,0x00,0x22
                                      };

void LCD_fill(void)
{
	uint8_t i=0;
	uint16_t i1;
	for(i=0;i<=6;i++){

        while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
		WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, (LCD_fill_masiv[i][0]&0xFF));
		
         while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
		WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, (LCD_fill_masiv[i][1]&0xFF));

         while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
		WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, (LCD_fill_masiv[i][2]&0xFF));

		UTIL_WaitTimeInUs(BOARD_MCK,1);
	}

	for(i1=0;i1<=23232;i1++){
		
		while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
		WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, 0x07);
		while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
		WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, 0x80);
	}
}

void lcd_write16(uint16_t dat)
{
  // lcd_write(dat>>8);
  // lcd_write(dat);
   while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
	WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, (dat>>8)&0xFF);
	while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
	WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, (dat&0xFF));
}


void fill_screen(uint16_t color)
{
  uint16_t i;

  lcd_wr_com(0x16);
  lcd_wr_dat(0x83, 0x00);
  lcd_wr_com(0x17);
  lcd_wr_dat(0xAF, 0x00);
  lcd_wr_com(0x21);
  lcd_wr_dat(0x00, 0x00);
  lcd_wr_com(0x22);
  
  while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
	WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, 0x76);
  
  // start data transmission  
  for (i=0; i<DISP_W*DISP_H; i++)
    lcd_write16(color);
}

void put_char(uint8_t x, uint8_t y, char c, uint8_t rot)
{
  uint8_t h,ch,p,mask;

  
  switch(rot)
  {
    default:
    case 0:
      lcd_wr_com(0x05);
      lcd_wr_dat(0x00,0x38);
      lcd_wr_com(0x17);
      lcd_wr_dat(x+CHAR_W-1, x);
      lcd_wr_com(0x16);
      lcd_wr_dat(y+CHAR_H-1, y);
      lcd_wr_com(0x21);
      lcd_wr_dat(x,y);
      lcd_wr_com(0x22);
	  
		while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
		WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, 0x76); 
      break;
    case 1:
      lcd_wr_com(0x05);
      lcd_wr_dat(0x00,0x30);
      lcd_wr_com(0x16);
      lcd_wr_dat(x+CHAR_W-1, x);
      lcd_wr_com(0x17);
      lcd_wr_dat(y+CHAR_H-1, y);
      lcd_wr_com(0x21);
      lcd_wr_dat(y, x);
      lcd_wr_com(0x22);
       
	   while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
		WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, 0x76);  
      break;
  }

    for (h=0; h<CHAR_H; h++) // every column of the character
  {        
       if (rot==1)
          ch=ascii_tab[ c-32 ][h];	
	else
          ch=ascii_tab[ c-32 ][h];
	
        mask=0x80;
        for (p=0; p<CHAR_W; p++)  // write the pixels
        {
          if (ch&mask)
          {
            lcd_write16(LCD_textcolor);                
          }
          else
          {
            lcd_write16(LCD_backcolor);
          }
          mask=mask>>1;
        }  // for p
  }
  
//  for (h=0; h<CHAR_H; h++) // every column of the character
//  {        
//       if (rot==1)
//          ch=ascii_tab[ c-32 ][h];	
//	else
//          ch=ascii_tab[ c-32 ][CHAR_H-h-1];
//	
//        mask=0x80;
//        for (p=0; p<CHAR_W; p++)  // write the pixels
//        {
//          if (ch&mask)
//          {
//            lcd_write16(LCD_textcolor);                
//          }
//          else
//          {
//            lcd_write16(LCD_backcolor);
//          }
//          mask=mask>>1;
//        }  // for p
//  }
}

/* ------------------------------------------------------------------------- */



