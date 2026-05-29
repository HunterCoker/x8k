#ifndef __HID_MOUSE_H__
#define __HID_MOUSE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Define the vendor id and product id */
#define USBD_VID    0x0416
#define USBD_PID    0x8249

/* Define HID Class Specific Request */
#define GET_REPORT      0x01
#define GET_IDLE        0x02
#define GET_PROTOCOL    0x03
#define SET_REPORT      0x09
#define SET_IDLE        0x0A
#define SET_PROTOCOL    0x0B

/* USB HID Interface Class protocol */
#define HID_NONE        0x00
#define HID_KEYBOARD    0x01
#define HID_MOUSE       0x02

/* USB HID Class Report Type */
#define HID_RPT_TYPE_INPUT      0x01
#define HID_RPT_TYPE_OUTPUT     0x02
#define HID_RPT_TYPE_FEATURE    0x03

/* USB HID Protocol Type */
#define HID_BOOT_PROTOCOL   0x00
#define HID_REPORT_PROTOCOL 0x01

/* Define EP maximum packet size */
#define CEP_MAX_PKT_SIZE        64
#define CEP_OTHER_MAX_PKT_SIZE  64
#define EPA_MAX_PKT_SIZE        64
#define EPA_OTHER_MAX_PKT_SIZE  64
#define EPB_MAX_PKT_SIZE        64
#define EPB_OTHER_MAX_PKT_SIZE  64

/* Define EP buffer addresses */
#define CEP_BUF_BASE    0x00
#define CEP_BUF_LEN     CEP_MAX_PKT_SIZE
#define EPA_BUF_BASE    (CEP_BUF_BASE + CEP_BUF_LEN)
#define EPA_BUF_LEN     EPA_MAX_PKT_SIZE
#define EPB_BUF_BASE    (EPA_BUF_BASE + EPA_BUF_LEN)
#define EPB_BUF_LEN     EPB_MAX_PKT_SIZE

/* Define the interrupt IN and OUT EP number */
#define INT_IN_EP_NUM   0x01
#define INT_OUT_EP_NUM  0x02

/* Define Descriptor information */
#define HID_DEFAULT_INT_IN_INTERVAL 10
#define USBD_SELF_POWERED           0
#define USBD_REMOTE_WAKEUP          1
#define USBD_MAX_POWER              50  /* The unit is in 2mA. ex: 50 * 2mA = 100mA */

#define LEN_CONFIG_AND_SUBORDINATE \
    (LEN_CONFIG + LEN_INTERFACE + LEN_HID + LEN_ENDPOINT)

#define MOUSE_BUTTON_LEFT  (0x01 << 0)
#define MOUSE_BUTTON_RIGHT (0x01 << 1)

typedef struct {
    uint8_t buttons;
    int8_t  dx;
    int8_t  dy;
    int8_t  wheel;
} __attribute__((packed)) mouse_report_t;

void HID_Init(void);
void HID_ClassRequest(void);

void EPA_Handler(void);
void EPB_Handler(void);

void HID_Update(int16_t dx, int16_t dy);

#ifdef __cplusplus
}
#endif

#endif  /* __HID_MOUSE_H__ */