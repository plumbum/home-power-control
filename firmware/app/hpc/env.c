#include "env.h"

#include <string.h>

#define EEPROM_ADDR (0x50)
#define I2C_TIMEOUT (MS2ST(40))

/* I2C1 */
static const I2CConfig i2cfg1 = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_2,
};



void envInit(void)
{
    i2cInit();

    i2cStart(&I2CD1, &i2cfg1);

    /* tune ports for I2C1 */
    palSetPadMode(IOPORT2, GPIOB_I2C1_SCL, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
    palSetPadMode(IOPORT2, GPIOB_I2C1_SDA, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);

    chThdSleepMilliseconds(100);  /* Just to be safe. */

}


i2cflags_t i2c_errors;

msg_t _eepromReadBlock(void* data, int offset, size_t bytes)
{
    msg_t status = RDY_OK;

    uint8_t addr = EEPROM_ADDR | ((offset/256) & 0x03);
    uint8_t offs = offset & 0xFF;

    status = i2cMasterTransmitTimeout(&I2CD1, addr,
        &offs, 1,
        NULL, 0,
        I2C_TIMEOUT);
    if (status != RDY_OK) { return status; }

    status = i2cMasterReceiveTimeout(&I2CD1, addr,
        data, bytes,
        I2C_TIMEOUT);
    if (status != RDY_OK) { return status; }

    return status;
}

msg_t eepromReadBlock(void* data, int offset, size_t bytes)
{
    msg_t status;
    i2cAcquireBus(&I2CD1);
    status = _eepromReadBlock(data, offset, bytes);
    i2cReleaseBus(&I2CD1);
    if (status != RDY_OK) {
        i2c_errors = i2cGetErrors(&I2CD1);
    }
    return status;
}

msg_t _eepromWritePage(const void* data, int page)
{
    msg_t status, status1;
    uint8_t addr = EEPROM_ADDR | ((page/16) & 0x03);
    uint8_t offset = (page%16) * 16;
    uint8_t buf[17];
    buf[0] = offset;
    memcpy(buf+1, data, 16);

    /* Write block */
    status = i2cMasterTransmitTimeout(&I2CD1, addr,
        buf, 17,
        NULL, 0,
        I2C_TIMEOUT);
    if (status != RDY_OK) { return status; }

    /* Pooling write done */
    do {
        chThdYield();
        status1 = i2cMasterTransmitTimeout(&I2CD1, addr,
            buf, 1,
            NULL, 0,
            I2C_TIMEOUT);
    } while (status1 != RDY_OK);

    return status;
}

/**
 * Write 16 bytes page
 * @param data
 * @param page
 * @return msg_t
 */
msg_t eepromWritePage(const void* data, int page)
{
    msg_t status;
    i2cAcquireBus(&I2CD1);
    status = _eepromWritePage(data, page);
    i2cReleaseBus(&I2CD1);
    if (status != RDY_OK) {
        i2c_errors = i2cGetErrors(&I2CD1);
    }
    return status;
}

