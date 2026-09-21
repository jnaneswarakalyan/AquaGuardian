#include "config.h"
#include "delay.h"
#include "keypad.h"
#include "lcd.h"
#include "ui.h"

void EnterConfigMode(void)
{
    u32 k;
    u32 temp;

    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("CONFIG MODE");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("PRESS 'e' TO SET");

    k = keyscan();

    if (k == 'e')
    {
        CmdLCD(CLEAR_LCD);
        CmdLCD(GOTO_LINE1_POS0);
        StrLCD("1:TIME 2:INT");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("3:TARGET");

        k = keyscan();

        switch (k)
        {
            case '1':
                CmdLCD(CLEAR_LCD);
                CmdLCD(GOTO_LINE1_POS0);
                StrLCD("ENTER HH: ");
                DisplayCurrentTime();
                CmdLCD(GOTO_LINE1_POS0 + 10);
                temp = ReadNum(HOUR, &skipped);
                if (!skipped) HOUR = (temp > 23) ? 23 : temp;

                CmdLCD(CLEAR_LCD);
                CmdLCD(GOTO_LINE1_POS0);
                StrLCD("ENTER MM: ");
                DisplayCurrentTime();
                CmdLCD(GOTO_LINE1_POS0 + 10);
                temp = ReadNum(MIN, &skipped);
                if (!skipped) MIN = (temp > 59) ? 59 : temp;

                CmdLCD(CLEAR_LCD);
                CmdLCD(GOTO_LINE1_POS0);
                StrLCD("ENTER SS: ");
                DisplayCurrentTime();
                CmdLCD(GOTO_LINE1_POS0 + 10);
                temp = ReadNum(SEC, &skipped);
                if (!skipped) SEC = (temp > 59) ? 59 : temp;

                RemainingIntervalSec = IntervalTotalSec;
                PreviousSec = SEC;
                break;

            case '2':
                CmdLCD(CLEAR_LCD);
                CmdLCD(GOTO_LINE1_POS0);
                StrLCD("INT HH: ");
                DisplayInterval();
                CmdLCD(GOTO_LINE1_POS0 + 8);
                temp = ReadNum(IntervalHour, &skipped);
                if (!skipped) IntervalHour = (temp > 23) ? 23 : temp;

                CmdLCD(CLEAR_LCD);
                CmdLCD(GOTO_LINE1_POS0);
                StrLCD("INT MM: ");
                DisplayInterval();
                CmdLCD(GOTO_LINE1_POS0 + 8);
                temp = ReadNum(IntervalMin, &skipped);
                if (!skipped) IntervalMin = (temp > 59) ? 59 : temp;

                CmdLCD(CLEAR_LCD);
                CmdLCD(GOTO_LINE1_POS0);
                StrLCD("INT SS: ");
                DisplayInterval();
                CmdLCD(GOTO_LINE1_POS0 + 8);
                temp = ReadNum(IntervalSec, &skipped);
                if (!skipped) IntervalSec = (temp > 59) ? 59 : temp;

                IntervalTotalSec = ((u32)IntervalHour * 3600UL) +
                                   ((u32)IntervalMin * 60UL) +
                                   (u32)IntervalSec;

                if (IntervalTotalSec == 0)
                {
                    IntervalHour = 0;
                    IntervalMin  = 0;
                    IntervalSec  = 15;
                    IntervalTotalSec = 15;
                }

                RemainingIntervalSec = IntervalTotalSec;
                PreviousSec = SEC;
                break;

            case '3':
                CmdLCD(CLEAR_LCD);
                CmdLCD(GOTO_LINE1_POS0);
                StrLCD("TARGET: ");
                CmdLCD(GOTO_LINE2_POS0);
                StrLCD("OLD: ");
                U32LCD(TotalGlass);
                CmdLCD(GOTO_LINE1_POS0 + 8);

                temp = ReadNum(TotalGlass, &skipped);
                if (!skipped)
                {
                    if (temp == 0) temp = 1;
                    if (temp > 20) temp = 20;
                    TotalGlass = (u8)temp;

                    WaterTaken     = 0;
                    RemainingGlass = TotalGlass;
                    MissCount      = 0;
                    goal_done      = 0;
                }
                break;

            default:
                CmdLCD(CLEAR_LCD);
                CmdLCD(GOTO_LINE1_POS0);
                StrLCD("INVALID CHOICE");
                delay_s(1);
                break;
        }
    }
}
