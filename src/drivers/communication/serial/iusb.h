#pragma once

#include "drivers/util_macros.h"

#include <stdint.h>
#include <stddef.h>

#define REQUEST_TRANSFER_DIRECTION_Msk  0x80
#define REQUEST_TYPE_Msk                0x60
#define REQUEST_RECIPIENT_Msk           0x1f

enum ReqTransferDirection : uint8_t
{
    HostToDevice  = 0,
    DeviceToHost  = 1,
};

enum ReqType : uint8_t
{
    Standard    = 0,
    Class       = 1,
    Vendor      = 2,
};

enum ReqRecipient : uint8_t
{
    Device      = 0,
    Interface   = 1,
    Endpoint    = 2,
    Other       = 3,
};

struct UsbSetupPacket
{
    uint8_t  bmRequestType;
    uint8_t  bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;

    UsbSetupPacket(uint8_t buf[8])
    {
        bmRequestType = buf[0];
        bRequest      = buf[1];
        wValue        = (static_cast<uint16_t>(buf[2]) << 8) | buf[3];
        wIndex        = (static_cast<uint16_t>(buf[4]) << 8) | buf[5];
        wLength       = (static_cast<uint16_t>(buf[6]) << 8) | buf[7];
    }
    
    uint8_t getReqDirection() const
    {
        return static_cast<uint8_t>((bmRequestType & REQUEST_TRANSFER_DIRECTION_Msk) >> 7);
    }

    uint8_t getReqType() const
    {
        return (static_cast<uint8_t>(bmRequestType & REQUEST_TYPE_Msk) >> 5);
    }

    uint8_t getReqRecipient() const
    {
        return static_cast<uint8_t>(bmRequestType & REQUEST_RECIPIENT_Msk);
    }
};

class IUsb
{
public:
    IUsb() = default;
    DISALLOW_COPY_AND_ASSIGN(IUsb);
    ~IUsb() = default;

    virtual void initialize() = 0;
    virtual void bufferReport(const uint8_t* report, size_t size) = 0;
};