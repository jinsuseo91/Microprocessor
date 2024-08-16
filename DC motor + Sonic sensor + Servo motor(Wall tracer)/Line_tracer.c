#include "tm4c123gh6pm.h"
#include "TExaS2.h"
#include "DC_motor.h"
#include "sonic.h"
#include "stdlib.h"
 
void EnableInterrupts(void);  // E nable interrupts
unsigned long Sensor_data, left_d, right_d, mid_d;
unsigned long a,b,c,d;
unsigned long right_min, left_min, mid_min;
char Direction;
void Delay1ms(unsigned long time);
extern unsigned long H, L;
int i , j , k, z ;
int main(void){
    unsigned long volatile delay;
    TExaS_Init(ADC0_AIN1_PIN_PE2, ADC0_AIN1_PIN_PE1, ADC0_AIN1_PIN_PE0, LED_PIN_PF2);
	
    DC_Motor_Init();
    sonic_Init();

  while(1){
      right_d = 0;
      right_min = 0;
      left_d = 0;
      left_min = 0;
      mid_d = 0;
      mid_min = 0;
      i = 0;
      j = 0;
      k = 0;
      z = 0;
		
      Motor_stop();
      
      Ser_motor_0(&H, &L);
      Delay100ms(1);
      right_d = Get_data();
      right_min = right_d;
      while(i<15){
       right_d = Get_data();
       if(right_d < right_min) right_min = right_d;
       i++;                                                                                                    
      }
         
      Ser_motor_90(&H, &L);
      Delay100ms(1);
      while(j<15){
       right_d = Get_data();
       if(right_d < right_min) right_min = right_d;
       j++;
      }
			
      Ser_motor_180(&H, &L);
      Delay100ms(1);
      left_d = Get_data();
      left_min = left_d;
      while(k<15){
       left_d = Get_data();
       if(left_d < left_min) left_min = left_d;
       k++;
      }
			
      Ser_motor_90(&H, &L);
      while(z<15){
       left_d = Get_data();
        if(left_d < left_min) left_min = left_d;
       z++;
       }
      
      if((left_min > 15 & right_min >15) |  (left_min == right_min)){
         Change_direction(0, 'm');
          Delay100ms(4);
      }
      else if(left_min<right_min){
         if(left_min < 4) {
            Change_direction(0,'r');
            Delay100ms(3);
            Change_direction(0,'m');
            Delay100ms(1);
         }
            else if(left_min < 8) {
            Change_direction(0,'r');
            Delay100ms(2);
            Change_direction(0,'m');
            Delay100ms(1);
         }
         else{ 
            Change_direction(0,'r');
            Delay100ms(1);
            Change_direction(0,'m');
            Delay100ms(3);
         }      
      }
      else {
         if(right_min < 4) {
            Change_direction(0,'l');
            Delay100ms(3);
            Change_direction(0,'m');
            Delay100ms(1);
         }
             else if(right_min < 8) {
            Change_direction(0,'l');
            Delay100ms(2);
            Change_direction(0,'m');
            Delay100ms(1);
         }
         else{
            Change_direction(0,'l');
            Delay100ms(1);
            Change_direction(0,'m');
            Delay100ms(3);
         } 
      }
  }
}