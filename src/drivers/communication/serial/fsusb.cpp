#include "drivers/communication/serial/fsusb.h"

#include <M480.h>
#include <usbd.h>

namespace communication::serial
{

void FSUsb::initialize()
{
    SYS_UnlockReg();

    SYS_LockReg();
}

}  // namespace communication::serial