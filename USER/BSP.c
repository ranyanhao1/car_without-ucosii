#include "BSP.h"
#include "includes.h"
extern uint8_t CCD_filtering_data[128];
uint32_t number;
int PWM_Duty;
void BSP_Init(void)
{
	DelayInit();
  OLED_pin_Config();
	LCD_Init();
	GPIO_Config();
	LPT_Config();
  PIT_Config();
	UART_Config();
  ADC_Config();
	PWM_Config();
  PID_Init();
}

void GPIO_Config(void)
{
  GPIO_QuickInit(HW_GPIOE,0,kGPIO_Mode_OPP);
	GPIO_WriteBit(HW_GPIOE,0,0);  //指示灯
	
	//CCD引脚初始化
	GPIO_QuickInit(HW_GPIOB,2,kGPIO_Mode_OPP);
	GPIO_QuickInit(HW_GPIOB,4,kGPIO_Mode_OPP);
	
	//拨码开关引脚初始化
	GPIO_QuickInit(HW_GPIOC, 12, kGPIO_Mode_IPU);  //上拉输入
	GPIO_QuickInit(HW_GPIOC, 13, kGPIO_Mode_IPU);
	GPIO_QuickInit(HW_GPIOC, 14, kGPIO_Mode_IPU);
	GPIO_QuickInit(HW_GPIOC, 15, kGPIO_Mode_IPU);
	GPIO_QuickInit(HW_GPIOC, 16, kGPIO_Mode_IPU);
	GPIO_QuickInit(HW_GPIOC, 17, kGPIO_Mode_IPU);
	GPIO_QuickInit(HW_GPIOC, 18, kGPIO_Mode_IPU);
	GPIO_QuickInit(HW_GPIOC, 19, kGPIO_Mode_IPU);
}

void UART_Config(void)
{
	UART_QuickInit(UART0_RX_PA15_TX_PA14,115200);
}

void ADC_Config(void)
{
  ADC_QuickInit(ADC0_SE8_PB0,kADC_SingleDiff8or9);  //CCD
	ADC_QuickInit(ADC1_SE6_PE2,kADC_SingleDiff8or9);
}

void PWM_Config(void)
{
	GPIO_QuickInit(HW_GPIOC,2,kGPIO_Mode_OPP);
	GPIO_WriteBit(HW_GPIOC, 2, 0);
	
  FTM_PWM_QuickInit(FTM1_CH0_PA08,kPWM_EdgeAligned,50);  //舵机周期20ms
	FTM_PWM_ChangeDuty(HW_FTM1,HW_FTM_CH0,750);
	
//	FTM_PWM_QuickInit(FTM0_CH0_PC01,kPWM_EdgeAligned,3000);//电机周期需要实验验证平滑性
//	FTM_PWM_ChangeDuty(HW_FTM0,HW_FTM_CH0,1500);
}

void PIT_Config(void)  
{
  PIT_QuickInit(HW_PIT_CH0,100*1000);   //周期100ms
	PIT_CallbackInstall(HW_PIT_CH0, Pulse_Counting_ISR);
	PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF); //开启PIT通道0中断
}

void LPT_Config(void)
{
   LPTMR_PC_QuickInit(LPTMR_ALT2_PC05); 
}

void Pulse_Counting_ISR(void)
{
  number=LPTMR_PC_ReadCounter();
	LPTMR_ClearCounter();
}

void OLED_pin_Config(void)
{
  GPIO_QuickInit(HW_GPIOD,6,kGPIO_Mode_OPP);
	GPIO_QuickInit(HW_GPIOD,7,kGPIO_Mode_OPP);
	GPIO_QuickInit(HW_GPIOD,8,kGPIO_Mode_OPP);
	GPIO_QuickInit(HW_GPIOD,9,kGPIO_Mode_OPP);
}

void Voltage_Show(void)
{
	float Voltage; 
  Voltage = ((ADC_QuickReadValue(ADC1_SE6_PE2))/(1000.00))*(5300.00); 
	if(Voltage > 7.00)
	{
	  LCD_Print(1,2,"Voltage is ok");
	}
	else
	{
	  LCD_Print(1,2,"Voltage is LOW");
	}
}

void Delay_stick(uint8_t times)
{
  uint8_t i=0;
	for(i=0;i<times;i++)
	{
	  __nop();
	}
}	

uint8_t CCD_TASK(void)
{
	uint8_t i;
  CCD_gather();
 // CCD_Filtering();	
 // Data_binarization(averaging());		
	//return (0);
	//return(TrackMidline());
}

void DIR_TASK(uint8_t Track_Midline_value)
{
	double iIncpid_1; 
  iIncpid_1 = PID_Calc(1, 0, Track_Midline_value);	
	FTM_PWM_ChangeDuty(HW_FTM1,HW_FTM_CH0,Servo_pwm(iIncpid_1));
}

void SCU_TASK(uint32_t speed)
{
  PWM_Duty += PID_Calc(0, speed, 0);
	if(PWM_Duty >= 1000)
	{
	  FTM_PWM_ChangeDuty(HW_FTM0,HW_FTM_CH0,1000);  /* 0-10000 对应 0-100%占空比 */
	//	printf("PWM_Duty = %d\n",PWM_Duty);
	}
	if(PWM_Duty > 0 && PWM_Duty < 1000)
	{
		FTM_PWM_ChangeDuty(HW_FTM0,HW_FTM_CH0,PWM_Duty);  /* 0-10000 对应 0-100%占空比 */
	//	printf("PWM_Duty = %d\n",PWM_Duty);
	}
	if (PWM_Duty <= 0)
	{
	 FTM_PWM_ChangeDuty(HW_FTM0,HW_FTM_CH0,0);  /* 0-10000 对应 0-100%占空比 */
//		printf("PWM_Duty = %d\n",0);
	}
}


