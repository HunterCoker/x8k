#pragma once

#include "drivers/util_macros.h"

namespace communication::serial
{

/**
 *
 */
class Uart
{
public:
    Uart() = default;
    DISALLOW_COPY_AND_ASSIGN(Uart);
    ~Uart() = default;

    void initialize();

    void putChar(const char c) const;
};

}  // namespace communication::serial