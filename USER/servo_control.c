#include "servo_control.h"

double Servo_Angle(double Location_corner)
{
  double error,x,y,servo_corner;
	uint8_t Install_angle;
	Install_angle = Install_angle_change();
	error = Location_corner;
	y = error * Each_Width; 
	x = CCD_H / tan(90.000 - Install_angle);
  servo_corner = (atan(y / x)) * (180.000 / PI);    //角度与弧度的关系
	return (servo_corner);
}

float Servo_pwm(double servo_corner)
{
	double Servo_pwm_duty,A;
	if(servo_corner != 0)
	{
	  Servo_pwm_duty = 750 - servo_corner;
			if(Servo_pwm_duty >= 1083 )
	    {
	      Servo_pwm_duty = 1083 ;
	    }
	    else if(Servo_pwm_duty < 0)
	    {
	     Servo_pwm_duty = 1 ;
	    }
      else
	    {
	     Servo_pwm_duty = Servo_pwm_duty;
	    }	
	}
	else
	{
	  Servo_pwm_duty = 750;
	}
  A = Servo_pwm_duty;
	printf("Servo_pwm_duty = %f",A);
 return (Servo_pwm_duty);
}	
