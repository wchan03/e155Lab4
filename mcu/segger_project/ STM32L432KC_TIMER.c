// STM32L432KC_TIMER.c
// Source code for timer functions

#include "STM32L432KC_TIMER.h"
//#include "math.h"

void enableDelayTimer(TIMER_TypeDef * TIMERx) { //used for delay

  // prescale register
  TIMERx->TIMx_PSC = 799; 
  
  //update generation
  TIMERx->TIMx_EGR |= (1 << 0);

  // configure counter 
  TIMERx->TIMx_CR1 |= (1 << 7); //ARR is enabled
  TIMERx->TIMx_CR1 |= (1 << 0); //counter enabled CEN
  TIMERx->TIMx_CNT = 0; //set counter to 0

}

void enablePWMTimer(TIMER_TypeDef * TIMERx){  //Pulse Width Modulation mode enabled //used to be timer15

  TIMERx->TIMx_PSC = 799; // prescale register, TIMx_PSC

  //write PWM mode 1
  TIMERx->TIMx_CCMR1 |= (1 << 6); 
  TIMERx->TIMx_CCMR1 |= (1 << 5);
  TIMERx->TIMx_CCMR1 &= ~(1 << 4); //OCxM set to 110
  TIMERx->TIMx_CCMR1 |= (1 << 3); //enable preload reg. setting OCxPE bit in TIMx_CCMRx

  TIMERx->TIMx_CCMR1 &= ~(0b11 << 0); //CC1 channel configured as output
  TIMERx->TIMx_CR1 |= (1 << 7); //auto-reload preload reg. by setting ARPE bit in TIMx_CR1 reg.
  TIMERx->TIMx_CR1 |= (1 << 0); //enable counter CEN

  //OCx output. set active high?
  TIMERx->TIMx_CCER &= ~(1 << 1); //OC1 signal is output on corresponding input pin. active high
  TIMERx->TIMx_CCER |= (1 << 0); //enable output
  TIMERx->TIMx_BDTR |= (1 << 15); //OC output enabled

  TIMERx->TIMx_EGR |= (1 << 0); //initialize all registers by setting UG bit in TIMx_EGR reg

}



void delay_millis(TIMER_TypeDef * TIMERx, uint32_t ms){  //play for desired amount of time. AKA count to ms amount
  

  //convert ms to cycles- each cycle is 0.1 ms, so multiply ms by 100 before adding into counter
  //apply at ARR 
  TIMERx->TIMx_ARR = (0);
  TIMERx->TIMx_ARR = (100*ms);

  //generate update event
  //if(ms == 0) return;
  TIMERx->TIMx_EGR |= (1 << 0);
  TIMERx->TIMx_CR1 |= (1<<0);

  //set counter to 0 
  TIMERx->TIMx_CNT = 0;  

  //reset status register 
  TIMERx->TIMx_SR &= ~(1 << 0); 

  while((TIMERx->TIMx_SR & 0b1) != 1){} //wait for flag
  //while(TIMERx->TIMx_SR != 2){}

  TIMERx->TIMx_CR1 &= ~(1 << 0);


}

void pitch_set(TIMER_TypeDef * TIMERx, uint32_t pitch){ //TIM15
  //set PWM to desired frequency 
  TIMERx->TIMx_CNT = 0;
  // set values of TIMx_ARR register
  // pitch is in Hz, clock is in 100kHz
  if(pitch == 0) {
    TIMERx->TIMx_CCR1 = 0;
    TIMERx->TIMx_ARR = 2; 
    TIMERx->TIMx_EGR |= (1 << 0);}
  else { 
    TIMERx->TIMx_ARR = 100000/pitch;
    //set duty cycle to 50%
    TIMERx->TIMx_CCR1 |= TIMERx->TIMx_ARR/2;
    //update generation
    TIMERx->TIMx_EGR |= (1 << 0);
  }


}