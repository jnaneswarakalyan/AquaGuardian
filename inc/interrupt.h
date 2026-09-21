#ifndef INTERRUPT_H
#define INTERRUPT_H
#include "aquaguardian.h"
void InitEINT0(void);
void eint0_isr(void) __irq;
#endif
