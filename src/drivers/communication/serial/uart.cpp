#include "drivers/communication/serial/uart.h"

#include <M480.h>
#include <uart.h>

namespace communication::serial
{

void Uart::initialize()
{
    CLK_EnableModuleClock(UART0_MODULE);

    /* Set UART0 module clock source (HXT) and divider (1) */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));

    /* Setup PB.12 and PB.13 as multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk       | SYS_GPB_MFPH_PB13MFP_Msk);
    SYS->GPB_MFPH |=  (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    /* Open UART0 connection at 115200 baudrate */
    UART_Open(UART0, 115200);
}

void Uart::putChar(const char c) const
{
    while (UART_IS_TX_FULL(UART0));
    UART_WRITE(UART0, c);
}

}  // namespace x8k::communication::serial