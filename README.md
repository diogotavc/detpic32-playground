# detpic32-playground

This is a very empty repository for testing and playing around with the detpic32.

> [!NOTE]
> This repository can be useful for solving [AC2](https://www.ua.pt/pt/uc/15184) guides, so I've decided to also put solved exercises here.

## Programs

### [peripherals_counters](./peripherals_counter.c)

Simple counter from 0x00 to 0xFF.
- Seven segment displays and LEDs display the numbers
- Switches control the speed (from 2 seconds to 2/15 seconds)
- Button halts the counter and restarts it, if it's reached 0xFF
- Potentiometer controls the brightness of the seven segment displays

## Utils

### [detpic32_utils](./detpic32_utils.c)

Basic functions for interfacing with the peripherals.
- delay - Waits for the specific milliseconds
- delayAsync - Same as before, but without halting the program
- initComponent - Initializes (or deactivates) the specific component
- cmdDisplay - Displays any number, from 0x00 to 0xFF
- cmdLED - Displays any number, from 0000 0000 to 1111 1111
- stateButton() - Check the state of the button (0 or 1)
- stateSwitch() - Check the state of the switches (from 0000 to 1111)