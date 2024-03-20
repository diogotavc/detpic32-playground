#include <detpic32.h>

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
    int i, average, voltage;
    while (1)
    {
        i = 0;
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
        printInt(voltage, 10);
        putChar('\n');
        IFS1bits.AD1IF = 0;                 // Reset AD1IF
    }
    return 0;
}
