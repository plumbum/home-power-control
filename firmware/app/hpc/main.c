/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

#include "usb_cdc.h"
#include "vshell.h"
#include "env.h"

#include "chprintf.h"

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

/*
 * Red LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg)
{

    (void)arg;
    chRegSetThreadName("blinker");
    while (TRUE) {
        if(usbGetDriver()->state == USB_ACTIVE) {
            palSetPad(IOPORT2, GPIOB_LED1);
            chThdSleepMilliseconds(20);
            palClearPad(IOPORT2, GPIOB_LED1);
            chThdSleepMilliseconds(100);
        }
        palSetPad(IOPORT2, GPIOB_LED1);
        chThdSleepMilliseconds(20);
        palClearPad(IOPORT2, GPIOB_LED1);
        chThdSleepMilliseconds(1000);
        /* systime_t time = usbGetDriver()->state == USB_ACTIVE ? 250 : 500;
        palClearPad(IOPORT1, GPIOA_LED2);
        chThdSleepMilliseconds(time);
        palSetPad(IOPORT1, GPIOA_LED2);
        chThdSleepMilliseconds(time); */
    }
    return 0;
}

static WORKING_AREA(waResponce, 1024);
static msg_t psResponce(void *arg)
{
    (void)arg;
    while(TRUE) {
        int c = sdGet(&SD1);
        if(c >= 0) {
            chnWrite(&SDU1, (uint8_t*)(&c), 1);
        }
    }
    return 0;
}

static const SerialConfig sd1_config =
{
    115200,
    0,
    USART_CR2_STOP1_BITS | USART_CR2_LINEN,
    0
};

/*
 * Application entry point.
 */
int main(void)
{

    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the
     *   RTOS is active.
     */
    halInit();
    chSysInit();

    envInit();

    usbcdcInit();

    vshellInit((BaseSequentialStream*)&SDU1);

    sdStart(&SD1, &sd1_config);

    /*
     * Creates the blinker thread.
     */
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
    chThdCreateStatic(waResponce, sizeof(waResponce), NORMALPRIO, psResponce, NULL);

    /*
     * Normal main() thread activity, in this demo it does nothing except
     * sleeping in a loop and check the button state.
     */
    while (TRUE) {
        vshellRestart(usbGetDriver()->state == USB_ACTIVE);
        chThdSleepMilliseconds(1000);
    }
}

