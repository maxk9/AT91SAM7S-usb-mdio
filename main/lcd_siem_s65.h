
#ifndef LCD_SIEM_S65_H 
#define LCD_SIEM_S65_H

#include <stdint.h>



#define DISP_W 132
#define DISP_H 176
#define CHAR_H 14
#define CHAR_W 8
#define TEXT_COL 16
#define TEXT_ROW 12

#define STR0 (DISP_W-CHAR_H)
#define STR1 (DISP_W-2*CHAR_H)
#define STR2 (DISP_W-3*CHAR_H)
#define STR3 (DISP_W-4*CHAR_H)
#define STR4 (DISP_W-5*CHAR_H)
#define STR5 (DISP_W-6*CHAR_H)
#define STR6 (DISP_W-7*CHAR_H)
#define STR7 (DISP_W-8*CHAR_H)
#define STR8 (DISP_W-9*CHAR_H)


#define BLACK 0x0000
#define RED   0xf800
#define GREEN 0x07e0
#define BLUE  0x001f
#define WHITE 0xffff
#define ORANGE 0xfca0
#define sky 0x54fb
#define yellow 0xDEE0
#define orange 0xAB00
#define pink 0xF97F
#define brown 0x8200
#define grey 0x8410
#define violet 0x91D4

extern uint16_t LCD_backcolor;    // color of background
extern uint16_t LCD_textcolor;


//extern void put_char(uint8_t x, uint8_t y, char c, uint8_t rot);
extern void put_char(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint8_t rot);
extern void LCD_put_string(uint8_t x, uint8_t y, char *str,uint8_t orient, uint16_t color);

extern void init_lcd(void);
extern void LCD_fill(void);
extern void fill_screen(uint16_t color);
extern void fill_area( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint16_t color);
extern void do_paint_area(uint16_t color);


#endif //#ifndef LCD_SIEM_S65_H
