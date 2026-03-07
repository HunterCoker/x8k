#include "drivers/drivers.h"

Drivers* Drivers::_S_driversInstance = nullptr;

Drivers* Drivers::get()
{
    if (!_S_driversInstance)
        _S_driversInstance = nullptr;
    return _S_driversInstance;
}

void Drivers::initialize()
{
    clock.initialize();
    uart.initialize();
    spi.initialize();
    fsusb.initialize();
    hsusb.initialize();
    paw3395.initialize();
}