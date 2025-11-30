#include "uart_init.h"

#include <stdio.h>

static void delay()
{
    for (volatile uint32_t i = 0; i < 12000000; i++);
}

static void SYS_Init(void)
{
    /* HXT clock setup */
    {
        /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
        GPIO_SetMode(PF, BIT2 | BIT3, GPIO_MODE_INPUT);

        /* Enable HXT clock (external XTAL 12MHz) */
        CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

        /* Wait for HXT clock ready */
        CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

        /* Set MCU core clock to its maximum frequency of 192MHz */
        CLK_SetCoreClock(192000000);
        CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV2;
    }
    
    UART_Init();
}

int main()
{
    SYS_UnlockReg();
    SYS_Init();
    SYS_LockReg();

    GPIO_SetMode(PH, BIT1, GPIO_MODE_OUTPUT);

    while (1)
    {
        PH1 ^= BIT0;
        printf("hello\n");
        delay();
    }

    UART_Close(UART0);

    return 0;
}