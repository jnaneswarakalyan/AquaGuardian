#ifndef LCD_H
#define LCD_H
#include "aquaguardian.h"
void WriteLCD(u8 byte);
void CmdLCD(u8 cmd);
void CharLCD(u8 asciiVal);
void StrLCD(s8 *str);
void U32LCD(u32 n);
void InitLCD(void);
void BuildCGRAM(u8 *p, u8 nBytes);
#endif
