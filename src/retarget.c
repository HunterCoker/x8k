#include <stdint.h>
#include <stddef.h>

extern void uart_put_char(const char c);

void ProcessHardFault(uint32_t lr, uint32_t msp, uint32_t psp)
{
    (void)lr;
    (void)msp;
    (void)psp;

    while (1) {
        __asm volatile ("nop");
    }
}

int _write(int file, const void* buffer, size_t count)
{
    (void)file;

    for (size_t i = 0; i < count; ++i)
    {
        char c = ((const uint8_t*)buffer)[i];

        if (c == '\n')
            uart_put_char('\r');
        uart_put_char(c);
    }

    return (int)count;
}