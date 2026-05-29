#include "drivers/drivers.h"

static void updateIO(Drivers* drivers);

int main(void)
{
    Drivers* drivers = Drivers::get();
    drivers->initialize();

    /* TODO: profile the main loop to verify true 8k polling rate (i.e., ensure
        there is a fresh report ready every 125_us) */
    
    while (true)
    {
        updateIO(drivers);

        auto& sensor = drivers->paw3395;

        if (sensor.motionReady())
        {
            HIDReport report
            {
                .dx      = sensor.getDx(),
                .dy      = sensor.getDy(),
                .wheel   = 0x00,
                .buttons = 0x00,
            };

            drivers->usb.bufferReport(
                reinterpret_cast<uint8_t*>(&report),
                sizeof(report)
            );
        }
    }
}

static void updateIO(Drivers* drivers)
{
    drivers->paw3395.update();
}
