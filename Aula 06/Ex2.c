#include <detpic32.h>

int main(void)
{
    TRISBbits.TRISB4 = 1;    // Disable digital output
    AD1PCFGbits.PCFG4 = 0;   // Configure as analog input (AN4)
    AD1CON1bits.SSRC = 7;    // Conversion trigger selection bits
    AD1CON1bits.CLRASAM = 1; // Stop conv. when 1st adc interrupt is generated
    AD1CON3bits.SAMC = 16;   // Sample time is 16 TAD (*100 ns)
    AD1CON2bits.SMPI = 0;    // (1) samples stores in ADC1BUF[0]
    AD1CHSbits.CH0SA = 4;    // Analog channel 4 (AN4)
    AD1CON1bits.ON = 1;      // Enable adc
    TRISDbits.TRISD11 = 0;   // Configure RD11 as digital output
    volatile int aux;
    while (1)
    {
        AD1CON1bits.ASAM = 1;             // Start conversion
        LATDbits.LATD11 = 1;              // Set LATD11 = 1
        while (IFS1bits.AD1IF == 0);      // Wait while conversion not done (AD1IF == 0)
        LATDbits.LATD11 = 0;              // Reset LATD11
        aux = ADC1BUF0;                   // Store conversion result in aux
        IFS1bits.AD1IF = 0;               // Reset AD1IF
    }
    return 0;
}
