#ifndef KEYPAD_H
#define KEYPAD_H
#include "aquaguardian.h"
void Init_KPM(void);
u32 colscan(void);
u32 rowcheck(void);
u32 colcheck(void);
u32 keyscan(void);
u32 ReadNum(u32 default_val, u8 *skipped_flag);
#endif
