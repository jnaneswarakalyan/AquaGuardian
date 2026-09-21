#include "rtc.h"

void InitRTC(void)
{
    CCR     = (1 << 1);
    PREINT  = 456;
    PREFRAC = 25024;
    CCR     = (1 << 0);
}

void SetStartDateTime(void)
{
    HOUR  = 17;
    MIN   = 24;
    SEC   = 0;
    DOM   = 31;
    MONTH = 7;
    YEAR  = 2026;
    DOW   = 5;
}
