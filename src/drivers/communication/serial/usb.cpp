#include "drivers/communication/serial/usb.h"

#include <M480.h>
#include <clk.h>
#include <usbd.h>

#include "hid.h"

#include <string.h>

/* USB Globals ***********************************************/

/* Descriptors */

extern S_USBD_INFO_T gsUsbdInfo;

/* Endpoint Configuration */

constexpr uint32_t SetupBufBase = 0x00000000;
constexpr uint32_t SetupBufLen  = 8;

constexpr uint32_t Ep0BufBase = SetupBufBase + SetupBufLen;
constexpr uint32_t Ep0BufLen  = 64;

constexpr uint32_t Ep1BufBase = Ep0BufBase + Ep0BufLen;
constexpr uint32_t Ep1BufLen  = 64;

constexpr uint32_t Ep2BufBase = Ep1BufBase + Ep1BufLen;
constexpr uint32_t Ep2BufLen  = 8;

/* HID state */

static volatile bool     g_ep2Ready    = true;
static volatile uint8_t  g_idleRate    = 0;
static volatile uint8_t  g_protocol    = 1;  // 0 = boot, 1 = report

alignas(4) uint8_t        g_pendingReport[Ep2BufLen] = {0};
static volatile uint32_t  g_pendingReportSize        = 0;
static volatile bool      g_reportPending            = false;

/* Endpoint Configuration ****************************************************/

void USBD_ConfigureEndpoints()
{
    USBD->STBUFSEG = SetupBufBase;

    /* EP0 = control IN, USB endpoint 0 IN */
    USBD_CONFIG_EP(EP0, USBD_CFG_CSTALL | USBD_CFG_EPMODE_IN  | 0);
    USBD_SET_EP_BUF_ADDR(EP0, Ep0BufBase);

    /* EP1 = control OUT, USB endpoint 0 OUT */
    USBD_CONFIG_EP(EP1, USBD_CFG_CSTALL | USBD_CFG_EPMODE_OUT | 0);
    USBD_SET_EP_BUF_ADDR(EP1, Ep1BufBase);

    /* EP2 = interrupt IN, USB endpoint 1 IN */
    USBD_CONFIG_EP(EP2, USBD_CFG_EPMODE_IN | INT_IN_EP_NUM);
    USBD_SET_EP_BUF_ADDR(EP2, Ep2BufBase);

    USBD_SET_PAYLOAD_LEN(EP0, 0);
    USBD_SET_PAYLOAD_LEN(EP1, 0);
}

void ep0SendData(const uint8_t* data, uint32_t len)
{
    if (len > Ep0BufLen)
        len = Ep0BufLen;

    memcpy(reinterpret_cast<void*>(USBD_BUF_BASE + Ep0BufBase),
           data,
           len);

    USBD_SET_DATA1(EP0);
    USBD_SET_PAYLOAD_LEN(EP0, len);
}

void trySendPendingEp2Report()
{
    if (!g_ep2Ready || !g_reportPending)
        return;

    memcpy(reinterpret_cast<void*>(USBD_BUF_BASE + Ep2BufBase),
           g_pendingReport,
           g_pendingReportSize);

    g_ep2Ready = false;
    g_reportPending = false;

    USBD_SET_PAYLOAD_LEN(EP2, g_pendingReportSize);
}

/* Interrupt Configuration & Handlers ****************************************/

void USBD_ConfigureInterrupts()
{
    USBD_ENABLE_INT(USBD_INTEN_BUSIEN_Msk |
                    USBD_INTEN_USBIEN_Msk);
}

extern "C" void USBD_IRQHandler(void)
{
    const uint32_t u32IntSts = USBD_GET_INT_FLAG();
    const uint32_t u32State  = USBD_GET_BUS_STATE();

    if (u32IntSts & USBD_INTSTS_VBDETIF_Msk)
    {
        USBD_CLR_INT_FLAG(USBD_INTSTS_VBDETIF_Msk);

        if (USBD_IS_ATTACHED())
        {
            USBD_ENABLE_USB();
        }
        else
        {
            USBD_DISABLE_USB();
        }
    }

    if (u32IntSts & USBD_INTSTS_BUSIF_Msk)
    {
        USBD_CLR_INT_FLAG(USBD_INTSTS_BUSIF_Msk);

        if (u32State & USBD_ATTR_USBRST_Msk)
        {
            USBD_ENABLE_USB();
            USBD_SwReset();

            g_ep2Ready          = true;
            g_idleRate          = 0;
            g_protocol          = 1;
            g_reportPending     = false;
            g_pendingReportSize = 0;

            USBD_ConfigureEndpoints();
        }

        if (u32State & USBD_ATTR_SUSPEND_Msk)
        {
            USBD_DISABLE_PHY();
        }

        if (u32State & USBD_ATTR_RESUME_Msk)
        {
            USBD_ENABLE_USB();
        }
    }

    if (u32IntSts & USBD_INTSTS_USBIF_Msk)
    {
        if (u32IntSts & USBD_INTSTS_SETUP_Msk)
        {
            USBD_CLR_INT_FLAG(USBD_INTSTS_SETUP_Msk);

            USBD_STOP_TRANSACTION(EP0);
            USBD_STOP_TRANSACTION(EP1);

            USBD_ProcessSetupPacket();
        }

        if (u32IntSts & USBD_INTSTS_EP0)
        {
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP0);
            USBD_CtrlIn();
        }

        if (u32IntSts & USBD_INTSTS_EP1)
        {
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP1);
            USBD_CtrlOut();
        }

        if (u32IntSts & USBD_INTSTS_EP2)
        {
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP2);

            g_ep2Ready = true;
            trySendPendingEp2Report();
        }
    }
}

void USBD_ClassRequest(void)
{
    uint8_t buf[8];
    USBD_GetSetupPacket(buf);
    UsbSetupPacket setupPacket(buf);

    /* For a basic HID mouse, class requests should be class/interface */
    if (setupPacket.getReqType()      != ReqType::Class ||
        setupPacket.getReqRecipient() != ReqRecipient::Interface)
    {
        USBD_SetStall(0);
        return;
    }

    /* If you only have interface 0, validate that */
    if ((setupPacket.wIndex & 0x00FFu) != 0u)
    {
        USBD_SetStall(0);
        return;
    }

    if (setupPacket.getReqDirection() == ReqTransferDirection::DeviceToHost)
    {
        switch(setupPacket.bRequest)
        {
            case HID_REQUEST_GET_REPORT:
            {
                /* Setup error, stall the device */
                USBD_SetStall(0);
                break;
            }
            case HID_REQUEST_GET_IDLE:
            {
                const uint8_t value = g_idleRate;
                ep0SendData(&value, 1);
                break;
            }
            case HID_REQUEST_GET_PROTOCOL:
            {
                const uint8_t value = g_protocol;
                ep0SendData(&value, 1);
                break;
            }
            default:
            {
                /* Setup error, stall the device */
                USBD_SetStall(0);
                break;
            }
        }
    }
    else
    {
        switch(setupPacket.bRequest)
        {
        case HID_REQUEST_SET_REPORT:
            if ((setupPacket.wIndex & 0x00ff) == HID_REPORT_TYPE_FEATURE)
            {
                USBD_SET_DATA1(EP1);
                USBD_SET_PAYLOAD_LEN(EP1, 0);
            }
            break;
        case HID_REQUEST_SET_IDLE:
            /* Status stage */
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 0);
            break;
        case HID_REQUEST_SET_PROTOCOL:
            break;
        default:
            /* Setup error, stall the device */
            USBD_SetStall(0);
            break;
        }
    }
}

namespace communication::serial
{

void Usb::initialize()
{
    SYS_UnlockReg();

    CLK_EnableModuleClock(USBD_MODULE);

    /* Configure PHY for USB */
    {
        /* Select USBD and enable PHY power */
        SYS->USBPHY &= ~(SYS_USBPHY_USBROLE_Msk);
        SYS->USBPHY |= SYS_USBPHY_USBEN_Msk | SYS_USBPHY_SBO_Msk;
    
        CLK_SysTickDelay(10);  // delay > 10 us
    }

    USBD_Open(&gsUsbdInfo, USBD_ClassRequest, NULL);

    USBD_SET_ADDR(0x00000000);
    USBD_ConfigureEndpoints();
    USBD_ConfigureInterrupts();
    NVIC_EnableIRQ(USBD_IRQn);
    
    USBD_Start();

    SYS_LockReg();
}

void Usb::bufferReport(const uint8_t* report, uint32_t size)
{
    if (report == nullptr || size == 0)
    {
        return;
    }

    if (size > Ep2BufLen)
    {
        size = Ep2BufLen;
    }

    memcpy(g_pendingReport, report, size);
    g_pendingReportSize = static_cast<uint32_t>(size);
    g_reportPending = true;

    trySendPendingEp2Report();
}

}  // namespace communication::serial