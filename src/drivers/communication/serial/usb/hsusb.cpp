#include "drivers/communication/serial/hsusb.h"

#include <M480.h>
#include <hsusbd.h>

#include "hid.h"

extern "C" void USBD20_IRQHandler(void);
void HSUSBD_ClassRequest(void);

// /* Define EP buffer addresses */
// #define CEP_BUF_BASE    0x00
// #define CEP_BUF_LEN     CEP_MAX_PKT_SIZE
// #define EPA_BUF_BASE    (CEP_BUF_BASE + CEP_BUF_LEN)
// #define EPA_BUF_LEN     EPA_MAX_PKT_SIZE
// #define EPB_BUF_BASE    (EPA_BUF_BASE + EPA_BUF_LEN)
// #define EPB_BUF_LEN     EPB_MAX_PKT_SIZE

// /* Define EP maximum packet size */
// #define CEP_MAX_PKT_SIZE        64
// #define CEP_OTHER_MAX_PKT_SIZE  64
// #define EPA_MAX_PKT_SIZE        64
// #define EPA_OTHER_MAX_PKT_SIZE  64
// #define EPB_MAX_PKT_SIZE        64
// #define EPB_OTHER_MAX_PKT_SIZE  64

// /* Standard request mappings */
// #define USB_REQ_GET_STATUS        0x00
// #define USB_REQ_CLEAR_FEATURE     0x01
// #define USB_REQ_SET_FEATURE       0x03
// #define USB_REQ_SET_ADDRESS       0x05
// #define USB_REQ_GET_DESCRIPTOR    0x06
// #define USB_REQ_SET_DESCRIPTOR    0x07
// #define USB_REQ_GET_CONFIGURATION 0x08
// #define USB_REQ_SET_CONFIGURATION 0x09
// #define USB_REQ_GET_INTERFACE     0x0A
// #define USB_REQ_SET_INTERFACE     0x0B
// #define USB_REQ_SYNCH_FRAME       0x0C

// extern volatile uint8_t g_hsusbd_RemoteWakeupEn;

// static volatile bool g_EPAReady = false;
// static volatile bool g_EPBReady = false;
// static volatile bool g_suspend  = false;
// static volatile uint8_t g_u8ReportProtocol = HID_REPORT_PROTOCOL;

// void HSUSBD_ConfigureInterrupts()
// {
//     HSUSBD_ENABLE_USB_INT(HSUSBD_GINTEN_USBIEN_Msk |
//                           HSUSBD_GINTEN_CEPIEN_Msk |
//                           HSUSBD_GINTEN_EPAIEN_Msk |
//                           HSUSBD_GINTEN_EPBIEN_Msk);

//     HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk     |
//                           HSUSBD_BUSINTEN_SUSPENDIEN_Msk |
//                           HSUSBD_BUSINTEN_RESUMEIEN_Msk);

//     HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk);
// }

// void HSUSBD_ConfigureEndpoints()
// {
//     /* Control endpoint */
//     HSUSBD_SetEpBufAddr(CEP, CEP_BUF_BASE, CEP_BUF_LEN);
//     HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk |
//                           HSUSBD_CEPINTEN_STSDONEIEN_Msk);

//     /* EPA ==> Interrupt IN endpoint, address 1 */
//     HSUSBD_SetEpBufAddr(EPA, EPA_BUF_BASE, EPA_BUF_LEN);
//     HSUSBD_SET_MAX_PAYLOAD(EPA, EPA_MAX_PKT_SIZE);
//     HSUSBD_ConfigEp(EPA, INT_IN_EP_NUM, HSUSBD_EP_CFG_TYPE_INT, HSUSBD_EP_CFG_DIR_IN);

//     /* EPB ==> Interrupt OUT endpoint, address 2 */
//     HSUSBD_SetEpBufAddr(EPB, EPB_BUF_BASE, EPB_BUF_LEN);
//     HSUSBD_SET_MAX_PAYLOAD(EPB, EPB_MAX_PKT_SIZE);
//     HSUSBD_ConfigEp(EPB, INT_OUT_EP_NUM, HSUSBD_EP_CFG_TYPE_INT, HSUSBD_EP_CFG_DIR_OUT);
// }

namespace communication::serial
{

void HSUsb::initialize()
{
    SYS_UnlockReg();

    // CLK_EnableModuleClock(HSUSBD_MODULE);

    // /* Configure PHY for HSUSB */
    // {
    //     /* Select HSUSBD, disable active, enable PHY power */
    //     SYS->USBPHY &= ~(SYS_USBPHY_HSUSBROLE_Msk | SYS_USBPHY_HSUSBACT_Msk);
    //     SYS->USBPHY |= SYS_USBPHY_HSUSBEN_Msk;
    
    //     CLK_SysTickDelay(10);  // delay > 10 us
        
    //     /* Activate HSUSB PHY */
    //     SYS->USBPHY |= SYS_USBPHY_HSUSBACT_Msk;
    // }

    // HSUSBD_Open(&gsHSInfo, HSUSBD_ClassRequest, NULL);

    // HSUSBD_SET_ADDR(0x00000000);
    // HSUSBD_ConfigureEndpoints();
    // HSUSBD_ConfigureInterrupts();
    // NVIC_EnableIRQ(USBD20_IRQn);
    
    HSUSBD_Start();

    SYS_LockReg();
}

void HSUsb::bufferReport(const uint8_t* report, size_t size)
{
    
}

}  // namespace communication::serial

// extern "C" void USBD20_IRQHandler(void)
// {
//     __IO uint32_t IrqStL, IrqSt;

//     IrqStL = HSUSBD->GINTSTS & HSUSBD->GINTEN;
//     if (!IrqStL) return;
    
//     /* bus event interrupts */

//     if (IrqStL & HSUSBD_GINTSTS_USBIF_Msk)
//     {
//         IrqSt = HSUSBD->BUSINTSTS & HSUSBD->BUSINTEN;

//         if (IrqSt & HSUSBD_BUSINTSTS_RSTIF_Msk)
//         {
//             printf("  [BUS] resetting...\n");

//             HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_RSTIF_Msk);

//             g_suspend = false;

//             HSUSBD_SwReset();
//             HSUSBD_ResetDMA();
            
//             HSUSBD->EP[EPA].EPRSPCTL = HSUSBD_EPRSPCTL_FLUSH_Msk;

//             HSUSBD_SET_ADDR(0x00);
//             HSUSBD_ConfigureInterrupts();
//             HSUSBD_ConfigureEndpoints();
//         }

//         if (IrqSt & HSUSBD_BUSINTSTS_SUSPENDIF_Msk)
//         {
//             printf("  [BUS] suspending...\n");

//             HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_SUSPENDIF_Msk);

//             g_suspend = true;

//             HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk |
//                                   HSUSBD_BUSINTEN_RESUMEIEN_Msk);
//         }

//         if (IrqSt & HSUSBD_BUSINTSTS_RESUMEIF_Msk)
//         {
//             printf("  [BUS] resuming...\n");

//             HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_RESUMEIF_Msk);

//             g_suspend = false;

//             HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk |
//                                   HSUSBD_BUSINTEN_SUSPENDIEN_Msk);
//         }
//     }

//     /* control endpoint interrupts */

//     if (IrqStL & HSUSBD_GINTSTS_CEPIF_Msk)
//     {
//         IrqSt = HSUSBD->CEPINTSTS & HSUSBD->CEPINTEN;

//         if (IrqSt & HSUSBD_CEPINTSTS_SETUPPKIF_Msk)
//         {
//             printf("  [CEP] setup packet...\n");
            
//             HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_SETUPPKIF_Msk);

//             HSUSBD_ProcessSetupPacket();
//             return;
//         }

//         if (IrqSt & HSUSBD_CEPINTSTS_STSDONEIF_Msk)
//         {
//             printf("  [CEP] done...\n");
            
//             HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);

//             HSUSBD_UpdateDeviceState();

//             HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk);            
//             return;
//         }

//         if (IrqSt & HSUSBD_CEPINTSTS_INTKIF_Msk)
//         {
//             printf("  [CEP] token in...\n");

//             HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);

//             if (!(IrqSt & HSUSBD_CEPINTSTS_STSDONEIF_Msk))
//             {
//                 HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
//                 HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_TXPKIEN_Msk);
//                 HSUSBD_CtrlIn();
//             }
//             else
//             {
//                 HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
//                 HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_TXPKIEN_Msk |
//                                       HSUSBD_CEPINTEN_STSDONEIEN_Msk);
//             }
//             return;
//         }

//         if (IrqSt & HSUSBD_CEPINTSTS_OUTTKIF_Msk)
//         {
//             printf("  [CEP] token out...\n");

//             HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_OUTTKIF_Msk);
            
//             HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
//             return;
//         }

//         if (IrqSt & HSUSBD_CEPINTSTS_RXPKIF_Msk)
//         {
//             printf("  [CEP] rx packet...\n");
        
//             HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_RXPKIF_Msk);

//             HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);

//             HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk |
//                                   HSUSBD_CEPINTEN_STSDONEIEN_Msk);
//             return;
//         }

//         if (IrqSt & HSUSBD_CEPINTSTS_TXPKIF_Msk)
//         {
//             printf("  [CEP] tx packet...\n");

//             HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
//             HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);

//             HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);

//             if (g_hsusbd_CtrlInSize)
//             {
//                 HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
//                 HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_INTKIEN_Msk);
//             }
//             else
//             {
//                 if (g_hsusbd_CtrlZero == 1)
//                     HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_ZEROLEN);

//                 HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
//                 HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk |
//                                       HSUSBD_CEPINTEN_STSDONEIEN_Msk);
//             }
//             return;
//         }

//         if (IrqSt & HSUSBD_CEPINTSTS_STALLIF_Msk)
//         {
//             printf("  [CEP] stall...\n");

//             HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STALLIF_Msk);
//             return;
//         }

//         printf("  [warn] CEP INTERRUPT NOT HANDLED\n");
//     }

//     /* other endpoint interrupts */

//     if (IrqStL & HSUSBD_GINTSTS_EPAIF_Msk)
//     {
//         IrqSt = HSUSBD->EP[EPA].EPINTSTS & HSUSBD->EP[EPA].EPINTEN;

//         HSUSBD_ENABLE_EP_INT(EPA, 0);

//         g_EPAReady = true;

//         HSUSBD_CLR_EP_INT_FLAG(EPA, IrqSt);
//     }
    
//     if (IrqStL & HSUSBD_GINTSTS_EPBIF_Msk)
//     {
//         IrqSt = HSUSBD->EP[EPB].EPINTSTS & HSUSBD->EP[EPB].EPINTEN;

//         g_EPBReady = true;

//         HSUSBD_CLR_EP_INT_FLAG(EPB, IrqSt);
//     }
// }

// extern "C" void HSUSBD_ClassRequest(void)
// {
//     static uint8_t u8Report = 0;
//     static uint8_t u8Idle   = 0;

//     if (gUsbCmd.bmRequestType & BIT7)  // Device to host
//     {
//         switch (gUsbCmd.bRequest)
//         {
//         case GET_REPORT:
//             HSUSBD_PrepareCtrlIn(&u8Report, 1ul);
//             HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
//             HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_INTKIEN_Msk);
//             break;
//         case GET_IDLE:
//             HSUSBD_PrepareCtrlIn(&u8Idle, 1ul);
//             HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
//             HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_INTKIEN_Msk);
//             break;
//         case GET_PROTOCOL:
//             if (gUsbCmd.wIndex == 0x00)
//             {
//                 HSUSBD_PrepareCtrlIn((uint8_t *)&g_u8ReportProtocol, 1ul);
//                 HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
//                 HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_INTKIEN_Msk);
//                 break;
//             }
//             else
//             {
//                 /* Setup error, stall the device */
//                 HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
//                 break;
//             }
//         default:
//             /* Setup error, stall the device */
//             HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
//             break;
//         }
//     }
//     else  // Host to device
//     {
//         switch (gUsbCmd.bRequest)
//         {
//         case SET_REPORT:
//             if (((gUsbCmd.wValue >> 8) & 0xff) == 3) /* Request Type = Feature */
//             {    
//                 /* Status stage */
//                 HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
//                 HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
//                 HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
//             }
//             else
//             {
//                 // Stall
//                 /* Setup error, stall the device */
//                 HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
//             }
//             break;
//         case SET_IDLE:
//             u8Idle = (gUsbCmd.wValue >> 8) & 0xff;
        
//             /* Status stage */
//             HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
//             HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
//             HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
//             break;
//         case SET_PROTOCOL:
//             if (gUsbCmd.wIndex == 0x00)
//             {
//                 if (gUsbCmd.wValue == HID_BOOT_PROTOCOL)
//                     g_u8ReportProtocol = HID_BOOT_PROTOCOL;
//                 else
//                     g_u8ReportProtocol = HID_REPORT_PROTOCOL;
                
//                 /* Status stage */
//                 HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
//                 HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
//                 HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
//             }
//             else
//             {
//                 // Stall
//                 /* Setup error, stall the device */
//                 HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
//             }
//             break;
//         default:
//             // Stall
//             /* Setup error, stall the device */
//             HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
//             break;
//         }
//     }
// }