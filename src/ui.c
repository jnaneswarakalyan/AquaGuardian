#include "ui.h"
#include "lcd.h"

void DisplayCurrentTime(void)
{
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Now: ");
    if (HOUR < 10) CharLCD('0');
    U32LCD(HOUR);
    CharLCD(':');
    if (MIN < 10)  CharLCD('0');
    U32LCD(MIN);
    CharLCD(':');
    if (SEC < 10)  CharLCD('0');
    U32LCD(SEC);
    StrLCD("    ");
}

void DisplayInterval(void)
{
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Now: ");
    if (IntervalHour < 10) CharLCD('0');
    U32LCD(IntervalHour);
    CharLCD(':');
    if (IntervalMin < 10)  CharLCD('0');
    U32LCD(IntervalMin);
    CharLCD(':');
    if (IntervalSec < 10)  CharLCD('0');
    U32LCD(IntervalSec);
    StrLCD("    ");
}

void RenderDashboardLine2(void)
{
    u8 icon_idx;

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("W:");
    if (WaterTaken < 10) CharLCD('0');
    U32LCD(WaterTaken);

    StrLCD(" R:");
    if (RemainingGlass < 10) CharLCD('0');
    U32LCD(RemainingGlass);

    StrLCD(" M:");
    if (MissCount < 10) CharLCD('0');
    U32LCD(MissCount);

    CharLCD(' ');
    if (TotalGlass > 0)
    {
        icon_idx = (WaterTaken * 4) / TotalGlass;
        if (icon_idx > 4) icon_idx = 4;
        CharLCD(icon_idx);
    }
    StrLCD(" ");
}
