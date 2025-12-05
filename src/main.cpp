#include "sys/sys_init.h"
#include "sys/uart_init.h"
#include "sys/hid_mouse.h"

#include <stdio.h>

int main()
{
    SYS_Init();

    UART_Init();
    printf("Uart Init!\n");

    HID_Init();
    printf("HID Init!\n");

    while (1)
    {
        // HID_Process();
    }
}