#include <detpic32.h>

volatile int voltage = 0;

void configureAll(void)
{
    // ADC
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 7;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;
    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;

    // Timer T1
    T1CONbits.TCKPS = 2;
    PR1 = 62499;
    TMR1 = 0;
    IEC0bits.T1IE = 1;
    IPC1bits.T1IP = 2;

    // Timer T3
    T3CONbits.TCKPS = 2;
    PR3 = 49999;
    TMR3 = 0;
    IEC0bits.T3IE = 1;
    IPC3bits.T3IP = 2;
    T1CONbits.TON = 1;
    T3CONbits.TON = 1;

    TRISD &= 0xFF9F; // Mark RD6 - RD5 as outputs
    TRISC &= 0xBFFF; // Mark RC14 as output
    TRISB &= 0x80FF; // Mark RB14 - RB8 as outputs
}

void cmdDisplay(unsigned char value)
{
    static const char hex2disp[] = {
        0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
        0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    static char disp = 1; // 1 high, 0 low

    unsigned int digit_hi = value >> 4;    // shift right 4 bits (1 hex digit)
    unsigned int digit_low = value & 0x0F; // ignore 4 bits to the left

    if (disp)
    {
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x80FF) | hex2disp[digit_hi] << 8;
        disp = 0;
    }
    else
    {
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x80FF) | hex2disp[digit_low] << 8;
        disp = 1;
    }
}

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

void _int_(_ADC_VECTOR) isr_adc(void)
{
    int average = 0;
    int *p = (int *)(&ADC1BUF0);
    int i;
    for (i = 0; i < 8; i++) {
        average += p[i*4];
    }
    average /= 8;
    voltage = (average * 33 + 511) / 1023, 10;

    IFS1bits.AD1IF = 0;
    AD1CON1bits.ASAM = 0;
}

void _int_(_TIMER_1_VECTOR) isr_T1(void)
{
    AD1CON1bits.ASAM = 1;
    IFS0bits.T1IF = 0;
}

void _int_(_TIMER_3_VECTOR) isr_T3(void)
{
    cmdDisplay(toBcd(voltage));
    IFS0bits.T3IF = 0;
}

int main(void)
{
    configureAll();

    // Reset interrupt flags
    IFS1bits.AD1IF = 0;
    IFS0bits.T1IF = 0;
    IFS0bits.T3IF = 0;

    EnableInterrupts();

    while (1);
    return 0;
}
