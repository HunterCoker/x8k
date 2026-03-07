#pragma once

#include "drivers/util_macros.h"

#include <stdint.h>

constexpr uint32_t operator""_us(unsigned long long us)
{
    return static_cast<uint32_t>(us);
}

constexpr uint32_t operator""_ms(unsigned long long ms)
{
    return static_cast<uint32_t>(ms * 1000ull);
}

namespace architecture
{

/**
 * HXT clock
 */
class Clock
{
public:
    Clock() = default;
    DISALLOW_COPY_AND_ASSIGN(Clock);
    ~Clock() = default;

    void initialize();

    void delay(uint32_t us);
};

}  // namespace architecture