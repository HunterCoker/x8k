#include "uart_init.h"

#include <NuMicro.h>

#include <assert.h>
#include <stdbool.h>

static bool s_uartInitialized = false;

void UART_Init(void)
{
#ifdef DEBUG
    CLK_EnableModuleClock(UART0_MODULE);

    /* Set UART0 module clock source (HXT) and divider (1) */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));

    /* Setup PB.12 and PB.13 as multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk);
    SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    /* Open UART0 connection at 115200 baudrate */
    UART_Open(UART0, 115200);

    s_uartInitialized = true;
#endif
}

void uart_put_char(const char c)
{
#ifdef DEBUG
    assert(s_uartInitialized == true);

    while (UART_IS_TX_FULL(UART0));
    UART_WRITE(UART0, c);
#else
#   error "UART only available in debug configuration."
#endif
}
