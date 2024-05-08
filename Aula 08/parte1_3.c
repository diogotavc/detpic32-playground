#include <detpic32.h>

void _int_(12) isr_T3(void)     // vector number 12 as defined in the Family DataSheet (Page 75)
{
    static int count = 0;
    count = (count + 1) % 2;

    if (count == 0)
        putChar('.');

    IFS0bits.T3IF = 0;
}

int main(void)
{
    T3CONbits.TCKPS = 7;
    PR3 = 39061;
    TMR3 = 0;                   // reset timer register T3
    T3CONbits.TON = 1;          // enable timer T3

    IPC3bits.T3IP = 2;          // set timer T3 interrupt priority to 2 (range [1..6])
    IEC0bits.T3IE = 1;          // enable timer T3 interrupts

    EnableInterrupts();
    while (1);
    return 0;
}
