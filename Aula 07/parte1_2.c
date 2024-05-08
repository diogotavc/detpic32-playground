#include <detpic32.h>

volatile unsigned char voltage = 0;

// Interrupt service routine (interrupt handler)
void _int_ (27) isr_adc(void)   // Vector number 27 as defined in the Family DataSheet (Page 75)
{
    int *p = (int *)(&ADC1BUF0);
    for ( i = 0; i < 4; i++) {
        average += p[i*4];
    }
    average /= 4;
    voltage = (average * 33 + 511) / 1023;  // Calculate the average values
    IFS1bits.AD1IF = 0;                     // Reset AD1IF flag
}

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

int main(void)
{
    TRISB &= 0x807F;            // Prepare DISP as outputs
    TRISD &= 0xFFCF;            // Prepare CNTL_DISP as outputs
    TRISBbits.TRISB4 = 1;       // Disable digital output
    AD1PCFGbits.PCFG4 = 0;      // Configure as analog input (AN4)
    AD1CON1bits.SSRC = 7;       // Conversion trigger selection bits
    AD1CON1bits.CLRASAM = 1;    // Stop conv. when 1st adc interrupt is generated
    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (*100 ns)
    AD1CON2bits.SMPI = 7;       // (8) samples stores in ADC1BUF0[0,7]
    AD1CHSbits.CH0SA = 4;       // Analog channel 4 (AN4)
    AD1CON1bits.ON = 1;         // Enable adc
    IPC6bits.AD1IP = 2;         // Configure the priority of A/D interrupts (0 disabled -> 7 maximum)
    IFS1bits.AD1IF = 0;         // Clear A/D interrupt flag
    IEC1bits.AD1IE = 1;         // Enable A/D interrupts
    EnableInterrupts();         // Globally enable interrupts
    while(1)
    {
        if (cnt == 0)
        {
            AD1CON1bits.ASAM = 1;   // Start A/D conversion
        }
        // Send voltage value to displays
        resetCoreTimer();
        while (readCoreTimer() < 200000);
        cnt = (cnt + 1) % 20;
    }
    return 0;
}
