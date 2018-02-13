\ Real time clock, based on LSI
\ no calendar functions, just a 32-bit counter incrementing once a second

$40021000 constant RCC
    RCC $1C + constant RCC-APB1ENR
    RCC $20 + constant RCC-BDCR
    RCC $24 + constant RCC-CSR

$40007000 constant PWR
     PWR $0 + constant PWR-CR

$40002800 constant RTC
     RTC $00 + constant RTC-CRH
     RTC $04 + constant RTC-CRL
     RTC $0C + constant RTC-PRLL
     RTC $18 + constant RTC-CNTH
     RTC $1C + constant RTC-CNTL

$E000E100 constant NVIC-EN0R \ IRQ  0 to 31 Set Enable Register
$E000E104 constant NVIC-EN1R \ IRQ 32 to 63 Set Enable Register

: bit ( u -- u )  \ turn a bit position into a single-bit mask
  1 swap lshift  1-foldable ;

: rtc-wait ( -- ) begin 1 bit RCC-CSR bit@ until ;

: rtc-init ( -- )  \ restart internal RTC using 37KHz LSI
  %11 27 lshift RCC-APB1ENR bis!   \ enable PWREN and BKPEN
               8 bit PWR-CR bis!   \ disable backup domain write protection
\           16 bit RCC-BDCR bis!   \ reset backup domain
            16 bit RCC-BDCR bic!   \ releases backup domain
                 1 RCC-CSR bis!   \ LSION
                   rtc-wait
      %10 8 lshift RCC-BDCR bis!   \ RTCSEL = LSI
            15 bit RCC-BDCR bis!   \ RTCEN
       begin 3 bit RTC-CRL hbit@ until  \ wait RSF
                   rtc-wait
             4 bit RTC-CRL bis!   \ set CNF
             36999 RTC-PRLL h!     \ set PRLL for 37 KHz LSI
             4 bit RTC-CRL bic!   \ clear CNF
                   rtc-wait
;

: now! ( u -- )  \ set current time
            rtc-wait
       4 bit RTC-CRL bis!
        dup RTC-CNTL h!
  16 rshift RTC-CNTH h!
       4 bit RTC-CRL hbic! ;

: now ( -- u )  \ return current time in seconds
\ use a spinloop to read consistent CNTL + CNTH values
  0 0  begin  2drop  RTC-CNTL h@ RTC-CNTH h@  over RTC-CNTL h@ = until
  16 lshift or ;


0 variable sec-cnt

: rtc-int
    1 sec-cnt +!
    1 RTC-CRL bic! \ Reset SECF
;

: init
    rtc-init
    ['] rtc-int irq-rtc !
    \ ['] rtc-int irq-rtcalarm !
    3 bit NVIC-EN0R bis!  \ enable RTC interrupt 3
    begin 5 bit RTC-CRL hbit@ until  \ wait RTOFF
    4 bit RTC-CRL bis!   \ set CNF
    1 RTC-CRH bis! \ Enable SECIE 
    4 bit RTC-CRL bic!   \ clear CNF
    begin 5 bit RTC-CRL hbit@ until  \ wait RTOFF
;

: cnt. sec-cnt @ . ;

