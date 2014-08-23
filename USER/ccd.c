#include "ccd.h"

uint8_t CCD_original_data[132]={0};
uint8_t CCD_filtering_data[128]={0};

struct process_flag Lost_flag;


void CCD_gather(void)
{
	uint8_t i = 0;
	CCD_CLK(0);
	__nop();
	CCD_SI(1);  
	__nop();
	CCD_CLK(1);
	__nop();
	CCD_SI(0);
	__nop();
	

	for(i=0;i<128;i++)   
	{
	  CCD_CLK(0);
	  __nop();
		CCD_original_data[i+2] = ADC_QuickReadValue(ADC0_SE8_PB0);
		CCD_CLK(1);
		__nop();
	}

	__nop();   
	CCD_CLK(1);
	__nop();
	CCD_CLK(0);
	__nop();
	
//	for(i=0;i<128;i++)
//		{
//      if(CCD_original_data[127+2]==0xff)
//			{
//        CCD_original_data[127+2]=0xfe;
//      }
//			UART_WriteByte(HW_UART0,CCD_original_data[i+2]);
//    }
//		UART_WriteByte(HW_UART0,0xff);
}

void CCD_Restet(void)
{
	uint8_t i=0;
  CCD_CLK(0);
	__nop();
	CCD_SI(1);
	__nop();
	CCD_CLK(1);//?????CLK
	__nop();
	CCD_SI(0);  
	__nop();  
	
	for(i=1;i<129;i++)   //???2--129?CLK
	{
		CCD_CLK(0);
	  __nop();
    CCD_CLK(1);
		__nop();
	}
}


void CCD_Filtering(void)      //Êý¾ÝÂË²¨
{
	uint8_t i =0,j = 0,k = 0,MAX = 0,MIN = 0;
	uint8_t temp[5] = {0};
	CCD_original_data[0] = CCD_original_data[2];
	CCD_original_data[1] = CCD_original_data[3];
	CCD_original_data[130] = CCD_original_data[128];
	CCD_original_data[131] = CCD_original_data[129];	
	for(i=0;i<128;i++)
	{
	  for(j=0,k=i;j<5;j++)
		{
			k = i+j;
		  temp[j] = CCD_original_data[k];
		}
		MAX = MAX_5(temp[0],temp[1],temp[2],temp[3],temp[4]);
		MIN = MIN_5(temp[0],temp[1],temp[2],temp[3],temp[4]);
		if(MAX == CCD_original_data[i+2] || MIN == CCD_original_data[i+2])
		{
		  CCD_filtering_data[i] =  Data_sort(temp);
		}
		else
		{
		  CCD_filtering_data[i] = CCD_original_data[i+2];
		}
	}
//		for(i=0;i<128;i++)
//		{
//      if(CCD_filtering_data[127]==0xff)
//			{
//        CCD_filtering_data[127]=0xfe;
//      }
//			UART_WriteByte(HW_UART0,CCD_filtering_data[i]);
//    }
//		UART_WriteByte(HW_UART0,0xff);
}


void Data_binarization(uint8_t average)
{
  uint8_t i = 0;
	for(i=0;i<128;i++)
	{
	  if(CCD_filtering_data[i] >= average)
		{
		 CCD_filtering_data[i] = 0XFF;
		}
		else
		{
		  CCD_filtering_data[i] = 0X00;
		}
	}
			for(i=0;i<128;i++)
		{
      if(CCD_filtering_data[127]==0xff)
			{
        CCD_filtering_data[127]=0xfe;
      }
			UART_WriteByte(HW_UART0,CCD_filtering_data[i]);
    }
		UART_WriteByte(HW_UART0,0xff);
}	


uint8_t Data_sort(uint8_t data[5])
{
  uint8_t i = 0,j = 0,temp = 0;
	for(j=0;j<5;j++)
	{
	  for(i=0;i<5-j;i++)
		{
		  if(data[i] > data[i+1])
			{
			  temp = data[i];
				data[i] = data[i+1];
				data[i+1] = temp;
			}
		}
	}
	return (data[3]);
}


uint8_t averaging(void)
{
  uint8_t i = 0,MIN=CCD_filtering_data[0],MAX=CCD_filtering_data[0];
	for(i=1;i<128;i++)
	{
	  MAX = MAX_2(MAX,CCD_filtering_data[i]);
		MIN = MIN_2(MIN,CCD_filtering_data[i]);
	}
	return ((MAX + MIN)/2);
}	

uint8_t TrackMidline(void)
{
	  uint8_t i = 0;
	  uint8_t Left_Side = 0,Right_Side = 0,Track_Midline_value = 0,A = 0,B = 0;
		for(i=0;i<64;i++)
	  {
	   if(CCD_filtering_data[64-i]==0)
		 {
		   Left_Side = 64 - i;
			 break;
		 }
	  	else
			{
			  Left_Side = 0;
			}				
	  }
		
		for(i=0;i<64;i++)
	  {
	   if(CCD_filtering_data[64+i]==0)
		 {
		   Right_Side = 64 + i;
			 break;
		 }
	  	else
			{
			  Right_Side = 127;
			}				
	  }
		A = Left_Side;
		B = Right_Side;
//		printf("Left_Side = %d\n",A);
//		printf("Right_Side = %d\n",B);
		if(Left_Side <=8 && (Right_Side >=80 && Right_Side <= 88))
		{
		  Lost_flag.Lost_left = 1;
		}
		else if (Left_Side >= 40 && Right_Side >=110)
		{
		  Lost_flag.Lost_right = 1;
		}
    else
		{
      Lost_flag.Lost_left  = 0;
		  Lost_flag.Lost_right = 0;
    }
		
		Track_Midline_value = ((Right_Side + Left_Side)/2);
		return (Track_Midline_value);
}

uint8_t MAX_5(uint8_t data_0,uint8_t data_1,uint8_t data_2,uint8_t data_3,uint8_t data_4)
{
 return (MAX_2(MAX_2(MAX_2(MAX_2(data_0,data_1),data_2),data_3),data_4));
}

uint8_t MIN_5(uint8_t data_0,uint8_t data_1,uint8_t data_2,uint8_t data_3,uint8_t data_4)
{
 return (MIN_2(MIN_2(MIN_2(MIN_2(data_0,data_1),data_2),data_3),data_4));
}

uint8_t MAX_2(uint8_t a,uint8_t b)
{
	return ((a >= b)? a:b); 
}

uint8_t MIN_2(uint8_t a,uint8_t b)
{
 return ((a <= b)? a:b);
}



