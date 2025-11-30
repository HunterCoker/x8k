#include <NuMicro.h>

#include <stdio.h>

extern "C" void ProcessHardFault(uint32_t lr, uint32_t msp, uint32_t psp)
{
    (void)lr;
    (void)msp;
    (void)psp;

    while (1) {
        __asm volatile ("nop");
    }
}

extern "C" int _write(int file, const void* buffer, size_t count)
{
    (void)file;

    const uint8_t* buf = (const uint8_t*)buffer;

    for (size_t i = 0; i < count; ++i)
    {
        char c = buf[i];

        if (c == '\n')
        {
            while (UART_IS_TX_FULL(UART0));
            UART_WRITE(UART0, '\r');
        }

        while (UART_IS_TX_FULL(UART0));
        UART_WRITE(UART0, c);
    }

    return static_cast<int>(count);
}

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
    
    /* Nu-Link-Me UART0 setup */
    {
        CLK_EnableModuleClock(UART0_MODULE);
    
        /* Set UART0 module clock source (HXT) and divider (1) */
        CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));
    
        /* Setup PB.12 and PB.13 as multi-function pins for UART0 RXD and TXD */
        SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk);
        SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
    }
}


int main()
{
    SYS_UnlockReg();
    SYS_Init();
    SYS_LockReg();

    // Open UART0 connection at 115200 baudrate
    UART_Open(UART0, 115200);

    GPIO_SetMode(PH, BIT1, GPIO_MODE_OUTPUT);
    
    size_t i = 0;

    while (1)
    {
        PH1 ^= BIT0;

        delay();
    }

    UART_Close(UART0);

    return 0;
}