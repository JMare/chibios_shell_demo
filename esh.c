#include "ch.h"
#include "hal.h"
#include "shell.h"
#include "chprintf.h"
#include "usbcfg.h"

#include "logstream.h"

// This defines how often the logstream command will stop to check for a keypress
#define CMD_LOGSTREAM_TIMEOUT_MS 50

static void cmd_logstream(BaseSequentialStream *chp, int argc, char *argv[])
{
  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: logstream\r\n");
  return;
  }

  chprintf(chp, "\r\nlogstream: system log output\r\n\r\n");

  // While no key is pressed
  while(chnGetTimeout((BaseChannel *)chp,TIME_IMMEDIATE) == Q_TIMEOUT)
  {
    char *s = logGetString(chTimeMS2I(CMD_LOGSTREAM_TIMEOUT_MS));
    if(s != NULL)
    {
      chprintf(chp,s);
      logReturnString(s);
    }
  }
}

static void cmd_example(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: example\r\n");
    return;
  }

  chprintf(chp, "Example shell command\r\n");
}

static const ShellCommand commands[] = {
                                        {"logstream", cmd_logstream},
                                        {"example", cmd_example},
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

  logFIFOInit();

  /*
   * If the board was reset while connected to the commputer
   * this will force a reconnect without needing to replug the cable*/
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  // Shell manager initialization
  shellInit();
}

/*
** This should be called periodicly from a low priority thread to spawn a new shell thread if required or clean up if the shell has exited
 */
void eshService(void)
{
  static thread_t *shelltp = NULL;

  // if the usb is active and the shell is not active, spawn a thread
  if (!shelltp && SDU1.config->usbp->state == USB_ACTIVE && shelltp == NULL)
  {
    palSetPad(GPIOD, GPIOD_LED5); // Red light to indicate shell is active
    shelltp = chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(2048),
                                            "shell", NORMALPRIO + 1,
                                            shellThread, (void *)&shell_cfg1);
  }
  else if(chThdTerminatedX(shelltp))
  {
    palClearPad(GPIOD, GPIOD_LED5); // Turn off shell indicator light
    chThdRelease(shelltp);
    shelltp = NULL;
  }
}
