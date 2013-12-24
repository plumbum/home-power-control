#ifndef _USB_CDC_H_
#define _USB_CDC_H_

#include <ch.h>
#include <hal.h>

#include "shell.h"

extern SerialUSBDriver SDU1;

void usbcdcInit(void);

void usbcdcRespawn(void);

#endif /* _USB_CDC_H_ */

