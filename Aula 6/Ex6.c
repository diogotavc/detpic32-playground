#include <detpic32.h>

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

int main(void)
{
    TRISBbits.TRISB4 = 1;    // Disable digital output
    AD1PCFGbits.PCFG4 = 0;   // Configure as analog input (AN4)
    AD1CON1bits.SSRC = 7;    // Conversion trigger selection bits
    AD1CON1bits.CLRASAM = 1; // Stop conv. when 1st adc interrupt is generated
    AD1CON3bits.SAMC = 16;   // Sample time is 16 TAD (*100 ns)
    AD1CON2bits.SMPI = 3;    // (4) samples stores in ADC1BUF[0,3]
    AD1CHSbits.CH0SA = 4;    // Analog channel 4 (AN4)
    AD1CON1bits.ON = 1;      // Enable adc
    TRISB = TRISB & 0x80FF;  // tell displays to output
    TRISD = TRISD & 0xFF9F;  // enable ports for display selection
    int i = 0;
    int average, voltage;
    while (1)
    {
        if(i == 0)
        {
            average = 0;
            voltage = 0;
            AD1CON1bits.ASAM = 1;               // Start conversion
            while (IFS1bits.AD1IF == 0);        // Wait while conversion not done (AD1IF == 0)
            int *p = (int *)(&ADC1BUF0);
            for ( i = 0; i < 4; i++) {
                average += p[i*4];
            }
            average /= 4;
            voltage = (average * 33 + 511) / 1023;
        }
        cmdDisplay(voltage);    // missing conversion from hex to decimal before sending to display
        resetCoreTimer();
        while (readCoreTimer() < 200000);
        i = (i + 1) % 20;
    }
}
