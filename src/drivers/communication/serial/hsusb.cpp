#include "drivers/communication/serial/hsusb.h"

#include <M480.h>
#include <hsusbd.h>

namespace communication::serial
{

void HSUsb::initialize()
{
    SYS_UnlockReg();

    // CLK_EnableModuleClock(HSUSBD_MODULE);

    // /* Select HSUSBD, disable active, enable PHY power */
    // SYS->USBPHY = (SYS->USBPHY & ~(SYS_USBPHY_HSUSBROLE_Msk | SYS_USBPHY_HSUSBACT_Msk)) | SYS_USBPHY_HSUSBEN_Msk;
    // for (volatile int i = 0; i < 0x1000; ++i); // delay > 10 us
    
    // /* Activate HSUSB PHY */
    // SYS->USBPHY |= SYS_USBPHY_HSUSBACT_Msk;

    // HSUSBD_Open(&gsHSInfo, HID_ClassRequest, NULL);

    // HSUSBD_SET_ADDR(0x00000000);
    // HSUSBD_ConfigureInterrupts();
    // HSUSBD_ConfigureEndpoints();

    // NVIC_EnableIRQ(USBD20_IRQn);
    
    // HSUSBD_Start();

    SYS_LockReg();
}

}  // namespace communication::serial