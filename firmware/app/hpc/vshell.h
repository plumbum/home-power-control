#ifndef _VSHELL_H_
#define _VSHELL_H_

#include <ch.h>

void vshellInit(BaseSequentialStream *stream);
void vshellRestart(bool_t active);

#endif /* _VSHELL_H_ */

