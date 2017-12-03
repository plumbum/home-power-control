OMODE-PP PA4 io-mode!
OMODE-PP PA5 io-mode!
OMODE-PP PA6 io-mode!
OMODE-PP PA7 io-mode!

OMODE-PP PA15 io-mode!
OMODE-PP PB0 io-mode!
OMODE-PP PB1 io-mode!
OMODE-PP PB3 io-mode!

PA15 ios!
PB0  ios!
PB1  ios!
PB3  ios!

i2c-init
\ i2c-fast

i2c.

$50 i2c-addr
    0 >i2c 0 >i2c
    1 >i2c 2 >i2c
    3 >i2c 4 >i2c
    5 >i2c 6 >i2c
    7 >i2c 8 >i2c
    0 i2c-xfer .

$50 i2c-addr
    0 >i2c 0 >i2c
    4 i2c-xfer .
    i2c> hex.
    i2c> hex.
    i2c> hex.
    i2c> hex.


