#include "includes.h"

extern uint32_t number;

int main()
{
	uint8_t Track_Midline_value;
  BSP_Init();
	Voltage_Show();
  LCD_Print(1,2,"Voltage = ");
  CCD_Restet();
	PID_Init();
  while(1)
	{
   GPIO_ToggleBit(HW_GPIOE,0);
   Track_Midline_value = CCD_TASK();
	 DIR_TASK(Track_Midline_value);
	 SCU_TASK(number);	
  }
}

