#include <detpic32.h>

void delay(int ms)
{
    resetCoreTimer();
    while (readCoreTimer() < ms * 20000);
}

int delayAsync(int* internalCounter, int ms)
{
    if(*internalCounter < ms){
        (*internalCounter)++;
        return 0;
    } else
    {
        *internalCounter = 0;
        return 1;
    }
}

void initComponent(unsigned char component, unsigned char enable)
{
    // char | component | pins
    //  0   |  display  | RB[8,14], RD[5,6]
    //  1   |  LEDs     | RE[0,7]

    // (default state is input)
    //  2   |  button   | RD[8]
    //  3   |  switches | RB[0,3]
    //  4   |  adc      | ???
    switch (component)
    {
    case 0:
        if (enable)
        {
            TRISB = TRISB & 0x80FF; // tell displays to output
            TRISD = TRISD & 0xFF9F; // enable ports for display selection
        }
        else
        {
            LATD = LATD & 0xFF9F;   // disable displays
            TRISB = TRISB | 0x8000; // reset displays to default state
            TRISD = TRISD | 0x0060; // reset ports for display selection
        }
        break;
    case 1:
        if (enable)
        {
            TRISE = TRISE & 0xFF00; // tell LEDs to output
            LATE = LATE & 0xFF00;   // reset all LEDs
        }
        else
        {
            LATE = LATE & 0xFF00;   // reset all LEDs
            TRISE = TRISE | 0x00FF; // reset LEDs to default state
        }
        break;
    case 2:
        if (enable)
        {
            TRISBbits.TRISB4 = 1;       // disable digital output
            AD1PCFGbits.PCFG4 = 0;      // configure as analog input (AN4)
            AD1CON1bits.SSRC = 7;       // conversion trigger selection bits
            AD1CON1bits.CLRASAM = 1;    // stop conv. when 1st adc interrupt is generated
            AD1CON3bits.SAMC = 16;      // sample time is 16 TAD (*100 ns)
            AD1CON2bits.SMPI = 3;       // (4) samples stores in ADC1BUF[0,3]
            AD1CHSbits.CH0SA = 4;       // analog channel 4 (AN4)
            AD1CON1bits.ON = 1;         // enable adc
        }
        else
        {
            TRISBbits.TRISB4 = 1;
            AD1PCFGbits.PCFG4 = 1;
            AD1CON1bits.SSRC = 0;
            AD1CON1bits.CLRASAM = 0;
            AD1CON3bits.SAMC = 0;
            AD1CON2bits.SMPI = 0;
            AD1CHSbits.CH0SA = 0;
            AD1CON1bits.ON = 0;
        }
    default:
        break;
    }
}

void cmdDisplay(unsigned char value, unsigned char brightness)
{
    static const int rate = 40000;  // rate = (1000 * 20_000) / (refresh * 2)

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
    resetCoreTimer();
    while (readCoreTimer() < rate)
    {
        if (readCoreTimer() > (rate/100)*brightness) {
            LATB = (LATB & 0x80FF);
        }
    }
}

void cmdLED(unsigned char value)
{
    LATE = LATE & 0xFF00;
    LATE = LATE | (value & 0x01) | (value & 0x02) | (value & 0x04) | (value & 0x08);
    LATE = LATE | (value & 0x10) | (value & 0x20) | (value & 0x40) | (value & 0x80);
}

char stateButton()
{
    return !((PORTD & 0x0100) >> 8);
}

char stateSwitch()
{
    return (PORTB & 0x000F);
}
