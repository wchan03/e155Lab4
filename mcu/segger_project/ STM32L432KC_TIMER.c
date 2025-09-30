// STM32L432KC_TIMER.c
// Source code for timer functions

#include "STM32L432KC_TIMER.h"
//#include "math.h"

void enableTimer16(void) { //used for delay

  // configure counter 
  TIMER16->TIMx_CR1 |= (1 << 7); //ARR is enabled
  TIMER16->TIMx_CR1 |= (1 << 0); //counter enabled CEN
  TIMER16->TIMx_CNT = 0; //set counter to 0

  // prescale register
  TIMER16->TIMx_PSC = 39; //TODO: set prescaler to 80. 100kHz clock?

  //set ARR to max value
  //TIMER16->TIMx_ARR = 0xffff;

  //update generation
  TIMER16->TIMx_EGR |= (1 << 0);
}

void enableTimer15(void){  //Pulse Width Modulation mode enabled

  //TODO: set clock?

  ///disable slave select???
  TIMER15->TIMx_CR1 |= (1 << 7); //auto-reload preload reg. by setting ARPE bit in TIMx_CR1 reg.
  TIMER15->TIMx_CR1 |= (1 << 0); //enable counter CEN
  
  // prescale register, TIMx_PSC
  //TIMER15->TIMx_PSC |= (0b1000 << 0); //TODO: set prescaler to 800. 100kHz clock?
  TIMER15->TIMx_PSC = 79;
  TIMER15->TIMx_CCMR1 &= ~(0b11 << 0); //CC1 channel configured as output

  //write PWM mode 1. Differnce between mode 1 and mode 2????
  //TIMER15->TIMx_CCMR1 &= ~(0b1111 << 4); //Clear then set OCxM bits in TIMx_CCMRx
  TIMER15->TIMx_CCMR1 |= (1 << 6); //0111 for mode 2
  TIMER15->TIMx_CCMR1 |= (1 << 5);
  TIMER15->TIMx_CCMR1 &= ~(1 << 4); //OCxM set to 110
  TIMER15->TIMx_CCMR1 |= (1 << 3); //enable preload reg. setting OCxPE bit in TIMx_CCMRx

  //TODO: initialize TIMx_ARR to max value?
  //TIMER15->TIMx_ARR = 0xffff;
  //TODO: initialize Duty Cycle to anything?

  //OCx output. set active high?
  TIMER15->TIMx_CCER &= ~(1 << 1); //OC1 signal is output on corresponding input pin. active high
  TIMER15->TIMx_CCER |= (1 << 0); //enable output
  TIMER15->TIMx_BDTR |= (1 << 15); //OC output enabled

  TIMER15->TIMx_EGR |= (1 << 0); //initialize all registers by setting UG bit in TIMx_EGR reg
}



void delay_millis(uint32_t ms){
  //play for desired amount of time. AKA count to ms amount
  //generate update event
  TIMER16->TIMx_EGR |= (1 << 0);

  //convert ms to cycles
  // each cycle is 0.01 ms, so multiply ms by 100 before adding into counter
  //apply at ARR 
  TIMER16->TIMx_ARR = (100*ms);
  //reset status register 
  TIMER16->TIMx_SR &= ~(1 << 0); //TODO: does this work?

  //set counter to 0 
  TIMER16->TIMx_CNT = 0;
  
  while((TIMER16->TIMx_SR & 1) == 0); //wait for flag

   TIMER16->TIMx_CR1 &= ~(1 << 0);
}

void pitch_set(uint32_t pitch){
  //set PWM to desired frequency 

  // set values of TIMx_ARR register
  // pitch is in Hz, clock is in 100kHz
  TIMER15->TIMx_ARR = 1000000/pitch;

  //set duty cycle to 50%
  TIMER15->TIMx_CCR1 |= TIMER15->TIMx_ARR/2;

  //update generation
  TIMER15->TIMx_EGR |= (1 << 0);

  //set counter to 0 
  TIMER15->TIMx_CNT = 0;

}