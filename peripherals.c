#include <detpic32.h>

void delay(int ms)
{
    resetCoreTimer();
    while (readCoreTimer() < ms * 20000)
        ;
}

void initComponents(unsigned char component, unsigned char enable)
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

char stateButton()
{
    unsigned char state = (PORTD & 0x0100) >> 8;
    return !state;
}

int main(void)
{
    unsigned int rate = 250; // 250Hz -> 500Hz per display
    unsigned int time = 50;  // time in ms for each number

    unsigned int refresh = 1000 / (rate * 2);
    unsigned int internalCounter = 0;
    unsigned int counter = 0;

    initComponents(0, 1);
    while (counter < 256)
    {
        internalCounter = 0;
        do
        {
            cmdDisplay(counter);
            delay(refresh);
        } while (++internalCounter < (time / refresh));
        if (!stateButton())
        {
            counter++;
        }
    }
    initComponents(0, 0);
}
