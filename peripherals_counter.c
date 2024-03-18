#include <detpic32.h>
#include "detpic32_utils.c"

void counter(unsigned int maxTime)
{
    unsigned int refresh = 1000 / (250 * 2);
    unsigned int internalCounter = 0;
    unsigned int counter = 0;
    unsigned int time = maxTime / (stateSwitch() + 1);

    while (counter < 256)
    {
        internalCounter = 0;
        cmdLED(counter);
        do
        {
            time = maxTime / (stateSwitch() + 1);
            cmdDisplay(counter,100);
        } while (++internalCounter < (time / refresh));
        if (!stateButton())
        {
            counter++;
        }
    }
}

int main(void)
{
    unsigned int time = 2000; // highest time in ms for each number (ranges from time to (time/16) ms)

    while (1)
    {
        initComponent(0, 1);
        initComponent(1, 1);
        initComponent(2, 1);
        counter(time);
        initComponent(0, 0);
        initComponent(1, 0);
        initComponent(2, 0);
        while (!stateButton());
    }

    return 0;
}
