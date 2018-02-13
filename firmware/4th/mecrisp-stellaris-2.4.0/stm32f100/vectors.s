@
@    Mecrisp-Stellaris - A native code Forth implementation for ARM-Cortex M microcontrollers
@    Copyright (C) 2013  Matthias Koch
@
@    This program is free software: you can redistribute it and/or modify
@    it under the terms of the GNU General Public License as published by
@    the Free Software Foundation, either version 3 of the License, or
@    (at your option) any later version.
@
@    This program is distributed in the hope that it will be useful,
@    but WITHOUT ANY WARRANTY; without even the implied warranty of
@    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@    GNU General Public License for more details.
@
@    You should have received a copy of the GNU General Public License
@    along with this program.  If not, see <http://www.gnu.org/licenses/>.
@

@ -----------------------------------------------------------------------------
@ Interruptvektortabelle
@ -----------------------------------------------------------------------------

.include "../common/vectors-common.s"

@ Special interrupt handlers for this particular chip:

.word nullhandler+1 @ Position  0: Window Watchdog
.word nullhandler+1 @ Position  1: PVD through EXTI line detection
.word nullhandler+1 @ Position  2: Tamper and TimeStamp through EXTI line
.word irq_vektor_rtc+1 @ Position  3: RTC Wakeup
.word nullhandler+1 @ Position  4: Flash
.word nullhandler+1 @ Position  5: RCC
.word irq_vektor_exti0+1 @ Position  6: EXTI Line 0
.word irq_vektor_exti1+1 @ Position  7: EXTI Line 1
.word irq_vektor_exti2+1 @ Position  8: EXTI Line 2
.word irq_vektor_exti3+1 @ Position  9: EXTI Line 3
.word irq_vektor_exti4+1 @ Position 10: EXTI Line 4
.word nullhandler+1 @ Position 11: DMA1 Stream 0
.word nullhandler+1 @ Position 12: DMA1 Stream 1
.word nullhandler+1 @ Position 13: DMA1 Stream 2
.word nullhandler+1 @ Position 14: DMA1 Stream 3
.word nullhandler+1 @ Position 15: DMA1 Stream 4
.word nullhandler+1 @ Position 16: DMA1 Stream 5
.word nullhandler+1 @ Position 17: DMA1 Stream 6
.word irq_vektor_adc+1 @ Position 18: ADC global interrupts

.word 0  @ 19: Reserved
.word 0  @ 20: Reserved
.word 0  @ 21: Reserved
.word 0  @ 22: Reserved

.word nullhandler+1 @ Position 23: EXTI9_5
.word nullhandler+1 @ Position 24: TIM1_BRK
.word nullhandler+1 @ Position 25: TIM1_UP
.word nullhandler+1 @ Position 26: TIM1_TRG_COM
.word nullhandler+1 @ Position 27: TIM1_CC
.word nullhandler+1 @ Position 28: TIM2
.word nullhandler+1 @ Position 29: TIM3
.word nullhandler+1 @ Position 30: TIM4
.word nullhandler+1 @ Position 31: I2C1_EV
.word nullhandler+1 @ Position 32: I2C1_ER
.word nullhandler+1 @ Position 33: I2C2_EV
.word nullhandler+1 @ Position 34: I2C2_ER
.word nullhandler+1 @ Position 35: SPI1
.word nullhandler+1 @ Position 36: SPI2
.word nullhandler+1 @ Position 37: USART1
.word nullhandler+1 @ Position 38: USART2
.word nullhandler+1 @ Position 39: USART3
.word nullhandler+1 @ Position 40: EXTI15_10
.word irq_vektor_rtcalarm+1 @ Position 41: RTCAlarm
.word nullhandler+1 @ Position 42: CEC global interrupt
.word nullhandler+1 @ Position 43: TIM12
.word nullhandler+1 @ Position 44: TIM13
.word nullhandler+1 @ Position 45: TIM14

.word 0  @ 46: Reserved
.word 0  @ 47: Reserved
.word nullhandler+1 @ Position 48: FSMC
.word 0  @ 49: Reserved

.word nullhandler+1 @ Position 50: TIM5
.word nullhandler+1 @ Position 51: SPI3
.word nullhandler+1 @ Position 52: UART4
.word nullhandler+1 @ Position 53: UART5
.word nullhandler+1 @ Position 54: TIM6_DAC
.word nullhandler+1 @ Position 55: TIM7
.word nullhandler+1 @ Position 56: DMA2_Channel1
.word nullhandler+1 @ Position 57: DMA2_Channel2
.word nullhandler+1 @ Position 58: DMA2_Channel3
.word nullhandler+1 @ Position 59: DMA2_Channel4
.word nullhandler+1 @ Position 60: DMA2_Channel5

@ -----------------------------------------------------------------------------
