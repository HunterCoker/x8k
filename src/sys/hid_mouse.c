#include "hid_mouse.h"

#include "NuMicro.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>


extern volatile uint8_t g_hsusbd_RemoteWakeupEn;

static volatile bool g_u8EPAReady = false;
static volatile bool g_u8EPBReady = false;
static volatile bool g_u8Suspend  = false;
static volatile uint8_t g_u8ReportProtocol = HID_REPORT_PROTOCOL;

static void HSUSBD_ConfigureInterrupts(void)
{
    HSUSBD_ENABLE_USB_INT(HSUSBD_GINTEN_USBIEN_Msk |
                          HSUSBD_GINTEN_CEPIEN_Msk |
                          HSUSBD_GINTEN_EPAIEN_Msk |
                          HSUSBD_GINTEN_EPBIEN_Msk);

    HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk     |
                          HSUSBD_BUSINTEN_SUSPENDIEN_Msk |
                          HSUSBD_BUSINTEN_RESUMEIEN_Msk);

    HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk);
}

static void HSUSBD_ConfigureEndpoints(void)
{
    /* Control endpoint */
    HSUSBD_SetEpBufAddr(CEP, CEP_BUF_BASE, CEP_BUF_LEN);
    HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk |
                          HSUSBD_CEPINTEN_STSDONEIEN_Msk);

    /* EPA ==> Interrupt IN endpoint, address 1 */
    HSUSBD_SetEpBufAddr(EPA, EPA_BUF_BASE, EPA_BUF_LEN);
    HSUSBD_SET_MAX_PAYLOAD(EPA, EPA_MAX_PKT_SIZE);
    HSUSBD_ConfigEp(EPA, INT_IN_EP_NUM, HSUSBD_EP_CFG_TYPE_INT, HSUSBD_EP_CFG_DIR_IN);

    /* EPB ==> Interrupt OUT endpoint, address 2 */
    HSUSBD_SetEpBufAddr(EPB, EPB_BUF_BASE, EPB_BUF_LEN);
    HSUSBD_SET_MAX_PAYLOAD(EPB, EPB_MAX_PKT_SIZE);
    HSUSBD_ConfigEp(EPB, INT_OUT_EP_NUM, HSUSBD_EP_CFG_TYPE_INT, HSUSBD_EP_CFG_DIR_OUT);
}

void USBD20_IRQHandler(void)
{
    __IO uint32_t IrqStL, IrqSt;

    IrqStL = HSUSBD->GINTSTS & HSUSBD->GINTEN;
    if (!IrqStL) return;
    
    /* bus event interrupts */

    if (IrqStL & HSUSBD_GINTSTS_USBIF_Msk)
    {
        IrqSt = HSUSBD->BUSINTSTS & HSUSBD->BUSINTEN;

        if (IrqSt & HSUSBD_BUSINTSTS_RSTIF_Msk)
        {
            printf("  [BUS] resetting...\n");

            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_RSTIF_Msk);

            g_u8Suspend = false;

            HSUSBD_SwReset();
            HSUSBD_ResetDMA();
            
            HSUSBD->EP[EPA].EPRSPCTL = HSUSBD_EPRSPCTL_FLUSH_Msk;

            HSUSBD_SET_ADDR(0x00);
            HSUSBD_ConfigureInterrupts();
            HSUSBD_ConfigureEndpoints();
        }

        if (IrqSt & HSUSBD_BUSINTSTS_SUSPENDIF_Msk)
        {
            printf("  [BUS] suspending...\n");

            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_SUSPENDIF_Msk);

            g_u8Suspend = true;

            HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk |
                                  HSUSBD_BUSINTEN_RESUMEIEN_Msk);
        }

        if (IrqSt & HSUSBD_BUSINTSTS_RESUMEIF_Msk)
        {
            printf("  [BUS] resuming...\n");

            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_RESUMEIF_Msk);

            g_u8Suspend = false;

            HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk |
                                  HSUSBD_BUSINTEN_SUSPENDIEN_Msk);
        }
    }

    /* control endpoint interrupts */

    if (IrqStL & HSUSBD_GINTSTS_CEPIF_Msk)
    {
        IrqSt = HSUSBD->CEPINTSTS & HSUSBD->CEPINTEN;

        if (IrqSt & HSUSBD_CEPINTSTS_SETUPPKIF_Msk)
        {
            printf("  [CEP] setup packet...\n");
            
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_SETUPPKIF_Msk);

            HSUSBD_ProcessSetupPacket();
            return;
        }

        if (IrqSt & HSUSBD_CEPINTSTS_STSDONEIF_Msk)
        {
            printf("  [CEP] done...\n");
            
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);

            HSUSBD_UpdateDeviceState();

            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk);            
            return;
        }

        if (IrqSt & HSUSBD_CEPINTSTS_INTKIF_Msk)
        {
            printf("  [CEP] token in...\n");

            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);

            if (!(IrqSt & HSUSBD_CEPINTSTS_STSDONEIF_Msk))
            {
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_TXPKIEN_Msk);
                HSUSBD_CtrlIn();
            }
            else
            {
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_TXPKIEN_Msk |
                                      HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            }
            return;
        }

        if (IrqSt & HSUSBD_CEPINTSTS_OUTTKIF_Msk)
        {
            printf("  [CEP] token out...\n");

            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_OUTTKIF_Msk);
            
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            return;
        }

        if (IrqSt & HSUSBD_CEPINTSTS_RXPKIF_Msk)
        {
            printf("  [CEP] rx packet...\n");
        
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_RXPKIF_Msk);

            HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);

            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk |
                                  HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            return;
        }

        if (IrqSt & HSUSBD_CEPINTSTS_TXPKIF_Msk)
        {
            printf("  [CEP] tx packet...\n");

            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);

            HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);

            if (g_hsusbd_CtrlInSize)
            {
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_INTKIEN_Msk);
            }
            else
            {
                if (g_hsusbd_CtrlZero == 1)
                    HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_ZEROLEN);

                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk |
                                      HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            }
            return;
        }

        if (IrqSt & HSUSBD_CEPINTSTS_STALLIF_Msk)
        {
            printf("  [CEP] stall...\n");

            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STALLIF_Msk);
            return;
        }

        printf("  [warn] CEP INTERRUPT NOT HANDLED\n");
    }

    /* other endpoint interrupts */

    if (IrqStL & HSUSBD_GINTSTS_EPAIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPA].EPINTSTS & HSUSBD->EP[EPA].EPINTEN;

        HSUSBD_ENABLE_EP_INT(EPA, 0);

        EPA_Handler();

        HSUSBD_CLR_EP_INT_FLAG(EPA, IrqSt);
    }
    
    if (IrqStL & HSUSBD_GINTSTS_EPBIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPB].EPINTSTS & HSUSBD->EP[EPB].EPINTEN;

        EPB_Handler();

        HSUSBD_CLR_EP_INT_FLAG(EPB, IrqSt);
    }
}

void HID_VendorRequest(void)
{
    printf("HID_VendorRequest(void) called...\n");

    if (gUsbCmd.bmRequestType & 0x80)   /* request data transfer direction */
    {
        // Device to host
        switch (gUsbCmd.bRequest)
        {
        default:
        {
            /* Setup error, stall the device */
            HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
            break;
        }
        }
    }
    else
    {
        // Host to device
        switch (gUsbCmd.bRequest)
        {
        default:
        {
            // Stall
            /* Setup error, stall the device */
            HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
            break;
        }
        }
    }
}

void HID_RemoteWakeupProcess(void)
{
    printf("HID_RemoteWakeupProcess(void) called...\n");

    if (g_hsusbd_RemoteWakeupEn)
    {

    }
}

void EPA_Handler(void)
{
    g_u8EPAReady = true;
}

void EPB_Handler(void)
{
    g_u8EPBReady = true;
}

void HID_Init(void)
{
    HSUSBD_Open(&gsHSInfo, HID_ClassRequest, NULL);

    HSUSBD_SET_ADDR(0x00000000);
    HSUSBD_ConfigureInterrupts();
    HSUSBD_ConfigureEndpoints();

    NVIC_EnableIRQ(USBD20_IRQn);
    
    HSUSBD_Start();
    
    g_u8EPAReady = true;
}

void HID_ClassRequest(void)
{
    printf("HID_ClassRequest(void) called...\n");

    static uint8_t u8Report = 0;
    static uint8_t u8Idle = 0;

    if (gUsbCmd.bmRequestType & 0x80) // Device to host
    {
        switch (gUsbCmd.bRequest)
        {
        case GET_REPORT:
            HSUSBD_PrepareCtrlIn(&u8Report, 1ul);
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_INTKIEN_Msk);
            break;
        case GET_IDLE:
            HSUSBD_PrepareCtrlIn(&u8Idle, 1ul);
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_INTKIEN_Msk);
            break;
        case GET_PROTOCOL:
            if (gUsbCmd.wIndex == 0x00)
            {
                HSUSBD_PrepareCtrlIn((uint8_t *)&g_u8ReportProtocol, 1ul);
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_INTKIEN_Msk);
                break;
            }
            else
            {
                // Stall
                /* Setup error, stall the device */
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
                break;
            }
        default:
            // Stall
            /* Setup error, stall the device */
            HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
            break;
        }
    }
    else // Host to device
    {
        switch (gUsbCmd.bRequest)
        {
        case SET_REPORT:
            if (((gUsbCmd.wValue >> 8) & 0xff) == 3) /* Request Type = Feature */
            {    
                /* Status stage */
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            }
            else
            {
                // Stall
                /* Setup error, stall the device */
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
            }
            break;
        case SET_IDLE:
            u8Idle = (gUsbCmd.wValue >> 8) & 0xff;
        
            /* Status stage */
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
            HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            break;
        case SET_PROTOCOL:
            if (gUsbCmd.wIndex == 0x00)
            {
                if (gUsbCmd.wValue == HID_BOOT_PROTOCOL)
                    g_u8ReportProtocol = HID_BOOT_PROTOCOL;
                else
                    g_u8ReportProtocol = HID_REPORT_PROTOCOL;
                
                /* Status stage */
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            }
            else
            {
                // Stall
                /* Setup error, stall the device */
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
            }
            break;
        default:
            // Stall
            /* Setup error, stall the device */
            HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
            break;
        }
    }
}

void HID_Update(int16_t dx, int16_t dy)
{
    if (g_u8EPAReady)
    {
        /* Build report data */
        mouse_report_t report = {0};
        report.buttons = 0x00;
        report.dx = 0x00;
        report.dy = 0x00;
        report.wheel = 0x00;
        
        /* Set transfer length and trigger IN transfer */
        HSUSBD->EP[EPA].EPDAT_BYTE = report.buttons;
        HSUSBD->EP[EPA].EPDAT_BYTE = report.dx;
        HSUSBD->EP[EPA].EPDAT_BYTE = report.dy;
        HSUSBD->EP[EPA].EPDAT_BYTE = report.wheel;
        HSUSBD->EP[EPA].EPTXCNT = sizeof(report);

        g_u8EPAReady = false;

        HSUSBD_ENABLE_EP_INT(EPA, HSUSBD_EPINTEN_INTKIEN_Msk);
    }
}

