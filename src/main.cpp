#include "sys/sys_init.h"
#include "sys/uart_init.h"
#include "sys/hid_mouse.h"

#include <stdio.h>

int main()
{
    SYS_Init();

    UART_Init();

    HID_Init();

    printf("TEST\n");

    while (1)
    {
        // HID_Process();
    }
}