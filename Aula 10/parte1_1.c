#include <detpic32.h>

int main(void)
{
/*  Configure UART2:
    1 - Configure BaudRate Generator
    2 - Configure number of data bits, parity and number of stop bits
        (see U2MODE register)
    3 - Enable the transmitter and receiver modules (see register U2STA)
    4 - Enable UART2 (see register U2MODE)

    baudrate = 115200 = 20000000/16*(U2BRRG + 1)
    logo,
    U2BRG = ((20000000/115200)/16) - 1 = 10
*/

    U2BRG = 10;
    U2MODEbits.BRGH = 0;

    U2MODEbits.PDSEL = 00;
    U2MODEbits.STSEL = 0;

    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;
    U2MODEbits.ON = 1;

    while(1);
    return 0;
}
