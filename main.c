/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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

#include "esh.h"
#include "logstream.h"
#include "led.h"

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");

  ledSetDelay(1500);

  while (true) {
    XPRINTF("Blinking LED\r\n");
    ledOn();
    chThdSleepMilliseconds(ledGetDelay());
    ledOff();
    chThdSleepMilliseconds(ledGetDelay());
  }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  // Initilize the embedded shell for debugging
  eshInit();

  XPRINTF("ChibiOS Shell Demo Application \n\r");

  /*
   * Creates the example thread.
   */
  XPRINTF("Blinker Thread Created \n\r");
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);


  // Do nothing in the main loop - we will add to this
  while (true) {
    eshService();
    chThdSleepMilliseconds(500);
  }
}
