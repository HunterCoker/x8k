#pragma once

#include "drivers/util_macros.h"

#include "iusb.h"

namespace communication::serial
{

/**
 *
 */
class HSUsb : public IUsb
{
public:
    HSUsb() = default;
    DISALLOW_COPY_AND_ASSIGN(HSUsb);
    ~HSUsb() = default;

    void initialize() override;
    void bufferReport(const uint8_t* report, size_t size) override;
};

}  // namespace communication::serial