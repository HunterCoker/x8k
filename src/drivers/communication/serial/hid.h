#pragma once

#include <stdint.h>

/* Define the vendor id and product id */
#define HID_VENDOR_ID   0x0416
#define HID_PRODUCT_ID  0x0000

#define HID_MOUSE       0x02

/* Define HID Class Specific Request */
#define HID_REQUEST_GET_REPORT      0x01
#define HID_REQUEST_GET_IDLE        0x02
#define HID_REQUEST_GET_PROTOCOL    0x03
#define HID_REQUEST_SET_REPORT      0x09
#define HID_REQUEST_SET_IDLE        0x0A
#define HID_REQUEST_SET_PROTOCOL    0x0B

/* USB HID Class Report Type */
#define HID_REPORT_TYPE_INPUT   0x01
#define HID_REPORT_TYPE_OUTPUT  0x02
#define HID_REPORT_TYPE_FEATURE 0x03

/* USB HID Protocol Type */
#define HID_BOOT_PROTOCOL   0x00
#define HID_REPORT_PROTOCOL 0x01

/*
 */
struct HIDReport
{
    uint16_t dx;
    uint16_t dy;
    uint8_t wheel;
    uint8_t buttons;
};