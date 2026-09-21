#include "lcd.h"
#include "delay.h"

void WriteLCD(u8 byte)
{
    WRITEBYTE(IOPIN0, LCD_DATA, byte);
    IOCLR0 = (1 << LCD_RW);
    IOSET0 = (1 << LCD_EN);
    delay_us(1);
    IOCLR0 = (1 << LCD_EN);
    delay_ms(2);
}

void CmdLCD(u8 cmd)
{
    IOCLR0 = (1 << LCD_RS);
    WriteLCD(cmd);
}

void CharLCD(u8 asciiVal)
{
    IOSET0 = (1 << LCD_RS);
    WriteLCD(asciiVal);
}

void StrLCD(s8 *str)
{
    while (*str)
        CharLCD(*str++);
}

void U32LCD(u32 n)
{
    u8 a[10];
    s32 i = 0;
    if (n == 0)
    {
        CharLCD('0');
    }
    else
    {
        while (n > 0)
        {
            a[i] = (n % 10) + '0';
            n /= 10;
            i++;
        }
        for (--i; i >= 0; i--)
            CharLCD(a[i]);
    }
}

void InitLCD(void)
{
    IODIR0 |= ((0xFF << LCD_DATA) | (1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN));

    delay_ms(15);
    CmdLCD(0x30);
    delay_ms(4);
    delay_us(100);
    CmdLCD(0x30);
    delay_us(100);
    CmdLCD(0x30);
    CmdLCD(MODE_8BIT_2LINE);
    CmdLCD(DSP_ON_CUR_OFF);
    CmdLCD(CLEAR_LCD);
    CmdLCD(SHIFT_CUR_RIGHT);
}

void BuildCGRAM(u8 *p, u8 nBytes)
{
    u8 i;
    CmdLCD(GOTO_CGRAM_START);
    IOSET0 = (1 << LCD_RS);
    for (i = 0; i < nBytes; i++)
    {
        WriteLCD(p[i]);
    }
    CmdLCD(GOTO_LINE1_POS0);
}
