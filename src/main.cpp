#include "drivers/drivers.h"

int main(void)
{
    Drivers* drivers = Drivers::get();
    drivers->initialize();

    while (true)
    {
    }
}
