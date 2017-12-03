
$4000 eraseflashfrom  \ need to start off with a clean Mecrisp image
compiletoflash

4  constant io-ports  \ A..D
50 constant I2C.DELAY

include flib/mecrisp/calltrace.fs
include flib/mecrisp/cond.fs
include flib/mecrisp/hexdump.fs
include flib/stm32f1/io.fs
include flib/pkg/pins48.fs
include flib/stm32f1/hal.fs
include flib/stm32f1/spi.fs
include flib/stm32f1/timer.fs
include flib/stm32f1/pwm.fs
include flib/stm32f1/adc.fs
include flib/stm32f1/rtc.fs
\ include flib/any/i2c-bb.fs
include flib/any/ring.fs
include flib/stm32f1/i2c.fs

\ : chipid ( -- u1 u2 u3 3 )  \ unique chip ID as N values on the stack
\   $1FFFF7E8 @ $1FFFF7EC @ $1FFFF7F0 @ 3 ;
\ : hwid ( -- u )  \ a "fairly unique" hardware ID as single 32-bit int
\   chipid 1 do xor loop ;
\ : flash-kb ( -- u )  \ return size of flash memory in KB
\   $1FFFF7E0 h@ ;
\ : flash-pagesize ( addr - u )  \ return size of flash page at given address
\   drop flash-kb 128 <= if 1024 else 2048 then ;

\ STM32F100 VLDiscovery 
\ 48MHz fcpu settings
\ 2x overclocked with 0 waitstates
\ uart 115k2
\ may not work with your chip reliably
\ by Igor de om1zz, 2015
\ no warranties of any kind

$40021000 constant RCC_Base

$40013808 constant USART1_BRR

RCC_Base $00 + constant RCC_CR
  1 24 lshift constant PLLON
  1 25 lshift constant PLLRDY
  1 16 lshift constant HSEON
  1 17 lshift constant HSERDY

RCC_Base $04 + constant RCC_PLLCFGR
   1 16 lshift constant PLLSRC

: 48MHz ( -- )
  HSEON RCC_CR bis!   \ switch HSE ON
    \ Wait for HSE to be ready
  begin HSERDY RCC_CR bit@ until 
  PLLSRC            \ HSE clock is 8 MHz Xtal source
  4  18 lshift or  \ PLL multiplication factor
                    \ 8 MHz * 6 = 48 MHz = HCLK
  4  11 lshift or  \ PCLK2 = HCLK/2
  4  08 lshift or  \ PCLK1 = HCLK/2		    
  1  14 lshift or  \ ADCPRE = PCLK2/4
  2  or  \ PLL is the system clock
  RCC_PLLCFGR !
  PLLON RCC_CR bis!   \ switch PLL ON
  \ Wait for PLL to lock:
  begin PLLRDY RCC_CR bit@ until  
  48000000 clock-hz ! 
  $0d0 USART1_BRR ! \ Set Baud rate divider for 115200 Baud at PCLK2=24MHz (13.02)
;

: freemem ( -- ) flash-kb . ." KB <stm32f100> " hwid hex.
  $10000 compiletoflash here -  flashvar-here compiletoram here -
  ." ram/flash: " . . ." free " ;

: init ( -- )  \ board initialisation
  \ init  \ uses new uart init convention
  ['] ct-irq irq-fault !  \ show call trace in unhandled exceptions
  jtag-deinit  \ disable JTAG, we only need SWD
  48MHz
  1000 systick-hz
  freemem ." ok." cr
;

: cornerstone ( "name" -- )  \ define a flash memory cornerstone
  <builds begin here dup flash-pagesize 1- and while 0 h, repeat
  does>   begin dup  dup flash-pagesize 1- and while 2+   repeat  cr
  eraseflashfrom ;


( always end: ) here hex.
cornerstone eraseflash
compiletoram

