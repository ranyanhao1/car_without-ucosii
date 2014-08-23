#ifndef __BSP_H_
#define __BSP_H_
#include "includes.h"

extern uint32_t number;

void BSP_Init(void);
void GPIO_Config(void);
void UART_Config(void);
void ADC_Config(void);
void PWM_Config(void);
void OLED_pin_Config(void);
void Voltage_Show(void);


void PIT_Config(void);
void LPT_Config(void);
void Pulse_Counting_ISR(void);

void Delay_stick(unsigned char times);
void DIR_TASK(uint8_t Track_Midline_value);
uint8_t CCD_TASK(void);
void SCU_TASK(uint32_t speed);

#endif

