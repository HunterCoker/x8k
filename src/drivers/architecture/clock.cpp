#include "drivers/architecture/clock.h"

#include <M480.h>
#include <sys.h>
#include <clk.h>
#include <system_M480.h>

namespace architecture
{

void Clock::initialize()
{
    SYS_UnlockReg();

    /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    GPIO_SetMode(PF, BIT2 | BIT3, GPIO_MODE_INPUT);

    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set MCU core clock (PLL) to its maximum frequency of 192MHz */
    CLK_SetCoreClock(FREQ_192MHZ);
    SystemCoreClockUpdate();

    CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2;

    SYS_LockReg();
}

void Clock::delay(uint32_t us)
{
    const int32_t maxUs
        = ((1u << 24) - 1u) / (SystemCoreClock / 1000000u);

    if (us > maxUs)
        CLK_SysTickLongDelay(us);
    else
        CLK_SysTickDelay(us);
}

}  // namespace architecture