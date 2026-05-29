#pragma once

#include "drivers/util_macros.h"

namespace communication::gpio
{

/**
 * 
 */
class GpioPin
{
public:
    enum class Mode
    {
        Input,
        Output,
    };

public:
    GpioPin(Port port, Pin pin, Mode mode);
    DISALLOW_COPY_AND_ASSIGN(GpioPin);
    ~GpioPin() = default;

    void setHigh() const;
    void setLow() const;
};

}  // communication::gpio