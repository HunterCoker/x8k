#include <stdint.h>
#include <stddef.h>

#include "drivers/drivers.h"

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
    Drivers* drivers = Drivers::get();
    
    (void)file;

    for (size_t i = 0; i < count; ++i)
    {
        char c = (static_cast<const uint8_t*>(buffer))[i];

        if (c == '\n')
            drivers->uart.putChar('\r');
        drivers->uart.putChar(c);
    }

    return static_cast<int>(count);
}