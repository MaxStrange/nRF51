#ifndef _PROJECT_H_
#define _PROJECT_H_
/*
This file is used to set up meta project data such as
whether the build configuration.
*/

#define CONFIGURATION_DEBUG 1
#define CONFIGURATION_RELEASE 0


/*
Peripherals in use by which modules:

NRF_CLOCK   - low frequency clock - used by my_clock
NRF_TIMER0  - timer 0             - used by my_led for pwm
NRF_TIMER1  - timer 1             - used by range_finder
NRF_TIMER2  - timer 2             - used by servo for pwm
NRF_RTC0    - real time counter 0 - used by my_timer
NRF_UART0   - uart 0              - used by my_uart
NRF_GPIOTE  - including interrupt - used by range_finder
*/

/*
Pin mappings

P0.01 LCD
P0.02 LCD
P0.03 LCD
P0.04 LCD
P0.05 LCD
P0.06 LCD
P0.07
P0.08 --Not covered by shield
P0.09 UART TXD
P0.10 --Not covered by shield
P0.11 UART RXD
P0.12 Range finder
P0.13 Range finder
P0.14 Confirm button
P0.15 Select button
P0.16 Bumper
P0.17 Button 1
P0.18 Button 2
P0.19 Button 3
P0.20 Button 4
P0.21 LED1
P0.22 LED2
P0.23 LED3
P0.24 LED4
P0.25 Motor
P0.26 --Not covered by shield
P0.27 --Not covered by shield
P0.28 Motor
P0.29
P0.30
*/

//Interrupt table!

/*
// External Interrupts
  .long   POWER_CLOCK_IRQHandler
  .long   RADIO_IRQHandler
  .long   UART0_IRQHandler
  .long   SPI0_TWI0_IRQHandler
  .long   SPI1_TWI1_IRQHandler
  .long   0                         // Reserved
  .long   GPIOTE_IRQHandler
  .long   ADC_IRQHandler
  .long   TIMER0_IRQHandler
  .long   TIMER1_IRQHandler
  .long   TIMER2_IRQHandler
  .long   RTC0_IRQHandler
  .long   TEMP_IRQHandler
  .long   RNG_IRQHandler
  .long   ECB_IRQHandler
  .long   CCM_AAR_IRQHandler
  .long   WDT_IRQHandler
  .long   RTC1_IRQHandler
  .long   QDEC_IRQHandler
  .long   LPCOMP_IRQHandler
  .long   SWI0_IRQHandler
  .long   SWI1_IRQHandler
  .long   SWI2_IRQHandler
  .long   SWI3_IRQHandler
  .long   SWI4_IRQHandler
  .long   SWI5_IRQHandler
*/
#endif //_PROJECT_H_
