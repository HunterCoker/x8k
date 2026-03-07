#pragma once

#include "drivers/util_macros.h"

#include <stdint.h>

class Drivers;

namespace communication::sensors
{

/**
 * Driver interface for the Paw3395 optical navigation sensor.
 *
 * Note: Driver implementation is hidden behind an NDA.
 *
 * TODO: Precompile this driver as static library and link to it to hide implementation,
 *  while also allowing firmware to be compiled by anyone.
 */
class Paw3395
{
private:
    typedef uint8_t     register_t ;
    
public:
    Paw3395(Drivers* drivers);
    DISALLOW_COPY_AND_ASSIGN(Paw3395);
    ~Paw3395() = default;

    void initialize();

private:
    void loadInitRegisterSetting() const;

    uint8_t read(register_t reg) const;
    void write(register_t reg, const uint8_t tx) const;

private:
    enum Register : register_t;
    struct __attribute__((packed)) MotionBurst;

private:
    Drivers* _M_drivers;
};

}  // namespace communication::sensors