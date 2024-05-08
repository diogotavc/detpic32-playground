#include <detpic32.h>

void _int_(4) isr_T1(void)     // vector number 4 as defined in the Family DataSheet (Page 74)
{
    putChar('1');
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)     // vector number 12 as defined in the Family DataSheet (Page 74)
{
    putChar('3');
    IFS0bits.T3IF = 0;
}

int main(void)
{
    // configure timer T1 and T3
    T3CONbits.TCKPS =;
    PR3 = ;
    T1CONbits.TCKPS =;
    PR1 = ;

    // enable timer T1 and T3
    TMR1 = 0;
    T1CONbits.TON = 1;
    TMR3 = 0;
    T3CONbits.TON = 1;

    // configure interrupts for T1 and T3
    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;

    EnableInterrupts();
    while (1);
    return 0;
}
