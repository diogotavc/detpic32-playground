#include <detpic32.h>

void delay(int ms)
{
    resetCoreTimer();
    while (readCoreTimer() < ms * 20000)
        ;
}

void initComponent(unsigned char component, unsigned char enable)
{
    // char | component | pins
    //  0   |  display  | RB[8,14], RD[5,6]
    //  1   |  LEDs     | RE[0,7]

    // (default state is input)
    //  2   |  adc      | AN[4] (analog RB[4])
    //  3   |  button   | RD[8]
    //  4   |  switches | RB[0,3]
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
        break;
    default:
        break;
    }
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

void counter()
{
    unsigned int rate = 250;
    unsigned int time = 2000 / (stateSwitch() + 1);
    unsigned int refresh = 1000 / (rate * 2);
    unsigned int internalCounter = 0;
    unsigned int counter = 0;

    initComponent(0, 1);
    initComponent(1, 1);
    initComponent(2, 1);
    while (counter < 256)
    {
        internalCounter = 0;
        cmdLED(counter);
        do
        {
            time = 2000 / (stateSwitch() + 1);

            AD1CON1bits.ASAM = 1;
            while (IFS1bits.AD1IF == 0);
            IFS1bits.AD1IF = 0;

            // pint-size bit of debugging
            printInt(ADC1BUF0, 16 | 3 << 16);
            putChar('\r');

            if(ADC1BUF0 > 0)
            {
                initComponent(0, 1);
                rate = (ADC1BUF0/2) + 1;
                refresh = (1024 / (rate * 2)) + 1;
                cmdDisplay(counter);
            } else
            {
                initComponent(0, 0);
            }
            delay(refresh);
        } while (++internalCounter < (time / refresh));
        if (!stateButton())
        {
            counter++;
        }
    }
    initComponent(0, 0);
    initComponent(1, 0);
    initComponent(2, 0);
}

int main(void)
{
    while(1)
    {
        counter();
        while (!stateButton());
    }

    return 0;
}
