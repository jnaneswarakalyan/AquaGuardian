#ifndef AQUAGUARDIAN_H
#define AQUAGUARDIAN_H

#include <LPC21xx.h>

/* =================================================
 * AquaGuardian - Smart Water Drinking Reminder System
 * Target: LPC2148
 * ================================================= */

/* Set to 1 while testing on the bench so reminders fire every
 * 15 seconds instead of every hour. Set to 0 for the real build. */
#define DEMO_MODE 0

/* ================================================= */
/* TYPE DEFINITIONS                                  */
/* ================================================= */
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef signed char        s8;
typedef signed short       s16;
typedef signed int         s32;
typedef float              f32;

/* ================================================= */
/* BIT / NIBBLE / BYTE MANIPULATION MACROS           */
/* ================================================= */
#define READBIT(WORD, BITPOS)             (((WORD) >> (BITPOS)) & 1)
#define READNIBBLE(WORD, STARTBIT)        (((WORD) >> (STARTBIT)) & 0x0F)

#define WRITEBYTE(WORD, STARTBIT, BYTE) \
    (WORD = (((WORD) & ~((u32)0xFF << (STARTBIT))) | (((u32)(BYTE) & 0xFF) << (STARTBIT))))

#define WRITENIBBLE(WORD, STARTBIT, NIBBLE) \
    (WORD = (((WORD) & ~((u32)0x0F << (STARTBIT))) | (((u32)(NIBBLE) & 0x0F) << (STARTBIT))))

/* ================================================= */
/* HARDWARE PIN MAPPING                              */
/* ================================================= */
#define BUZZER_PIN       0   /* P0.0  - Buzzer Output */
#define SW_PIN          20   /* P0.20 - Manual Water Button (Active Low) */
#define LED_Y_PIN       21   /* P0.21 - Reminder Active Indicator */
#define LED_G_PIN       22   /* P0.22 - Goal Achieved Indicator */
#define LED_R_PIN       23   /* P0.23 - Delinquent / Missed Indicator */

/* LCD on Port 0 */
#define LCD_DATA         8   /* P0.8 to P0.15 (D0 - D7) */
#define LCD_RS           5   /* P0.16 */
#define LCD_RW           6   /* P0.17 */
#define LCD_EN           7   /* P0.18 */

/* Keypad Matrix on Port 1 */
#define ROW0            16   /* P1.16 - Row 0 */
#define ROW1            17   /* P1.17 - Row 1 */
#define ROW2            18   /* P1.18 - Row 2 */
#define ROW3            19   /* P1.19 - Row 3 */
#define COL0            20   /* P1.20 - Col 0 */
#define COL1            21   /* P1.21 - Col 1 */
#define COL2            22   /* P1.22 - Col 2 */
#define COL3            23   /* P1.23 - Col 3 */

/* EINT0 Configuration on P0.1 */
#define EINT0_VIC_CHNO   14
#define CONFIG_SW_PINSEL 0x0000000C  /* P0.1 as EINT0 (PINSEL0 bits 3:2 = 11) */

/* ================================================= */
/* HD44780 LCD COMMAND CONSTANTS                     */
/* ================================================= */
#define CLEAR_LCD        0x01
#define RET_CUR_HOME     0x02
#define SHIFT_CUR_RIGHT  0x06
#define SHIFT_CUR_LEFT   0x07
#define DSP_OFF          0x08
#define DSP_ON_CUR_OFF   0x0C
#define DSP_ON_CUR_ON    0x0E
#define DSP_ON_CUR_BLK   0x0F
#define SHIFT_DSP_LEFT   0x10
#define SHIFT_DSP_RIGHT  0x14
#define MODE_8BIT_2LINE  0x38
#define GOTO_LINE1_POS0  0x80
#define GOTO_LINE2_POS0  0xC0
#define GOTO_CGRAM_START 0x40

/* ================================================= */
/* APPLICATION STATE                                 */
/* ================================================= */
extern volatile u8 config_mode;

extern u8 TotalGlass;
extern u8 WaterTaken;
extern u8 RemainingGlass;
extern u8 MissCount;
extern u8 goal_done;
extern u8 skipped;

extern u32 IntervalHour;
extern u32 IntervalMin;
extern u32 IntervalSec;
extern u32 IntervalTotalSec;
extern u32 RemainingIntervalSec;

extern u8 PreviousSec;
extern u8 PreviousDay;

extern s8 week[][4];
extern u8 Glass[40];

#endif /* AQUAGUARDIAN_H */
