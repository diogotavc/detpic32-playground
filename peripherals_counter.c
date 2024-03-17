#include <detpic32.h>
#include "detpic32_utils.c"

void counter(unsigned int rate, unsigned int maxTime)
{
    unsigned int refresh = 1000 / (rate * 2);
    unsigned int internalCounter = 0;
    unsigned int counter = 0;
    unsigned int time = maxTime / (stateSwitch() + 1);

    initComponent(0, 1);
    initComponent(1, 1);
    while (counter < 256)
    {
        internalCounter = 0;
        cmdLED(counter);
        do
        {
            time = maxTime / (stateSwitch() + 1);
            cmdDisplay(counter);
            delay(refresh);
        } while (++internalCounter < (time / refresh));
        if (!stateButton())
        {
            counter++;
        }
    }
    initComponent(0, 0);
    initComponent(1, 0);
}

int main(void)
{
    unsigned int rate = 250;  // 250Hz -> 500Hz per display
    unsigned int time = 2000; // highest time in ms for each number (ranges from time to (time/16) ms)

    while (1)
    {
        counter(rate, time);
        while (!stateButton());
    }

    return 0;
}
