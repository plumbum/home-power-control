
PA3  constant BUZZER
PA4  constant OUT1
PA5  constant OUT2
PA6  constant OUT2
PA7  constant OUT2

PB3  constant LED1
PA15 constant LED2
PB1  constant LED3
PB0  constant LED4

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
;

