#include "reminder.h"
#include "delay.h"
#include "lcd.h"

void Reminder(void)
{
    u32 i;
    u8 drank = 0;

    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("DRINK WATER NOW!");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("PRESS SWITCH");

    IOSET0 = (1 << BUZZER_PIN) | (1 << LED_Y_PIN);

    /* 5-second alert window */
    for (i = 0; i < 500; i++)
    {
        if (((IOPIN0 >> SW_PIN) & 1) == 0)
        {
            delay_ms(20);
            if (((IOPIN0 >> SW_PIN) & 1) == 0)
            {
                drank = 1;
                if (WaterTaken < TotalGlass)
                {
                    WaterTaken++;
                    RemainingGlass = TotalGlass - WaterTaken;
                }
                while (((IOPIN0 >> SW_PIN) & 1) == 0);
                break;
            }
        }
        delay_ms(10);
    }

    IOCLR0 = (1 << BUZZER_PIN) | (1 << LED_Y_PIN);

    if (drank)
    {
        CmdLCD(CLEAR_LCD);
        CmdLCD(GOTO_LINE1_POS0);
        StrLCD("GOOD JOB!");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("WATER LOGGED");

        IOSET0 = (1 << LED_G_PIN);
        delay_s(2);
        IOCLR0 = (1 << LED_G_PIN);
    }
    else
    {
        MissCount++;

        CmdLCD(CLEAR_LCD);
        CmdLCD(GOTO_LINE1_POS0);
        StrLCD("REMINDER MISSED!");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("MISS COUNT: ");
        if (MissCount < 10) CharLCD('0');
        U32LCD(MissCount);

        IOSET0 = (1 << LED_R_PIN) | (1 << BUZZER_PIN);
        delay_s(3);
        IOCLR0 = (1 << LED_R_PIN) | (1 << BUZZER_PIN);
    }

    CmdLCD(CLEAR_LCD);
}
