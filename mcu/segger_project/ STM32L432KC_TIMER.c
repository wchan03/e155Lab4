// STM32L432KC_TIMER.c
// Source code for timer functions

#include "STM32L432KC_TIMER.h"
//#include "math.h"

void enableDelayTimer(TIMER_TypeDef * TIMERx) { //used for delay

  // prescale register
  TIMERx->TIMx_PSC = 79; //TODO: set prescaler to 80. 100kHz clock?
  
  //update generation
  TIMERx->TIMx_EGR |= (1 << 0);

  // configure counter 
  TIMERx->TIMx_CR1 |= (1 << 7); //ARR is enabled
  TIMERx->TIMx_CR1 |= (1 << 0); //counter enabled CEN
  TIMERx->TIMx_CNT = 0; //set counter to 0

}

void enablePWMTimer(TIMER_TypeDef * TIMERx){  //Pulse Width Modulation mode enabled //used to be timer15

  //TODO: does order matter?
  // prescale register, TIMx_PSC
  //TIMER15->TIMx_PSC |= (0b1000 << 0); //TODO: set prescaler to 800. 100kHz clock?
  TIMERx->TIMx_PSC = 79;

   //write PWM mode 1. Differnce between mode 1 and mode 2????
  //TIMER15->TIMx_CCMR1 &= ~(0b1111 << 4); //Clear then set OCxM bits in TIMx_CCMRx
  TIMERx->TIMx_CCMR1 |= (1 << 6); //0111 for mode 2
  TIMERx->TIMx_CCMR1 |= (1 << 5);
  TIMERx->TIMx_CCMR1 &= ~(1 << 4); //OCxM set to 110
  TIMERx->TIMx_CCMR1 |= (1 << 3); //enable preload reg. setting OCxPE bit in TIMx_CCMRx

  TIMERx->TIMx_CCMR1 &= ~(0b11 << 0); //CC1 channel configured as output
  ///disable slave select???
  TIMERx->TIMx_CR1 |= (1 << 7); //auto-reload preload reg. by setting ARPE bit in TIMx_CR1 reg.
  TIMERx->TIMx_CR1 |= (1 << 0); //enable counter CEN

  //OCx output. set active high?
  TIMERx->TIMx_CCER &= ~(1 << 1); //OC1 signal is output on corresponding input pin. active high
  TIMERx->TIMx_CCER |= (1 << 0); //enable output
  TIMERx->TIMx_BDTR |= (1 << 15); //OC output enabled

  TIMERx->TIMx_EGR |= (1 << 0); //initialize all registers by setting UG bit in TIMx_EGR reg
}



void delay_millis(TIMER_TypeDef * TIMERx, uint32_t ms){
  //play for desired amount of time. AKA count to ms amount
  //generate update event
  TIMERx->TIMx_EGR |= (1 << 0);
  TIMERx->TIMx_CR1 |=(1<<0);

  //convert ms to cycles- each cycle is 0.01 ms, so multiply ms by 100 before adding into counter
  //apply at ARR 
  TIMERx->TIMx_ARR = (100*ms);
  //reset status register 
  TIMERx->TIMx_SR &= ~(0 << 1); //TODO: does this work?
  TIMERx->TIMx_SR &= ~(1 << 0); 
  //set counter to 0 
  TIMERx->TIMx_CNT = 0;  
  while((TIMERx->TIMx_SR & 0b1) != 1){} //wait for flag

  TIMERx->TIMx_CR1 &= ~(1 << 0);
}

void pitch_set(TIMER_TypeDef * TIMERx, uint32_t pitch){
  //set PWM to desired frequency 

  // set values of TIMx_ARR register
  // pitch is in Hz, clock is in 100kHz
  TIMERx->TIMx_ARR = 100000/pitch; 

  //set duty cycle to 50%
  TIMERx->TIMx_CCR1 |= TIMER15->TIMx_ARR/2;

  //update generation
  TIMERx->TIMx_EGR |= (1 << 0);

  //set counter to 0 
  TIMERx->TIMx_CNT = 0;

}