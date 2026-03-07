#pragma once

#include "drivers/util_macros.h"

namespace communication::serial
{

/**
 *
 */
class FSUsb
{
public:
    FSUsb() = default;
    DISALLOW_COPY_AND_ASSIGN(FSUsb);
    ~FSUsb() = default;

    void initialize();
};

}  // namespace communication::serial