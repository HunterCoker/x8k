#include "sys_init.h"

#include <NuMicro.h>
#include <clk.h>
#include <sys.h>

void SYS_Init(void)
{
    SYS_UnlockReg();

    /* HXT clock setup */
    {
        /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
        GPIO_SetMode(PF, BIT2 | BIT3, GPIO_MODE_INPUT);

        /* Enable HXT clock (external XTAL 12MHz) */
        CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

        /* Wait for HXT clock ready */
        CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

        /* Set MCU core clock to its maximum frequency of 192MHz */
        CLK_SetCoreClock(FREQ_192MHZ);
        CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV2;
    }

    /*  HSUSB setup */
    {
        /* Select HSUSBD, disable active, enable PHY power */
        SYS->USBPHY = (SYS->USBPHY & ~(SYS_USBPHY_HSUSBROLE_Msk | SYS_USBPHY_HSUSBACT_Msk)) | SYS_USBPHY_HSUSBEN_Msk;
        for (volatile int i = 0; i < 0x1000; ++i); // delay > 10 us
        
        /* Activate HSUSB PHY */
        SYS->USBPHY |= SYS_USBPHY_HSUSBACT_Msk;

        CLK_EnableModuleClock(HSUSBD_MODULE);
    }

    SYS_LockReg();
}