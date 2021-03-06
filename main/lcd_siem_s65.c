

#include <board.h>
#include <spid.h>
#include <pio.h>
#include <lcd_siem_s65.h>
#include <string.h>



// ascii table, starting with character blank (32)
// size is 8x14


// массив символов
const uint8_t ascii_tab[167][14]={
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // space (32)
{ 0x00, 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x3c, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00}, // !
{ 0x00, 0x66, 0x66, 0x66, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // "
{ 0x00, 0x00, 0x00, 0x6c, 0x6c, 0xfe, 0x6c, 0x6c, 0x6c, 0xfe, 0x6c, 0x6c, 0x00, 0x00}, // #
{ 0x00, 0x18, 0x18, 0x7c, 0xc6, 0xc2, 0xc0, 0x7c, 0x06, 0x86, 0xc6, 0x7c, 0x18, 0x18}, // $
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
{ 0x00, 0x00, 0x00, 0x18, 0x38, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00, 0x00}, // 1
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc6, 0xfe, 0x00, 0x00}, // 2    50
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x06, 0x06, 0x3c, 0x06, 0x06, 0xc6, 0x7c, 0x00, 0x00}, // 3
{ 0x00, 0x00, 0x00, 0x0c, 0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x0c, 0x1e, 0x00, 0x00}, // 4
{ 0x00, 0x00, 0x00, 0xfe, 0xc0, 0xc0, 0xc0, 0xfc, 0x06, 0x06, 0xc6, 0x7c, 0x00, 0x00}, // 5
{ 0x00, 0x00, 0x00, 0x38, 0x60, 0xc0, 0xc0, 0xfc, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00}, // 6
{ 0x00, 0x00, 0x00, 0xfe, 0xc6, 0x06, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00}, // 7
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00}, // 8
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0x06, 0x0c, 0x78, 0x00, 0x00}, // 9
{ 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00}, // :
{ 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 0x00, 0x00}, // ;
{ 0x00, 0x00, 0x00, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x00, 0x00}, // <     60
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00}, // =
{ 0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x00, 0x00}, // >
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0x0c, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00}, // ?
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xde, 0xde, 0xde, 0xdc, 0xc0, 0x7c, 0x00, 0x00}, // @
{ 0x00, 0x00, 0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // A
{ 0x00, 0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x66, 0x66, 0x66, 0xfc, 0x00, 0x00}, // B
{ 0x00, 0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0, 0xc0, 0xc0, 0xc2, 0x66, 0x3c, 0x00, 0x00}, // C
{ 0x00, 0x00, 0x00, 0xf8, 0x6c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00, 0x00}, // D
{ 0x00, 0x00, 0x00, 0xfe, 0x66, 0x62, 0x68, 0x78, 0x68, 0x62, 0x66, 0xfe, 0x00, 0x00}, // E
{ 0x00, 0x00, 0x00, 0xfe, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x60, 0xf0, 0x00, 0x00}, // F    70
{ 0x00, 0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0, 0xc0, 0xde, 0xc6, 0x66, 0x3a, 0x00, 0x00}, // G
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // H
{ 0x00, 0x00, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00}, // I
{ 0x00, 0x00, 0x00, 0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78, 0x00, 0x00}, // J
{ 0x00, 0x00, 0x00, 0xe6, 0x66, 0x6c, 0x6c, 0x78, 0x6c, 0x6c, 0x66, 0xe6, 0x00, 0x00}, // K
{ 0x00, 0x00, 0x00, 0xf0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x62, 0x66, 0xfe, 0x00, 0x00}, // L
{ 0x00, 0x00, 0x00, 0xc6, 0xee, 0xfe, 0xd6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // M
{ 0x00, 0x00, 0x00, 0xc6, 0xe6, 0xf6, 0xfe, 0xde, 0xce, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // N
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00}, // O
{ 0x00, 0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x60, 0x60, 0x60, 0xf0, 0x00, 0x00}, // P     80
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xd6, 0xde, 0x7c, 0x0e, 0x00}, // Q
{ 0x00, 0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x6c, 0x66, 0x66, 0xe6, 0x00, 0x00}, // R
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0x60, 0x38, 0x0c, 0xc6, 0xc6, 0x7c, 0x00, 0x00}, // S
{ 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x5a, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00}, // T
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00}, // U
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x10, 0x00, 0x00}, // V
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xd6, 0xd6, 0xfe, 0x6c, 0x6c, 0x00, 0x00}, // W
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x7c, 0x38, 0x7c, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // X
{ 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00}, // Y
{ 0x00, 0x00, 0x00, 0xfe, 0xc6, 0x8c, 0x18, 0x30, 0x60, 0xc2, 0xc6, 0xfe, 0x00, 0x00}, // Z     90
{ 0x00, 0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c, 0x00, 0x00}, // [
{ 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0x0e, 0x06, 0x02, 0x00, 0x00}, // '\'
{ 0x00, 0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c, 0x00, 0x00}, // ]
{ 0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // ^
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff}, // _
{ 0x00, 0x30, 0x18, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // `
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00, 0x00}, // a
{ 0x00, 0x00, 0x00, 0xe0, 0x60, 0x60, 0x78, 0x6c, 0x66, 0x66, 0x66, 0x7c, 0x00, 0x00}, // b
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc0, 0xc0, 0xc6, 0x7c, 0x00, 0x00}, // c
{ 0x00, 0x00, 0x00, 0x1c, 0x0c, 0x0c, 0x3c, 0x6c, 0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00}, // d     100
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xfe, 0xc0, 0xc6, 0x7c, 0x00, 0x00}, // e
{ 0x00, 0x00, 0x00, 0x1c, 0x36, 0x32, 0x30, 0x7c, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00}, // f
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0xcc, 0x78}, // g
{ 0x00, 0x00, 0x00, 0xe0, 0x60, 0x60, 0x6c, 0x76, 0x66, 0x66, 0x66, 0xe6, 0x00, 0x00}, // h
{ 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00}, // i
{ 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x0e, 0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3c}, // j
{ 0x00, 0x00, 0x00, 0xe0, 0x60, 0x60, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0xe6, 0x00, 0x00}, // k
{ 0x00, 0x00, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00}, // l
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xec, 0xfe, 0xd6, 0xd6, 0xd6, 0xd6, 0x00, 0x00}, // m
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00}, // n      110
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00}, // o
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x66, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0}, // p
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0x0c, 0x1e}, // q
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x76, 0x66, 0x60, 0x60, 0xf0, 0x00, 0x00}, // r
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x70, 0x1c, 0xc6, 0x7c, 0x00, 0x00}, // s
{ 0x00, 0x00, 0x00, 0x10, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x36, 0x1c, 0x00, 0x00}, // t
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00}, // u
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x10, 0x00, 0x00}, // v
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xd6, 0xd6, 0xfe, 0x6c, 0x00, 0x00}, // w
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0x6c, 0x38, 0x38, 0x6c, 0xc6, 0x00, 0x00}, // x     120
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0x0c, 0x78}, // y
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xcc, 0x18, 0x30, 0x66, 0xfe, 0x00, 0x00}, // z
{ 0x00, 0x00, 0x00, 0x0e, 0x18, 0x18, 0x18, 0x70, 0x18, 0x18, 0x18, 0x0e, 0x00, 0x00}, // {
{ 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00}, // |
{ 0x00, 0x00, 0x00, 0x70, 0x18, 0x18, 0x18, 0x0e, 0x18, 0x18, 0x18, 0x70, 0x00, 0x00}, // }
{ 0x00, 0x60, 0x90, 0x90, 0x60, 0x00, 0x0e, 0x10, 0x10, 0x10, 0x10, 0x0e, 0x00, 0x00}, // ~
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe, 0x00, 0x00, 0x00}, // del
{ 0x00, 0x00, 0x08, 0x92, 0x95, 0x92, 0xd0, 0xd7, 0xb0, 0xb0, 0x90, 0x90, 0x00, 0x00}, // №
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //      190
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 
{ 0x00, 0x00, 0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // A
{ 0x00, 0x00, 0x00, 0xfe, 0x62, 0x60, 0x60, 0x7c, 0x66, 0x66, 0x66, 0xfc, 0x00, 0x00}, // Б
{ 0x00, 0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x66, 0x66, 0x66, 0xfc, 0x00, 0x00}, // B
{ 0x00, 0x00, 0x00, 0xfe, 0x62, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0xf0, 0x00, 0x00}, // Г
{ 0x00, 0x00, 0x00, 0x1c, 0x3c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0xfe, 0x82, 0x00, 0x00}, // Д
{ 0x00, 0x00, 0x00, 0xfe, 0x66, 0x62, 0x68, 0x78, 0x68, 0x62, 0x66, 0xfe, 0x00, 0x00}, // E
{ 0x00, 0x00, 0x00, 0xd6, 0xd6, 0xd6, 0xd6, 0x7c, 0x38, 0x7c, 0xd6, 0xd6, 0x00, 0x00}, // Ж
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x06, 0x1c, 0x06, 0x06, 0x06, 0xc6, 0x7c, 0x00, 0x00}, // З
{ 0x00, 0x00, 0x00, 0xc6, 0xce, 0xde, 0xf6, 0xe6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // И    200
{ 0x00, 0x00, 0x38, 0xc6, 0xce, 0xde, 0xf6, 0xe6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // Й
{ 0x00, 0x00, 0x00, 0xe6, 0x66, 0x6c, 0x6c, 0x78, 0x6c, 0x6c, 0x66, 0xe6, 0x00, 0x00}, // K
{ 0x00, 0x00, 0x00, 0x1e, 0x3e, 0x76, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // Л
{ 0x00, 0x00, 0x00, 0xc6, 0xee, 0xfe, 0xd6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // M
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xfe, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // Н
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00}, // O
{ 0x00, 0x00, 0x00, 0xfe, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // П
{ 0x00, 0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x60, 0x60, 0x60, 0xf0, 0x00, 0x00}, // P
{ 0x00, 0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0, 0xc0, 0xc0, 0xc2, 0x66, 0x3c, 0x00, 0x00}, // C
{ 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x5a, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00}, // T      210
{ 0x00, 0x00, 0x00, 0xce, 0x4c, 0x4c, 0x6c, 0x6c, 0x3c, 0x0c, 0x18, 0xf0, 0x00, 0x00}, // У
{ 0x00, 0x00, 0x00, 0x10, 0x7c, 0xfe, 0x92, 0x92, 0xfe, 0x7c, 0x10, 0x10, 0x00, 0x00}, // Ф
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x7c, 0x38, 0x7c, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // X
{ 0x00, 0x00, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfe, 0x02, 0x00}, // Ц
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xfe, 0x7e, 0x06, 0x06, 0x06, 0x00, 0x00}, // Ч
{ 0x00, 0x00, 0x00, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xfe, 0x00, 0x00}, // Ш
{ 0x00, 0x00, 0x00, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xff, 0x01, 0x00}, // Щ
{ 0x00, 0x00, 0x00, 0xe0, 0x60, 0x60, 0x7e, 0x7e, 0x66, 0x66, 0x7e, 0x7e, 0x00, 0x00}, // Ъ
{ 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xe5, 0xf6, 0xd6, 0xd6, 0xf6, 0xe6, 0x00, 0x00}, // Ы
{ 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xfc, 0xfe, 0xc6, 0xc6, 0xfe, 0xfc, 0x00, 0x00}, // Ь     220
{ 0x00, 0x00, 0x00, 0x7c, 0xfe, 0x86, 0x06, 0x3e, 0x06, 0x86, 0xfe, 0x7c, 0x00, 0x00}, // Э
{ 0x00, 0x00, 0x00, 0xde, 0xd6, 0xd6, 0xd6, 0xf6, 0xd6, 0xd6, 0xd6, 0xde, 0x00, 0x00}, // Ю
{ 0x00, 0x00, 0x00, 0xfe, 0xfe, 0xc6, 0xc6, 0xfe, 0xfe, 0x36, 0x66, 0xc6, 0x00, 0x00}, // Я
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00, 0x00}, // a 
{ 0x00, 0x00, 0x0c, 0x7c, 0xc0, 0xc0, 0xfc, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00}, // б
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xcc, 0xf8, 0xcc, 0xcc, 0xf8, 0x00, 0x00}, // в
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x62, 0x60, 0x60, 0x60, 0xf0, 0x00, 0x00}, // г
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x6c, 0x4c, 0x4c, 0x4c, 0xfe, 0x82, 0x00}, // д
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xfe, 0xc0, 0xc6, 0x7c, 0x00, 0x00}, // e
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd6, 0xd6, 0x7c, 0x38, 0xd6, 0xd6, 0x00, 0x00}, // ж     230
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x46, 0x0c, 0x06, 0x46, 0x3c, 0x00, 0x00}, // з
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xce, 0xd6, 0xe6, 0xc6, 0xc6, 0x00, 0x00}, // и
{ 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0xc6, 0xce, 0xd6, 0xe6, 0xc6, 0xc6, 0x00, 0x00}, // й
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe6, 0x6c, 0x78, 0x6c, 0x66, 0xe6, 0x00, 0x00}, // к
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x36, 0x66, 0x66, 0x66, 0xc6, 0x00, 0x00}, // л
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xee, 0xfe, 0xd6, 0xc6, 0xc6, 0x00, 0x00}, // м
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // н
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00}, // o
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00}, // п
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x66, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xe0}, // p     240
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc0, 0xc0, 0xc6, 0x7c, 0x00, 0x00}, // c
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xec, 0xfe, 0xd6, 0xd6, 0xd6, 0xd6, 0x00, 0x00}, // m
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3e, 0x06, 0x06, 0x3c}, // у
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x7c, 0xd6, 0xd6, 0xd6, 0x7c, 0x10, 0x00}, // ф
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0x6c, 0x38, 0x38, 0x6c, 0xc6, 0x00, 0x00}, // x 
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfe, 0x06, 0x00}, // ц
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x3e, 0x06, 0x06, 0x00, 0x00}, // ч
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xfe, 0x00, 0x00}, // ш
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xff, 0x03, 0x00}, // щ
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x7c, 0x66, 0x66, 0x66, 0x7c, 0x00, 0x00}, // ъ    250
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc2, 0xf2, 0xca, 0xca, 0xca, 0xf2, 0x00, 0x00}, // ы
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x7c, 0x66, 0x66, 0x66, 0x7c, 0x00, 0x00}, // ь
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x46, 0x1e, 0x06, 0x46, 0x3c, 0x00, 0x00}, // э
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0xd6, 0xf6, 0xd6, 0xd6, 0xcc, 0x00, 0x00}, // ю
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xc6, 0xc6, 0x7e, 0x26, 0x66, 0x00, 0x00}  // я     255
};


uint16_t LCD_backcolor=0xFFFF;    // color of background
uint16_t LCD_textcolor=0x0000;

 



/* ------------------------------------------------------------------------- */
//интерфейс к lCD

/// LED #0 pin definition (PA0).
#define PIN_LCD_RS   {AT91C_PIO_PA7, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
/// LED #1 pin definition (PA31).
#define PIN_LCD_RESET   {AT91C_PIO_PA31, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_LCD_CS  	{AT91C_PIO_PA11, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
//#define PINS_LCD_ctrl   PIN_LCD_RS, PIN_LCD_RESET, PIN_LCD_CS

static const Pin pinsLCD[] = {PIN_LCD_RS, PIN_LCD_RESET};

#define LCD_CS_HIGH		AT91C_BASE_SPI->SPI_MR |= (AT91C_SPI_PCS);//cs set is 0 channel
#define LCD_CS_LOW		AT91C_BASE_SPI->SPI_MR &= (~AT91C_SPI_PCS);//cs set is 0 channel

//#define LCD_CS_HIGH PIO_Set(&pinsLCD[2])
//#define LCD_CS_LOW PIO_Clear(&pinsLCD[2])
//#define LCD_CS_HIGH		UTIL_WaitTimeInUs(BOARD_MCK,1)
//#define LCD_CS_LOW		UTIL_WaitTimeInUs(BOARD_MCK,1)
//------------------------------------------------------------------------------
/// Wait time in ms
//------------------------------------------------------------------------------
 void UTIL_WaitTimeInMs(uint32_t mck, uint16_t time_ms)
{
    volatile uint16_t i = 0;
    i = (mck / 1000) * time_ms;
    i = i / 3;
	 while(i--);	
    //UTIL_Loop(i);
}

//------------------------------------------------------------------------------
/// Wait time in us
//------------------------------------------------------------------------------
 void UTIL_WaitTimeInUs(uint32_t mck, uint16_t time_us)
{
    volatile uint16_t i = 0;
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
// запись одного байта в дисплей
void lcd_write8(uint8_t dat)
{
	//READ_SPI(AT91C_BASE_SPI, SPI_RDR);
	//READ_SPI(AT91C_BASE_SPI, SPI_SR);
 while(!(READ_SPI(AT91C_BASE_SPI,SPI_SR)&AT91C_SPI_TDRE));
	WRITE_SPI(AT91C_BASE_SPI, SPI_TDR, dat&0xFF);
}

// запись двух байт
void lcd_write(uint16_t dat)
{
  lcd_write8((dat>>8)&0xFF);
  lcd_write8 (dat&0xFF);
}

// дергаем выводом CS для обозначения конца передачи данных
void lcd_cspulse(void)
{
	//UTIL_WaitTimeInUs(BOARD_MCK,1);
//	LCD_CS_HIGH;
//	__nop();
//	LCD_CS_LOW;
	UTIL_WaitTimeInUs(BOARD_MCK,5);
}

// выбор регистра в контроллере дисплея
void lcd_reg(uint8_t register_name)
{
  lcd_write8(0x74);  // стартовый байт на передачу команды
  lcd_write(register_name);
  lcd_cspulse();
}

// отправка двух байт в графическую память дисплея
void lcd_dat8(uint8_t register_dat1, uint8_t register_dat2)
{
  lcd_write8(0x76);  // стартовый байт на запись данных
  lcd_write8(register_dat1);
  lcd_write8(register_dat2);
  lcd_cspulse();
  
}

// то же самое, но из 16-и битной переменной
void lcd_dat(uint16_t data)
{
  lcd_write8(0x76);  // стартовый байт на запись данных
  lcd_write(data);
  lcd_cspulse();
}


// программа инициализации дисплея
void init_lcd(void)
{
	PIO_Configure(pinsLCD, PIO_LISTSIZE(pinsLCD));
	AT91C_BASE_SPI->SPI_MR &= (~AT91C_SPI_PCS);//cs set is 0 channel
	
	UTIL_WaitTimeInMs(BOARD_MCK,50);	
	PIO_Clear(&pinsLCD[1]);	//PIN_LCD_RESET	
	PIO_Clear(&pinsLCD[0]);	//PIN_LCD_RS	RS is not used, set it to high	
	UTIL_WaitTimeInMs(BOARD_MCK,1);
	PIO_Set(&pinsLCD[1]);	//PIN_LCD_RESET 
	UTIL_WaitTimeInMs(BOARD_MCK,10);
	
//	LCD_CS_LOW;
	UTIL_WaitTimeInMs(BOARD_MCK,15);
//  RESET=0;
//  CS=1;
//   UTIL_WaitTimeInMs(BOARD_MCK,10);
//  RESET=1;
//  
//  delay_ms(10);
//  
//  CS=0;
  
 
  lcd_reg(0x00);lcd_dat(0x0001);

  //lcd_reg(0x03);lcd_dat(0x000c);	//medium current
  lcd_reg(0x03);lcd_dat(0x0014);	//large current

  UTIL_WaitTimeInMs(BOARD_MCK,40);
  //lcd_reg(0x0d);lcd_dat(0x0616);
  lcd_reg(0x0d);lcd_dat(0x0016);	//Vci x –1.5

  lcd_reg(0x01);lcd_dat(0x0215);
  
  UTIL_WaitTimeInMs(BOARD_MCK,50);
  lcd_reg(0x07);lcd_dat(0x0037);
	
//	LCD_CS_HIGH;
	lcd_cspulse();
}

// заливка дисплея цветом color
void fill_screen(uint16_t color)
{
	uint16_t j=0, i=0;
//  LCD_CS_LOW;
  lcd_reg(0x16);lcd_dat(0x8300);
  lcd_reg(0x17);lcd_dat(0xAF00);
  lcd_reg(0x21);lcd_dat(0x0000);
  lcd_reg(0x22);lcd_write8(0x76);

  
  // start data transmission  
  for (i=0; i<DISP_H; i++)
	{
		for(j=0;j<DISP_W;j++)
		{
		lcd_write(color);
		}
	}
//	LCD_CS_HIGH;
//	lcd_cspulse();
	 UTIL_WaitTimeInMs(BOARD_MCK,1);

}
//==========================================================================================
//      Процедура заполнение цветом области
//==========================================================================================
// fill_area(10,14,32,16,BLUE); 
// Нарисовать прямоугольник с началом по х=10:у=14 с размером по х=32:у=16 цветом BLUE
void fill_area( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint16_t color)
{
     uint16_t i;
  //   uint16_t w,h;
		
	lcd_reg(0x05);lcd_dat(0x0030);
      lcd_reg(0x16);lcd_dat8(y2,y1);
      lcd_reg(0x17);lcd_dat8(x2,x1);
      lcd_reg(0x21);lcd_dat8(x1,y1);
      lcd_reg(0x22);lcd_write8(0x76);
	  
	/* lock globally interrupt */
    __disable_fiq();
	__disable_irq();
		
	//Пишем точку
//		w=x2-x1+1;
//		h=y2-y1+1;
    // for (i=0;i<w*h;i++) 
	 for (i=0;i<(7598);i++) 
     {
		 lcd_write(color);		 
     };
    /* unlock globally interrupt */
	__enable_fiq();
	__enable_irq();	 
	 
	 UTIL_WaitTimeInMs(BOARD_MCK,50);

}

void do_paint_area(uint16_t color)
{
	 uint16_t i;
   //  uint16_t w,h;
	
	lcd_reg(0x05);lcd_dat(0x0030);
	lcd_reg(0x16);lcd_dat8(DISP_W-1,0);
    lcd_reg(0x17);lcd_dat8(DISP_H-1,58);
	lcd_reg(0x21);lcd_dat8(0x00,0x00);
	lcd_reg(0x22);lcd_write8(0x76);
		/* lock globally interrupt */
    __disable_fiq();
	__disable_irq();
	for (i=0;i<(7598);i++) 
     {
		 lcd_write(color);
	 }
	     /* unlock globally interrupt */
	__enable_fiq();
	__enable_irq();
}

// поместить символ в графическую память дисплея
void put_char(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint8_t rot)
{
  uint8_t h,ch,p,mask;

  //LCD_CS_LOW;
  
  if(!rot)
  {
      lcd_reg(0x05);lcd_dat(0x0038);
      lcd_reg(0x17);lcd_dat8(x+CHAR_W-1, x);
      lcd_reg(0x16);lcd_dat8(y+CHAR_H-1, y);
      lcd_reg(0x21);lcd_dat8(x,y);
      lcd_reg(0x22);
      lcd_write8(0x76);  
  }
  else
  {
      lcd_reg(0x05);lcd_dat(0x0030);
      lcd_reg(0x16);lcd_dat8(x+CHAR_W-1, x);
      lcd_reg(0x17);lcd_dat8(y+CHAR_H-1, y);
      lcd_reg(0x21);lcd_dat8(y, x);
      lcd_reg(0x22);
      lcd_write8(0x76);  
  }

  
  for (h=0; h<CHAR_H; h++) // every column of the character
  {
       if (rot==1)
          ch=ascii_tab[ c-32 ][h];
          else
          ch=ascii_tab[ c-32 ][CHAR_H-h-1];
    
        mask=0x80;
        for (p=0; p<CHAR_W; p++)  // write the pixels
        {
          if (ch&mask)
          {
            lcd_write(color);                
          }
          else
          {
            lcd_write(LCD_backcolor);
          }
          //mask=mask/2;
		   mask=mask>>1;
        }  // for p
  }
//	LCD_CS_HIGH;
  lcd_cspulse();
}


// записать строку, начиная с координат x, y, цветом color и ориентацией orient
// если orient=0, то горизонтально, если =1, то вертикально
void LCD_put_string(uint8_t x, uint8_t y, char *str, uint8_t orient,uint16_t color)
{
	uint16_t j=0;
  //while (j<strlen(str))
  while (str[j]!=0)
  {
    put_char(x+j*CHAR_W,y,str[j], color,orient);   //  0 deg. rotated
    j++;
  }
}

/* ------------------------------------------------------------------------- */



