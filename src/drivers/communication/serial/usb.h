#pragma once

#include "drivers/util_macros.h"

#include "iusb.h"

namespace communication::serial
{

/**
 *
 */
class Usb : public IUsb
{
public:
    Usb() = default;
    DISALLOW_COPY_AND_ASSIGN(Usb);
    ~Usb() = default;

    void initialize() override;
    void bufferReport(const uint8_t* report, uint32_t size) override;
};

}  // namespace communication::serial