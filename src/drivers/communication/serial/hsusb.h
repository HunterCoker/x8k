#pragma once

#include "drivers/util_macros.h"

namespace communication::serial
{

/**
 *
 */
class HSUsb
{
public:
    HSUsb() = default;
    DISALLOW_COPY_AND_ASSIGN(HSUsb);
    ~HSUsb() = default;

    void initialize();
};

}  // namespace communication::serial