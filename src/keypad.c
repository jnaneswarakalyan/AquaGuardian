#include "keypad.h"
#include "lcd.h"

/* Layout:
   'C' = Backspace, 'e' = Enter/Confirm, 'd' = Skip/Escape */
const u8 KPMLUT[4][4] = {
    {'1', '2', '3', 'a'},
    {'4', '5', '6', 'b'},
    {'7', '8', '9', 'c'},
    {'C', '0', 'e', 'd'}
};

void Init_KPM(void)
{
    WRITENIBBLE(IODIR1, ROW0, 15);
    WRITENIBBLE(IOPIN1, ROW0, 0x00);
}

u32 colscan(void)
{
    return (READNIBBLE(IOPIN1, COL0) < 15) ? 0 : 1;
}

u32 rowcheck(void)
{
    u32 rno;
    u32 found = 0xFF;

    for (rno = 0; rno < 4; rno++)
    {
        WRITENIBBLE(IOPIN1, ROW0, (~(1 << rno)));
        if (colscan() == 0)
        {
            found = rno;
            break;
        }
    }
    WRITENIBBLE(IOPIN1, ROW0, 0x00);
    return found;
}

u32 colcheck(void)
{
    u32 cno;

    for (cno = 0; cno < 4; cno++)
    {
        if (READBIT(IOPIN1, (cno + COL0)) == 0)
            return cno;
    }
    return 0xFF;
}

u32 keyscan(void)
{
    u32 rno, cno, key;

    WRITENIBBLE(IOPIN1, ROW0, 0x00);
    while (colscan());

    rno = rowcheck();
    cno = colcheck();

    if (rno > 3 || cno > 3)
    {
        while (!colscan());
        return 0;
    }

    key = KPMLUT[rno][cno];

    WRITENIBBLE(IOPIN1, ROW0, 0x00);
    while (!colscan());

    return key;
}

u32 ReadNum(u32 default_val, u8 *skipped_flag)
{
    u32 num = 0;
    u8 digits = 0;
    u32 key;

    *skipped_flag = 0;

    while (1)
    {
        key = keyscan();

        if (key >= '0' && key <= '9')
        {
            if (digits < 5)
            {
                num = (num * 10) + (key - '0');
                digits++;
                CharLCD((u8)key);
            }
        }
        else if (key == 'C')
        {
            if (digits > 0)
            {
                num /= 10;
                digits--;
                CmdLCD(SHIFT_DSP_LEFT);
                CharLCD(' ');
                CmdLCD(SHIFT_DSP_LEFT);
            }
        }
        else if (key == 'e')
        {
            if (digits == 0)
            {
                *skipped_flag = 1;
                return default_val;
            }
            return num;
        }
        else if (key == 'd')
        {
            *skipped_flag = 1;
            return default_val;
        }
    }
}
