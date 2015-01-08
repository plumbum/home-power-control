#ifndef _USB_CDC_H_
#define _USB_CDC_H_

#include <ch.h>
#include <hal.h>

extern SerialUSBDriver SDU1;

void usbcdcInit(void);
USBDriver* usbGetDriver(void);

#endif /* _USB_CDC_H_ */

