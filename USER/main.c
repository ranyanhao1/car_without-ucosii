#include "includes.h"


int main()
{
	uint8_t Track_Midline_value = 0;
  BSP_Init();
//	DelayMs(1000);
	Voltage_Show();
  CCD_Restet();
	PID_Init();
  while(1)
	{
   GPIO_ToggleBit(HW_GPIOE,0);
   Track_Midline_value = CCD_TASK();
	 DIR_TASK(Track_Midline_value);
	 SCU_TASK(number);
	 DelayMs(20);	
  }
}

