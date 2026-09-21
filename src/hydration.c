#include "hydration.h"
#include "delay.h"
#include "lcd.h"
#include "ui.h"

void CheckWaterSwitch(void)
{
    if (((IOPIN0 >> SW_PIN) & 1) == 0)
    {
        delay_ms(20);
        if (((IOPIN0 >> SW_PIN) & 1) == 0)
        {
            if (WaterTaken < TotalGlass)
            {
                WaterTaken++;
                RemainingGlass = TotalGlass - WaterTaken;

                RemainingIntervalSec = IntervalTotalSec;
            }
            RenderDashboardLine2();
            while (((IOPIN0 >> SW_PIN) & 1) == 0);
        }
    }
}
