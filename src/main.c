#include "app.h"
#include "config.h"
#include "delay.h"
#include "gpio.h"
#include "hydration.h"
#include "interrupt.h"
#include "lcd.h"
#include "reminder.h"
#include "rtc.h"
#include "ui.h"

int main(void)
{
    /* 1. System initialization */
    System_Init();

    while (1)
    {
        /* --------------------------------------------- */
        /* AUTOMATIC MIDNIGHT / NEW DAY RESET            */
        /* --------------------------------------------- */
        if (DOM != PreviousDay)
        {
            PreviousDay = DOM;
            ResetDailyState();
        }

        /* --------------------------------------------- */
        /* MANUAL DRINK BUTTON (ACTIVE ANYTIME)          */
        /* --------------------------------------------- */
        if (goal_done == 0)
        {
            CheckWaterSwitch();
        }

        /* --------------------------------------------- */
        /* 1-SECOND TIME BASE & DASHBOARD UPDATE         */
        /* --------------------------------------------- */
        if (SEC != PreviousSec)
        {
            PreviousSec = SEC;

            /* Line 1: Digital Clock */
            CmdLCD(GOTO_LINE1_POS0);
            if (HOUR < 10) CharLCD('0');
            U32LCD(HOUR);
            CharLCD(':');
            if (MIN < 10)  CharLCD('0');
            U32LCD(MIN);
            CharLCD(':');
            if (SEC < 10)  CharLCD('0');
            U32LCD(SEC);

            CmdLCD(GOTO_LINE1_POS0 + 9);
            StrLCD(week[DOW]);
            StrLCD("  ");

            /* Countdown and reminders */
            if (goal_done == 0)
            {
                if (RemainingIntervalSec > 0)
                {
                    RemainingIntervalSec--;
                }

                if (RemainingIntervalSec == 0)
                {
                    Reminder();

                    /* Resync time base after alert finishes */
                    RemainingIntervalSec = IntervalTotalSec;
                    PreviousSec = SEC;

                    CmdLCD(CLEAR_LCD);
                }

                RenderDashboardLine2();
            }
        }

        /* --------------------------------------------- */
        /* GOAL COMPLETION                               */
        /* --------------------------------------------- */
        if ((WaterTaken >= TotalGlass) && (goal_done == 0))
        {
            goal_done = 1;

            CmdLCD(CLEAR_LCD);
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("GOAL COMPLETED!");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("100% DONE!");

            IOSET0 = (1 << LED_G_PIN) | (1 << BUZZER_PIN);
            delay_s(3);
            IOCLR0 = (1 << LED_G_PIN) | (1 << BUZZER_PIN);

            CmdLCD(CLEAR_LCD);
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("REST FOR TODAY  ");
        }

        /* --------------------------------------------- */
        /* EINT0 CONFIGURATION TRIGGER                   */
        /* --------------------------------------------- */
        if (config_mode)
        {
            IOCLR0 = (1 << BUZZER_PIN) | (1 << LED_Y_PIN);
            EnterConfigMode();
            config_mode = 0;

            CmdLCD(CLEAR_LCD);
            PreviousSec = SEC;
            RenderDashboardLine2();
        }

        delay_ms(20);
    }
}
