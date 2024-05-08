#include <detpic32.h>

int main(void)
{
    TRISCbits.TRISC14 = 0;
    while (1)
    {
        resetCoreTimer();
        while (readCoreTimer() < 10000000);
        LATCbits.LATC14 = !LATCbits.LATC14;
    }
    return 0;
}
