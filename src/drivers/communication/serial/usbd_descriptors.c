/**************************************************************************//**
 * @file     descriptors.c
 * @brief    M480 series USBD descriptor
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
/*!<Includes */

#include <stdint.h>

/* Define the vendor id and product id */
#define USBD_VID        0x0416
#define USBD_PID        0xB001

/*!<Define HID Class Specific Request */
#define GET_REPORT          0x01
#define GET_IDLE            0x02
#define GET_PROTOCOL        0x03
#define SET_REPORT          0x09
#define SET_IDLE            0x0A
#define SET_PROTOCOL        0x0B

/*!<USB HID Interface Class protocol */
#define HID_NONE            0x00
#define HID_KEYBOARD        0x01
#define HID_MOUSE           0x02

/*!<USB HID Class Report Type */
#define HID_RPT_TYPE_INPUT      0x01
#define HID_RPT_TYPE_OUTPUT     0x02
#define HID_RPT_TYPE_FEATURE    0x03

/*-------------------------------------------------------------*/
/* Define EP maximum packet size */
#define EP0_MAX_PKT_SIZE    8
#define EP1_MAX_PKT_SIZE    EP0_MAX_PKT_SIZE
#define EP2_MAX_PKT_SIZE    8

#define SETUP_BUF_BASE  0
#define SETUP_BUF_LEN   8
#define EP0_BUF_BASE    (SETUP_BUF_BASE + SETUP_BUF_LEN)
#define EP0_BUF_LEN     EP0_MAX_PKT_SIZE
#define EP1_BUF_BASE    (SETUP_BUF_BASE + SETUP_BUF_LEN)
#define EP1_BUF_LEN     EP1_MAX_PKT_SIZE
#define EP2_BUF_BASE    (EP1_BUF_BASE + EP1_BUF_LEN)
#define EP2_BUF_LEN     EP2_MAX_PKT_SIZE

/* Define the interrupt In EP number */
#define INT_IN_EP_NUM   0x01

/* Define Descriptor information */
#define HID_DEFAULT_INT_IN_INTERVAL     10
#define USBD_SELF_POWERED               0
#define USBD_REMOTE_WAKEUP              0
#define USBD_MAX_POWER                  50  /* The unit is in 2mA. ex: 50 * 2mA = 100mA */

#define LEN_CONFIG_AND_SUBORDINATE  \
    (LEN_CONFIG + LEN_INTERFACE + LEN_HID + LEN_ENDPOINT)

const uint8_t HID_MouseReportDescriptor[] =
{
    0x05, 0x01, /* Usage Page(Generic Desktop Controls) */
    0x09, 0x02, /* Usage(Mouse) */
    0xA1, 0x01, /* Collection(Application) */
    0x09, 0x01, /* Usage(Pointer) */
    0xA1, 0x00, /* Collection(Physical) */
    0x05, 0x09, /* Usage Page(Button) */
    0x19, 0x01, /* Usage Minimum(0x1) */
    0x29, 0x03, /* Usage Maximum(0x3) */
    0x15, 0x00, /* Logical Minimum(0x0) */
    0x25, 0x01, /* Logical Maximum(0x1) */
    0x75, 0x01, /* Report Size(0x1) */
    0x95, 0x03, /* Report Count(0x3) */
    0x81, 0x02, /* Input(3 button bit) */
    0x75, 0x05, /* Report Size(0x5) */
    0x95, 0x01, /* Report Count(0x1) */
    0x81, 0x01, /* Input(5 bit padding) */
    0x05, 0x01, /* Usage Page(Generic Desktop Controls) */
    0x09, 0x30, /* Usage(X) */
    0x09, 0x31, /* Usage(Y) */
    0x09, 0x38, /* Usage(Wheel) */
    0x15, 0x81, /* Logical Minimum(0x81)(-127) */
    0x25, 0x7F, /* Logical Maximum(0x7F)(127) */
    0x75, 0x08, /* Report Size(0x8) */
    0x95, 0x03, /* Report Count(0x3) */
    0x81, 0x06, /* Input(1 byte wheel) */
    0xC0,       /* End Collection */
    0xC0        /* End Collection */
};

const uint8_t gu8DeviceDescriptor[] =
{
    LEN_DEVICE,         /* bLength */
    DESC_DEVICE,        /* bDescriptorType */
    0x10, 0x02,         /* bcdUSB */
    0x00,               /* bDeviceClass */
    0x00,               /* bDeviceSubClass */
    0x00,               /* bDeviceProtocol */
    EP0_MAX_PKT_SIZE,   /* bMaxPacketSize0 */
    /* idVendor */
    (USBD_VID & 0x00FF),
    ((USBD_VID & 0xFF00) >> 8),
    /* idProduct */
    (USBD_PID & 0x00FF),
    ((USBD_PID & 0xFF00) >> 8),
    0x00, 0x00,         /* bcdDevice */
    0x01,               /* iManufacture */
    0x02,               /* iProduct */
    0x00,               /* iSerialNumber - no serial */
    0x01                /* bNumConfigurations */
};

const uint8_t gu8ConfigDescriptor[] =
{
    LEN_CONFIG,     /* bLength */
    DESC_CONFIG,    /* bDescriptorType */
    /* wTotalLength */
    LEN_CONFIG_AND_SUBORDINATE & 0x00FF,
    ((LEN_CONFIG_AND_SUBORDINATE & 0xFF00) >> 8),
    0x01,           /* bNumInterfaces */
    0x01,           /* bConfigurationValue */
    0x00,           /* iConfiguration */
    0x80 | (USBD_SELF_POWERED << 6) | (USBD_REMOTE_WAKEUP << 5),/* bmAttributes */
    USBD_MAX_POWER,         /* MaxPower */
    /* I/F descr: HID */
    LEN_INTERFACE,  /* bLength */
    DESC_INTERFACE, /* bDescriptorType */
    0x00,           /* bInterfaceNumber */
    0x00,           /* bAlternateSetting */
    0x01,           /* bNumEndpoints */
    0x03,           /* bInterfaceClass */
    0x01,           /* bInterfaceSubClass */
    HID_MOUSE,      /* bInterfaceProtocol */
    0x00,           /* iInterface */
    /* HID Descriptor */
    LEN_HID,        /* Size of this descriptor in UINT8s. */
    DESC_HID,       /* HID descriptor type. */
    0x10, 0x01,     /* HID Class Spec. release number. */
    0x00,           /* H/W target country. */
    0x01,           /* Number of HID class descriptors to follow. */
    DESC_HID_RPT,   /* Descriptor type. */
    /* Total length of report descriptor. */
    sizeof(HID_MouseReportDescriptor) & 0x00FF,
    ((sizeof(HID_MouseReportDescriptor) & 0xFF00) >> 8),
    /* EP Descriptor: interrupt in. */
    LEN_ENDPOINT,   /* bLength */
    DESC_ENDPOINT,  /* bDescriptorType */
    (INT_IN_EP_NUM | EP_INPUT), /* bEndpointAddress */
    EP_INT,         /* bmAttributes */
    /* wMaxPacketSize */
    EP2_MAX_PKT_SIZE & 0x00FF,
    ((EP2_MAX_PKT_SIZE & 0xFF00) >> 8),
    HID_DEFAULT_INT_IN_INTERVAL     /* bInterval */
};

const uint8_t gu8StringLangDesc[4] =
{
    4,              /* bLength */
    DESC_STRING,    /* bDescriptorType */
    0x09, 0x04
};

const uint8_t gu8VendorStringDesc[] =
{
    16,
    DESC_STRING,
    'N', 0, 'u', 0, 'v', 0, 'o', 0, 't', 0, 'o', 0, 'n', 0
};

const uint8_t gu8ProductStringDesc[] =
{
    20,
    DESC_STRING,
    'H', 0, 'I', 0, 'D', 0, ' ', 0, 'M', 0, 'o', 0, 'u', 0, 's', 0, 'e', 0
};

const uint8_t gu8BOSDescriptor[] =
{
    LEN_BOS,        /* bLength */
    DESC_BOS,       /* bDescriptorType */
    /* wTotalLength */
    0x0C & 0x00FF,
    ((0x0C & 0xFF00) >> 8),
    0x01,           /* bNumDeviceCaps */
    /* Device Capability */
    LEN_BOSCAP,     /* bLength */
    DESC_CAPABILITY,/* bDescriptorType */
    CAP_USB20_EXT,  /* bDevCapabilityType */
    0x02, 0x00, 0x00, 0x00  /* bmAttributes */
};

const uint8_t *gpu8UsbString[4] =
{
    gu8StringLangDesc,
    gu8VendorStringDesc,
    gu8ProductStringDesc,
    0
};

const uint8_t *gpu8UsbHidReport[3] =
{
    HID_MouseReportDescriptor,
    NULL,
    NULL
};

const uint32_t gu32UsbHidReportLen[3] =
{
    sizeof(HID_MouseReportDescriptor),
    0,
    0
};

const uint32_t gu32ConfigHidDescIdx[3] =
{
    (LEN_CONFIG + LEN_INTERFACE),
    0,
    0
};

const S_USBD_INFO_T gsInfo =
{
    (uint8_t* )gu8DeviceDescriptor,
    (uint8_t* )gu8ConfigDescriptor,
    (uint8_t**)gpu8UsbString,
    (uint8_t**)gpu8UsbHidReport,
    (uint8_t* )gu8BOSDescriptor,
    (uint32_t*)gu32UsbHidReportLen,
    (uint32_t*)gu32ConfigHidDescIdx
};


