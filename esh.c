#include "esh.h"

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"

#include "usbcfg.h"

static void cmd_debug(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: debug\r\n");
    return;
  }

  chprintf(chp, "\r\nSystem debug display\r\n\r\n");

  //TODO: Implement this debug output
}

static const ShellCommand commands[] = {
                                        {"debug", cmd_debug},
                                        {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
                                       (BaseSequentialStream *)&SDU1,
                                       commands
};

/*
** Initialize our esh (embedded shell)
 */
void eshInit(void)
{
  // Init the serial driver over usb
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */

  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  // Shell manager initialization
  shellInit();
}

/*
** This should be called periodicly from a low priority thread to spawn a new shell if required or clean up if the shell has exited
 */
void eshService(void)
{
  static thread_t *shelltp = NULL;

  // if the usb is active and the shell is not active, spawn a thread
  if (!shelltp && SDU1.config->usbp->state == USB_ACTIVE && shelltp == NULL)
  {
    palSetPad(GPIOD, GPIOD_LED5);       /* Red.  */
    shelltp = chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(128),
                                            "shell", NORMALPRIO + 1,
                                            shellThread, (void *)&shell_cfg1);
  }
  else if(chThdTerminatedX(shelltp))
  {
    palClearPad(GPIOD, GPIOD_LED5);       /* Red.  */
    chThdRelease(shelltp);
    shelltp = NULL;
  }
}
