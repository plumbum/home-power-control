#include "vshell.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <hal.h>

#include "shell.h"
#include "chprintf.h"


static Thread *shelltp = NULL;

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WA_SIZE(2048)
#define TEST_WA_SIZE    THD_WA_SIZE(256)

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
    size_t n, size;

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: mem\r\n");
        return;
    }
    n = chHeapStatus(NULL, &size);
    chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
    chprintf(chp, "heap fragments   : %u\r\n", n);
    chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
    static const char *states[] = {THD_STATE_NAMES};
    Thread *tp;

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: threads\r\n");
        return;
    }
    chprintf(chp, "    addr    stack prio refs     state time\r\n");
    tp = chRegFirstThread();
    do {
        chprintf(chp, "%.8lx %.8lx %4lu %4lu %9s %lu\r\n",
                 (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
                 (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
                 states[tp->p_state], (uint32_t)tp->p_time);
        tp = chRegNextThread(tp);
    } while (tp != NULL);
}

static void usage_on(BaseSequentialStream *chp)
{
    chprintf(chp, "Usage: on <1-4>\r\n");
}

static void cmd_on(BaseSequentialStream *chp, int argc, char *argv[])
{
    if (argc != 1) {
        usage_on(chp);
        return;
    }
    int ch = atoi(argv[0]);
    switch(ch) {
        case 1: palSetPad(IOPORT1, GPIOA_OUT1); break;
        case 2: palSetPad(IOPORT1, GPIOA_OUT2); break;
        case 3: palSetPad(IOPORT1, GPIOA_OUT3); break;
        case 4: palSetPad(IOPORT1, GPIOA_OUT4); break;
        default: usage_on(chp); return;
    }
}

static void usage_off(BaseSequentialStream *chp)
{
    chprintf(chp, "Usage: off <1-4>\r\n");
}

static void cmd_off(BaseSequentialStream *chp, int argc, char *argv[])
{
    if (argc == 0) {
        palClearPad(IOPORT1, GPIOA_OUT1);
        palClearPad(IOPORT1, GPIOA_OUT2);
        palClearPad(IOPORT1, GPIOA_OUT3);
        palClearPad(IOPORT1, GPIOA_OUT4);
        return;
    }
    if (argc != 1) {
        usage_off(chp);
        return;
    }
    int ch = atoi(argv[0]);
    switch(ch) {
        case 1: palClearPad(IOPORT1, GPIOA_OUT1); break;
        case 2: palClearPad(IOPORT1, GPIOA_OUT2); break;
        case 3: palClearPad(IOPORT1, GPIOA_OUT3); break;
        case 4: palClearPad(IOPORT1, GPIOA_OUT4); break;
        default: usage_off(chp); return;
    }
}

static void cmd_write(BaseSequentialStream *chp, int argc, char *argv[]) {
    static uint8_t buf[] =
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: write\r\n");
        return;
    }

    while (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == Q_TIMEOUT) {
        chSequentialStreamWrite(chp, buf, sizeof buf - 1);
    }
    chprintf(chp, "\r\n\nstopped\r\n");
}

static const ShellCommand commands[] = {
    {"on", cmd_on},
    {"off", cmd_off},
    {"mem", cmd_mem},
    {"threads", cmd_threads},
    {"write", cmd_write},
    {NULL, NULL}
};

static ShellConfig shell_cfg1;

/* Public function */
void vshellInit(BaseSequentialStream *stream)
{
    shell_cfg1.sc_channel = stream;
    shell_cfg1.sc_commands = commands;
    /*
     * Shell manager initialization.
     */
    shellInit();

}

void vshellRestart(bool_t active)
{
    if (!shelltp && active) {
        shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
    } else if (chThdTerminated(shelltp)) {
        chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
        shelltp = NULL;           /* Triggers spawning of a new shell.        */
    }
}

