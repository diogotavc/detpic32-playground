#include <detpic32.h>

void delay()
{
    resetCoreTimer();
    while (readCoreTimer() < 2000000);  // 10 Hz delay
}

int main(void)
{
    int cnt1 = 0;
    int cnt5 = 0;
    int cnt10 = 0;
    while (1)
    {
        putChar('\r');
        printInt(cnt1, 10 | 5 << 16);
        putChar('\t');
        printInt(cnt5, 10 | 5 << 16);
        putChar('\t');
        printInt(cnt10, 10 | 5 << 16);
        delay();
        if (cnt10 % 10 == 0)
        {
            cnt1++;
        }
        if (cnt10 % 2 == 0)
        {
            cnt5++;
        }
        cnt10++;
    }
}
