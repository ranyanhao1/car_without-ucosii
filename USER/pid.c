#include "pid.h"

static PID sPID;
static PID*sptr = &sPID;
int8_t iError_1;               //当前误差
extern uint8_t Lost_right;
extern uint8_t Lost_left;
void PID_Init(void)
{
  sptr->LastError  = 0;         
	sptr->PrevError  = 0;         
	sptr->Proportion = P_DATA;
	sptr->Integral   = I_DATA;
	sptr->Derivative = D_DATA;
	sptr->SetPoint   = Speed_Set();                   //((200/17.279)*(106/40)*200)*(100/1000);
	
	sptr->LastError_1  = 0;         
	sptr->PrevError_1  = 0;         
	sptr->Proportion_1 = P_DATA_1;
	sptr->Integral_1   = I_DATA_1;
	sptr->Derivative_1 = D_DATA_1;
	sptr->View_Midline   = 64;                  
}	

double PID_Calc(uint8_t pid_flag, int NextPoint, uint8_t Track_Midline)
{ 
  int iError,A,B;                 //当前误差
	int iIncpid;                //增量值
	double iIncpid_1;              //增量值
	if(pid_flag==0)
	{
		if((Lost_right == 1) || (Lost_left == 1) )
		{
		 if(Lost_right)
		 {
		   Lost_right = 0;
			 iIncpid = -500;
		 }
		 else 
		 {
		  Lost_left = 0;
			iIncpid = 751;
		 }
		}
	 else
	 {
	 	 iError = sptr->SetPoint - NextPoint;
		 A = iError;
//	   printf("iError = %d\n",A);
		 if(abs(iError) > 20)
		 {
		   iIncpid = sptr->Proportion * iError + sptr->Integral * sptr->LastError;
		 }
		 else
		 {
		   iIncpid = sptr->Proportion * iError + sptr->Integral * sptr->LastError + sptr->Derivative * sptr->PrevError; 
		 }
	   B = iIncpid; 
//	   printf("iIncpid = %d\n",B);
	   sptr->PrevError = sptr->LastError;
	   sptr->LastError = iError;
	 }
	 return (iIncpid);
	}
	else
	{
	 iError_1 = Track_Midline - sptr->View_Midline;
	 sptr->SetPoint = 310 - (2* iError_1* iError_1);	
	 if(sptr->SetPoint < 150)
	 {
     sptr->SetPoint  = 150;	
	 }
//		printf("iError_1 = %d\n",iError_1);
//		printf("LastError = %d\n",sptr->LastError_1);
		if(iError_1 >=0)
		{
		 sptr->Proportion_1 = 0.5 * iError_1*iError_1  ;
		}
		else
		{
		 sptr->Proportion_1 = 0.5 * iError_1*iError_1  ;
		}
   iIncpid_1 = sptr->Proportion_1 * iError_1 + sptr->Derivative_1 * sptr->LastError_1;		
	 sptr->LastError_1 = iError_1;
	 return (iIncpid_1);
	}
}	


