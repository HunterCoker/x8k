#include "drivers/communication/gpio/gpio_pin.h"

#include <M480.h>
#include <gpio.h>

namespace communication::gpio
{

static GPIO_T mapPort(GpioPin::Port port)
{
    switch (port)
    {

    }
}

static mapPin(GpioPin::Mode mode)
{

}

GpioPin::GpioPin(Mode mode)
{
    GPIO_T port = mapPin();
    GPIO_SetMode(port, pin, mode);
}

void GpioPin::setHigh() const
{
    _M_pin = 1;
}

void GpioPin::setLow() const
{
    _M_pin = 0;
}

}  // communication::gpio