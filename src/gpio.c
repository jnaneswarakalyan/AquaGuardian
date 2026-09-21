#include "gpio.h"

void InitGPIO(void)
{
    IODIR0 &= ~(1 << SW_PIN);
    IODIR0 |= (1 << LED_Y_PIN) | (1 << LED_G_PIN) | (1 << LED_R_PIN) | (1 << BUZZER_PIN);
    AllOutputsOff();
}

void AllOutputsOff(void)
{
    IOCLR0 = (1 << LED_Y_PIN) | (1 << LED_G_PIN) | (1 << LED_R_PIN) | (1 << BUZZER_PIN);
}
