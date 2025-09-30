/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Lab 4 Main Code
Author  : Wava Chan
Email   : wchan@g.hmc.edu
Date    : September 2025

*/

#include <stdio.h>
#include <stdint.h>
#include "lib/STM32L432KC_FLASH.h"
#include "lib/STM32L432KC_GPIO.h"
#include "lib/STM32L432KC_RCC.h"
#include "STM32L432KC_TIMER.h"

/*
LAB 4 STARTER CODE-- provided by E155
Tune: Fur Elise
*/

// Pitch in Hz, duration in ms
const int fur_elise[][2] = {
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	250},
{  0,	125},
{494,	125},
{523,	125},
{587,	125},
{659,	375},
{392,	125},
{699,	125},
{659,	125},
{587,	375},
{349,	125},
{659,	125},
{587,	125},
{523,	375},
{330,	125},
{587,	125},
{523,	125},
{494,	250},
{  0,	125},
{330,	125},
{659,	125},
{  0,	250},
{659,	125},
{1319,	125},
{  0,	250},
{623,	125},
{659,	125},
{  0,	250},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	500},
{  0,	0}};

#define PIN 8 //PA2. Listed alternate functions TIM15_CH1. A7


/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void) {

  configureFlash(); //set up flash memory 
  configureClock(); //set up PLL clock 

  RCC->APB2ENR |= (1 << 17); //enable TIM16 clock. pg 228
  RCC->APB2ENR |= (1 << 16); //enable TIM15 clock. pg 228
  RCC->AHB2ENR |= (1 << 0); //enable GPIOA clk pg. 223

  enableTimer15();
  enableTimer16();
   
  //configure pins
  pinMode(PIN, GPIO_ALT);
  GPIO->AFRL |= (0b1110 << 8); //connect pin A7/PA2 I/O to AF14 (TIM15_CH1) in GPIOx_AFRL register

  int size = sizeof(fur_elise)/sizeof(fur_elise[0]); //loop thru each line in fur_elise
  for(int i = 0; i < size; i = i + 1) {
    pitch_set(fur_elise[i][0]); //write frequency to timer 15
    delay_millis(fur_elise[i][1]); // delay by # of milliseconds
  }

  // play jackie and wilson

}

/*************************** End of file ****************************/
/*
TODO: my questions
output capture module. describe in pages abt TIm15/16. pWM mode
3. where is my clock (CK_INT) coming from? is it confirguable inside TIM15/16? Do I configure it through RCC instead? Is CK_INT the same as SYS_CLK? 
  CK_INT is SYS_CLK put through 2 prescalers
4. what frequency is CK_INT? Is there another way i should be calculating my PSC and ARR? 
5. check that wave is what you expect before building speaker
6. Where is my power supply coming from?
7. does my order matter?
*/ 