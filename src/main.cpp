#include <NuMicro.h>

extern "C" void ProcessHardFault(uint32_t lr, uint32_t msp, uint32_t psp)
{
    (void)lr;
    (void)msp;
    (void)psp;

    while (1) {
        __asm volatile ("nop");
    }
}

static void delay()
{
    for (volatile uint32_t i = 0; i < 500000; i++);
}

int main()
{
    GPIO_SetMode(PH, BIT1, GPIO_MODE_OUTPUT);

    while (1)
    {
        PH1 ^= 1;
        delay();
    }

    return 0;
}