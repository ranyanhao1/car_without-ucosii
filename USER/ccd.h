#ifndef __CCD_H_
#define __CCD_H_
#include "includes.h"

extern uint8_t CCD_filtering_data[128];
extern struct process_flag Lost_flag;

#define  CCD_SI(data)    GPIO_WriteBit(HW_GPIOB,2,data);
#define  CCD_CLK(data)   GPIO_WriteBit(HW_GPIOB,4,data);

struct process_flag
{
  uint8_t Lost_left  : 1;
  uint8_t Lost_right : 1;
};

void CCD_gather(void);
uint8_t TrackMidline(void);
uint8_t MAX_2(uint8_t a,uint8_t b);
uint8_t MAX_5(uint8_t data_0,uint8_t data_1,uint8_t data_2,uint8_t data_3,uint8_t data_4);
uint8_t MIN_2(uint8_t a,uint8_t b);
uint8_t MIN_5(uint8_t data_0,uint8_t data_1,uint8_t data_2,uint8_t data_3,uint8_t data_4);
uint8_t Data_sort(uint8_t data[5]);
void Data_binarization(uint8_t average);
uint8_t averaging(void);
void CCD_Filtering(void);
void CCD_Restet(void);

#endif
