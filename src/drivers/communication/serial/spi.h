#pragma once

#include "drivers/util_macros.h"

#include <stddef.h>
#include <stdint.h>

namespace communication::serial
{
    
/**
 *
 */
class Spi
{
public:
    Spi() = default;
    DISALLOW_COPY_AND_ASSIGN(Spi);
    ~Spi() = default;
 
    void initialize();

    uint8_t transmitByte(const uint8_t tx) const;
    void transfer(const uint8_t* tx, uint8_t* rx, size_t n) const;
};

}  // namespace communication::serial