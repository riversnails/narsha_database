/* LCD */
const char ascii_8x16[95][16] = {  //8x16 ascii font, each character takes16 bytes, total 1520 bytes, draw from top-left to top-right direction.
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //0x20 space
  {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00},  //0x21 !
  {0x36, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //0x22 "
  {0x00, 0x24, 0x24, 0x24, 0x24, 0x7e, 0x7e, 0x24, 0x24, 0x7e, 0x7e, 0x24, 0x24, 0x24, 0x00, 0x00},  //0x23 #
  {0x08, 0x3e, 0x69, 0x69, 0x69, 0x68, 0x3c, 0x0e, 0x0b, 0x4b, 0x4b, 0x4b, 0x3e, 0x08, 0x00, 0x00},  //0x24 $
  {0x00, 0x00, 0x62, 0x96, 0x94, 0x9c, 0x68, 0x18, 0x10, 0x36, 0x29, 0x69, 0x49, 0x46, 0x00, 0x00},  //0x25 %
  {0x00, 0x00, 0x38, 0x6c, 0x6c, 0x6c, 0x38, 0x73, 0xdb, 0xdb, 0xce, 0xce, 0xce, 0x7b, 0x00, 0x00},  //0x26 &
  {0x0c, 0x0c, 0x0c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //0x27 '
  {0x0c, 0x18, 0x18, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x18, 0x18, 0x0c},  //0x28 (
  {0x30, 0x18, 0x18, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x18, 0x18, 0x30},  //0x29 )
  {0x00, 0x00, 0x00, 0x63, 0x63, 0x36, 0x1c, 0x7f, 0x1c, 0x36, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00},  //0x2a *
  {0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},  //0x2b +
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x18},  //0x2c ,
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //0x2d -
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00},  //0x2e .
  {0x00, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x18, 0x10, 0x30, 0x20, 0x60, 0x40, 0xc0, 0x80, 0x00},  //0x2f /
  {0x00, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x66, 0x3c, 0x00, 0x00},  //0x30 0
  {0x00, 0x18, 0x18, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00},  //0x31 1
  {0x00, 0x3c, 0x66, 0xc3, 0xc3, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0xc0, 0xff, 0x00, 0x00},  //0x32 2
  {0x00, 0x3c, 0x66, 0xc3, 0x03, 0x06, 0x3c, 0x06, 0x03, 0x03, 0x03, 0xc3, 0x66, 0x3c, 0x00, 0x00},  //0x33 3
  {0x00, 0x0e, 0x0e, 0x1e, 0x1e, 0x36, 0x36, 0x66, 0x66, 0xc6, 0xff, 0x06, 0x06, 0x06, 0x00, 0x00},  //0x34 4
  {0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xfc, 0xe6, 0xc3, 0x03, 0x03, 0x03, 0xc3, 0x66, 0x3c, 0x00, 0x00},  //0x35 5
  {0x00, 0x3c, 0x66, 0xc3, 0xc0, 0xc0, 0xfc, 0xe6, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x00, 0x00},  //0x36 6
  {0x00, 0xff, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x18, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00},  //0x37 7
  {0x00, 0x3c, 0x66, 0xc3, 0xc3, 0x66, 0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x00, 0x00},  //0x38 8
  {0x00, 0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0x67, 0x3f, 0x03, 0x03, 0xc3, 0x66, 0x3c, 0x00, 0x00},  //0x39 9
  {0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},  //0x3a :
  {0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00, 0x00},  //0x3b ;
  {0x00, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x00, 0x00},  //0x3c <
  {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //0x3d =
  {0x00, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x00, 0x00},  //0x3e >
  {0x00, 0x3c, 0x66, 0xc3, 0xc3, 0x03, 0x06, 0x0c, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00},  //0x3f ?
  {0x00, 0x00, 0x7e, 0x87, 0x87, 0x03, 0x73, 0xfb, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b, 0x7e, 0x00, 0x00},  //0x40 @
  {0x00, 0x18, 0x3c, 0x66, 0x43, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00},  //0x41 A
  {0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00},  //0x42 B
  {0x00, 0x7e, 0xc3, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc3, 0xc3, 0x7e, 0x00, 0x00},  //0x43 C
  {0x00, 0xfc, 0xc6, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc6, 0xfc, 0x00, 0x00},  //0x44 D
  {0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0x00, 0x00},  //0x45 E
  {0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00},  //0x46 F
  {0x00, 0x7e, 0xe7, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xc7, 0xc3, 0xc3, 0xc3, 0xe7, 0x7d, 0x00, 0x00},  //0x47 G
  {0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00},  //0x48 H
  {0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00},  //0x49 I
  {0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xc3, 0xc3, 0x66, 0x3c, 0x00, 0x00},  //0x4a J
  {0x00, 0xc1, 0xc3, 0xc6, 0xcc, 0xc8, 0xd8, 0xf0, 0xf8, 0xec, 0xc4, 0xc6, 0xc3, 0xc1, 0x00, 0x00},  //0x4b K
  {0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0x00, 0x00},  //0x4c L
  {0x00, 0xc3, 0xc3, 0xc3, 0xe7, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00},  //0x4d M
  {0x00, 0xc3, 0xc3, 0xe3, 0xe3, 0xf3, 0xd3, 0xdb, 0xcb, 0xcf, 0xc7, 0xc7, 0xc3, 0xc3, 0x00, 0x00},  //0x4e N
  {0x00, 0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x00, 0x00},  //0x4f O
  {0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00},  //0x50 P
  {0x00, 0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xdf, 0xce, 0xc7, 0x7f, 0x00, 0x00},  //0x51 Q
  {0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc3, 0xc7, 0xfe, 0xcc, 0xc6, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00},  //0x52 R
  {0x00, 0x7e, 0xe7, 0xc1, 0xc0, 0xc0, 0xe0, 0x7e, 0x07, 0x03, 0x03, 0x03, 0xe7, 0x7e, 0x00, 0x00},  //0x53 S
  {0x00, 0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00},  //0x54 T
  {0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x00, 0x00},  //0x55 U
  {0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18, 0x00, 0x00},  //0x56 V
  {0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xdb, 0xdb, 0xff, 0x66, 0x66, 0x66, 0x00, 0x00},  //0x57 W
  {0x00, 0xc3, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00},  //0x58 X
  {0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00},  //0x59 Y
  {0x00, 0xff, 0xff, 0x03, 0x06, 0x04, 0x0c, 0x18, 0x10, 0x30, 0x60, 0xc0, 0xff, 0xff, 0x00, 0x00},  //0x5a Z
  {0x3e, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3e},  //0x5b [
  {0x00, 0xc0, 0x40, 0x60, 0x20, 0x30, 0x10, 0x18, 0x08, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x01, 0x00},  /*0x5c \*/
  {0x7c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c},  //0x5d ]
  {0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //0x5e ^
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff},  //0x5f _
  {0x30, 0x18, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //0x60 `
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00},  //0x61 a
  {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00},  //0x62 b
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00},  //0x63 c
  {0x00, 0x00, 0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00},  //0x64 d
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xc3, 0xc3, 0xc3, 0xff, 0xc0, 0xc0, 0xc1, 0x7e, 0x00, 0x00},  //0x65 e
  {0x00, 0x00, 0x1e, 0x30, 0x30, 0x7e, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00},  //0x66 f
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e},  //0x67 g
  {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00},  //0x68 h
  {0x00, 0x00, 0x0c, 0x0c, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00},  //0x69 i
  {0x00, 0x00, 0x0c, 0x0c, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x38},  //0x6a j
  {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc3, 0xc6, 0xcc, 0xd8, 0xf8, 0xec, 0xc6, 0xc3, 0xc1, 0x00, 0x00},  //0x6b k
  {0x00, 0x00, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00, 0x00},  //0x6c l
  {0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0xdb, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00},  //0x6d m
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x00, 0x00},  //0x6e n
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00},  //0x6f o
  {0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0},  //0x70 p
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03},  //0x71 q
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x37, 0x3c, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00},  //0x72 r
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0x7e, 0x03, 0x03, 0xc3, 0x7e, 0x00, 0x00},  //0x73 s
  {0x00, 0x00, 0x18, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x0e, 0x00, 0x00},  //0x74 t
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3f, 0x00, 0x00},  //0x75 u
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x36, 0x1c, 0x08, 0x00, 0x00},  //0x76 v
  {0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xdb, 0xdb, 0xdb, 0x7e, 0x66, 0x24, 0x00, 0x00},  //0x77 w
  {0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0xc3, 0x66, 0x3c, 0x3c, 0x66, 0xe3, 0xc3, 0xc3, 0x00, 0x00},  //0x78 x
  {0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x7e},  //0x79 y
  {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0xff, 0x00, 0x00},  //0x7a z
  {0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x70, 0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18},  //0x7b {
  {0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00},  //0x7c |
  {0x38, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0e, 0x0e, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x08, 0x38},  //0x7d }
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0xff, 0xce, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}   //0x7e ~
};
