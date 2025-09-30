// STM32L432KC_TIMER.h
// Header for FLASH functions

#ifndef STM32L4_TIMER_H
#define STM32L4_TIMER_H

#include <stdint.h>

//DEFINITIONS
// Base address for timers
#define TIMER_15_BASE (0x40014000UL) 
#define TIMER_16_BASE (0x40014400UL) //pg 73

//BITFIELDS

typedef struct {
  volatile uint32_t TIMx_CR1; //! Control Register 1
  volatile uint32_t TIMx_CR2; //! Control Register 2
  uint32_t R1_SMCR; //! slave mode control. should stay disabled. reserved
  volatile uint32_t TIMx_DIER; //! DMA/interrupt enable register
  volatile uint32_t TIMx_SR; //! Status
  volatile uint32_t TIMx_EGR; //! Event Generation
  volatile uint16_t TIMx_CCMR1; // capture/compare mode reg. 1, output compare mode
  uint32_t R2_TIMx_CCMR1; // capture/compare reserved
  volatile uint32_t TIMx_CCER; //capture/compare enable
  volatile uint32_t TIMx_CNT; // Counter
  volatile uint32_t TIMx_PSC; // Prescaler
  volatile uint32_t TIMx_ARR; // Auto-reload
  volatile uint32_t TIMx_RCR; //Repetition Counter
  volatile uint32_t TIMx_CCR1; // Capture/Compare
  uint32_t R3_TIM15_CCR2; // Reserved 
  uint32_t R4_TIM15_CCR2; // Reserved
  uint32_t R5_TIM15_CCR2; // Reserved
  volatile uint32_t TIMx_BDTR; //Break and Dead-time 
  volatile uint32_t TIMx_DCR; // DMA Control
  volatile uint32_t TIMx_DMAR; // DMA address for full transfer
  volatile uint16_t TIMx_OR1; //Option Reg. 1
  uint32_t R6_TIMx_OR1; // Reserved
  uint32_t R7_TIMx_OR1; // Reserved
  uint32_t R8_TIMx_OR1; // Reserved
  volatile uint16_t TIMx_OR2; //Option Reg. 2
} TIMER_TypeDef;

#define TIMER16 ((TIMER_TypeDef *) TIMER_16_BASE)
#define TIMER15 ((TIMER_TypeDef *) TIMER_15_BASE)

//FUNCTIONS

void enableTimer15(void);

void enableTimer16(void);

void delay_millis(uint32_t ms);

void pitch_set(uint32_t pitch);

#endif