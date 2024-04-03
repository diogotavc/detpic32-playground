#include <detpic32.h>

// Interrupt service routine (interrupt handler)
void _int_ (27) isr_adc(void)   // Vector number 27 as defined in the Family DataSheet (Page 75)
{
    putChar('\r');
    printInt(ADC1BUF0, 16 | 3 << 16);
    IFS1bits.AD1IF = 0;     // Reset AD1IF flag
    AD1CON1bits.ASAM = 1;   // Start A/D conversion
}

int main(void)
{
    TRISBbits.TRISB4 = 1;       // Disable digital output
    AD1PCFGbits.PCFG4 = 0;      // Configure as analog input (AN4)
    AD1CON1bits.SSRC = 7;       // Conversion trigger selection bits
    AD1CON1bits.CLRASAM = 1;    // Stop conv. when 1st adc interrupt is generated
    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (*100 ns)
    AD1CON2bits.SMPI = 0;       // (1) samples stores in ADC1BUF0
    AD1CHSbits.CH0SA = 4;       // Analog channel 4 (AN4)
    AD1CON1bits.ON = 1;         // Enable adc
    IPC6bits.AD1IP = 2;         // Configure the priority of A/D interrupts (0 disabled -> 7 maximum)
    IFS1bits.AD1IF = 0;         // Clear A/D interrupt flag
    IEC1bits.AD1IE = 1;         // Enable A/D interrupts
    EnableInterrupts();         // Globally enable interrupts
    AD1CON1bits.ASAM = 1;       // Start A/D conversion
    while(1);                   // Due to the delay of the conversion, it'll be interrupted mid loop
    return 0;
}
