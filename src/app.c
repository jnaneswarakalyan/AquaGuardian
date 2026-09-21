#include "app.h"
#include "gpio.h"
#include "interrupt.h"
#include "keypad.h"
#include "lcd.h"
#include "rtc.h"
#include "ui.h"

volatile u8 config_mode = 0;

u8 TotalGlass     = 8;
u8 WaterTaken     = 0;
u8 RemainingGlass = 8;
u8 MissCount      = 0;
u8 goal_done      = 0;
u8 skipped;

#if DEMO_MODE
u32 IntervalHour         = 0;
u32 IntervalMin          = 0;
u32 IntervalSec          = 15;
u32 IntervalTotalSec     = 15;
u32 RemainingIntervalSec = 15;
#else
u32 IntervalHour         = 1;
u32 IntervalMin          = 0;
u32 IntervalSec          = 0;
u32 IntervalTotalSec     = 3600;
u32 RemainingIntervalSec = 3600;
#endif

u8 PreviousSec;
u8 PreviousDay;

s8 week[][4] = {
    "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"
};

u8 Glass[40] = {
    /* 0%   */ 0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x00,
    /* 25%  */ 0x0E, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x00,
    /* 50%  */ 0x0E, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x00,
    /* 75%  */ 0x0E, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00,
    /* 100% */ 0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00
};

void System_Init(void)
{
    Init_KPM();

    InitGPIO();
    InitEINT0();
    InitRTC();
    InitLCD();
    BuildCGRAM(Glass, 40);

    SetStartDateTime();

    PreviousSec = SEC;
    PreviousDay = DOM;

    CmdLCD(CLEAR_LCD);
    RenderDashboardLine2();
}

void ResetDailyState(void)
{
    WaterTaken     = 0;
    RemainingGlass = TotalGlass;
    MissCount      = 0;
    goal_done      = 0;

    RemainingIntervalSec = IntervalTotalSec;
    PreviousSec          = SEC;

    AllOutputsOff();

    CmdLCD(CLEAR_LCD);
    RenderDashboardLine2();
}
