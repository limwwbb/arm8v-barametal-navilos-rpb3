/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

#include "HalUart.h"
#include "HalTimer.h"
#include "mbox.h"

static void Hw_init(void)
{
    // set up serial console
    Hal_uart_init();
    Hal_timer_init();
}
static void Printf_test(void)
{
    char* str = "printf pointer test";
    char* nullptr = 0;
    uint32_t i = 5;
    uint32_t* sysctrl0 = (uint32_t*)0x10001000;

    debug_printf("%s\n", "Hello printf");
    debug_printf("output string pointer: %s\n", str);
    debug_printf("%s is null pointer, %u number\n", nullptr, 10);
    debug_printf("%u = 5\n", i);
    debug_printf("dec=%u hex=%x\n", 0xff, 0xff);
    debug_printf("print zero %u\n", 0);
    debug_printf("SYSCTRL0 %x\n", *sysctrl0);
}

static void Timer_test(void)
{
    for(uint32_t i = 0; i < 5 ; i++)
    {
        debug_printf("current count : %u\n", Hal_timer_get_1ms_counter());
        delay(1000);
    }
}

void main()
{

    Hw_init();
    
    // get the board's unique serial number with a mailbox call
    mbox[0] = 8*4;                  // length of the message
    mbox[1] = MBOX_REQUEST;         // this is a request message
    
    mbox[2] = MBOX_TAG_GETSERIAL;   // get serial number command
    mbox[3] = 8;                    // buffer size
    mbox[4] = 8;
    mbox[5] = 0;                    // clear output buffer
    mbox[6] = 0;

    mbox[7] = MBOX_TAG_LAST;

    // send the message to the GPU and receive answer
    if (mbox_call(MBOX_CH_PROP)) {
        Hal_uart_put_char('G');
        uart_hex(mbox[6]);
        uart_hex(mbox[5]);
        Hal_uart_put_char('\n');
    } else {
        Hal_uart_put_char('N');
    }
    putstr("Hello World!\n");
    Printf_test();
    Timer_test();

// echo everything back
    while(1) {
        uart_send(uart_getc());
    }


}
