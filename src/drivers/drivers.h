#pragma once

#include "architecture/clock.h"
#include "communication/serial/uart.h"
#include "communication/serial/spi.h"
#include "communication/serial/fsusb.h"
#include "communication/serial/hsusb.h"
#include "communication/sensors/paw3395.h"

class Drivers
{
public:
    Drivers()
        : clock(),
          uart(), spi(), fsusb(), hsusb(),
          paw3395(this)
    {}

    static Drivers* get();

    void initialize();

public:
    architecture::Clock clock;
    communication::serial::Uart  uart;
    communication::serial::Spi   spi;
    communication::serial::FSUsb fsusb;
    communication::serial::HSUsb hsusb;
    communication::sensors::Paw3395 paw3395;

private:
    static Drivers* _S_driversInstance;
};