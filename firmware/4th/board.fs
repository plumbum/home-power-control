
PA3  constant BUZZER
PA4  constant OUT1
PA5  constant OUT2
PA6  constant OUT3
PA7  constant OUT4

PB3  constant LED4
PA15 constant LED3
PB0  constant LED2
PB1  constant LED1

PB8  constant BUTTON1
PB9  constant BUTTON2

: init
    init
    OUT1 dup ioc! OMODE-PP swap io-mode!
    OUT2 dup ioc! OMODE-PP swap io-mode!
    OUT3 dup ioc! OMODE-PP swap io-mode!
    OUT4 dup ioc! OMODE-PP swap io-mode!
    LED1 dup ioc! OMODE-PP swap io-mode!
    LED2 dup ioc! OMODE-PP swap io-mode!
    LED3 dup ioc! OMODE-PP swap io-mode!
    LED4 dup ioc! OMODE-PP swap io-mode!
    BUTTON1 dup ios! IMODE-PULL swap io-mode!
    BUTTON2 dup ios! IMODE-PULL swap io-mode!
;

