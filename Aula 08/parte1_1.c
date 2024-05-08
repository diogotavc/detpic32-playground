#include <detpic32.h>

int main(void)
{
/*
    Configure Timer T3 (2 Hz with interrupts disabled)

    k_prescaler = 20_000_000 / ((2^16 - 1) + 1) * f_out
                = 20_000_000 / 65536 * 2 = 152

    De acordo com o Manual na secção dos Timers:

    111 = 1:256 prescale value  <--- 256 é o valor mais próximo de 152
    110 = 1:64 prescale value
    101 = 1:32 prescale value
    100 = 1:16 prescale value
    011 = 1:8 prescale value
    010 = 1:4 prescale value
    001 = 1:2 prescale value
    000 = 1:1 prescale value

    Logo, TCKPS é 7 (1:256 prescaler)

    f_out = (f_pbclk / k_prescaler) / (PR3 + 1)
    2 = (20_000_000 / 256) / (PR3 + 1)
    PR3 = 78125 / 2 - 1 = 39061.5

    Logo, PR3 será 39062

    Para verificar,
    f_out_prescaler = f_pbclk / 256
    f_out = f_out_prescaler / (PR3 + 1)
    f_out = f_pbclk / (256 * (PR3 + 1))

    f_out = 20_000_000 / (256 * (39061 + 1)) = 2.0000256 Hz
*/

    T3CONbits.TCKPS = 7;
    PR3 = 39061;
    TMR3 = 0;                   // reset timer register T3
    T3CONbits.TON = 1;          // enable timer T3

    while(1)
    {
        while( IFS0bits.T3IF == 0 );    // wait for the interrupt flag (T3IF) to change
        IFS0bits.T3IF = 0;              // reset the interrupt flag
        putChar('.');
    }
    return 0;
}
