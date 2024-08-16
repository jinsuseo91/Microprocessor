#include "TExaS.h"
#include "TM4C123GH6PM.h"
#include <stdio.h>

#define two_second 0xF4240 // two-second delay from microsecond delay function (0xF4240 in hex is 1000000 in decimal)

unsigned long SW1, SW2;
unsigned long prev = 0;

void Servo_0_Degree(void);
void Servo_90_Degree(void);
void Servo_180_Degree(void);
void PLL_Init(void);

void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}
// 10000us equals 10ms
void SysTick_Wait10ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<delay; i++){
    SysTick_Wait(8000);  // wait  0.1ms
  }
}

void PORTA_Init(void){ unsigned long volatile delay;
    SYSCTL_RCGC2_R |= 0x00000001; // (a) activate clock for port A
   delay = SYSCTL_RCGC2_R;
    GPIO_PORTA_DIR_R |= 0x10; // (c) make PA4
   GPIO_PORTA_AFSEL_R &= ~0x10; // disable alt funct on PA4
   GPIO_PORTA_DEN_R |= 0x10; // enable digital I/O on PA4
   GPIO_PORTA_PCTL_R &= ~0x000F0000; // configure PA4 as GPIO
   GPIO_PORTA_AMSEL_R &= ~0x10; // disable analog functionality on PA4
}

void Switch_Init(void){ unsigned long volatile delay;
   SYSCTL_RCGC2_R |= 0x00000020; // (a) activate clock for port F
   delay = SYSCTL_RCGC2_R;
   GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
   GPIO_PORTF_CR_R = 0x11; // allow changes to PF4,0
   GPIO_PORTF_DIR_R &= ~0x11; // (c) make PF4,0 in (built-in button)
   GPIO_PORTF_AFSEL_R &= ~0x11; // disable alt funct on PF4,0
   GPIO_PORTF_DEN_R |= 0x11; // enable digital I/O on PF4,0
   GPIO_PORTF_PCTL_R &= ~0x000F000F; // configure PF4,0 as GPIO
   GPIO_PORTF_AMSEL_R &= ~0x11; // disable analog functionality on PF4,0
   GPIO_PORTF_PUR_R |= 0x11; // enable weak pull-up on PF4,0
}

/* Main code to control servo motor angular movement */
int main(void)
{
      PLL_Init();
      PORTA_Init();
      Switch_Init();
      SysTick_Init();
    while (1)
    {
            SW1 = GPIO_PORTF_DATA_R&0x10;
            SW2 = GPIO_PORTF_DATA_R&0x01;
         
            if(!SW1 && !SW2){
               Servo_90_Degree();
            }
            else if(!SW1){
               Servo_0_Degree();
            }
            else if(!SW2){
               Servo_180_Degree();
            }
    }
}

void Servo_0_Degree(void)
{
    int i;
    for (i = 0; i < 50; i++)
    {
        GPIO_PORTA_DATA_R |= 0x10; /* make control pin high */
        SysTick_Wait10ms(24);
        GPIO_PORTA_DATA_R &= ~0x10; /* make control pin low */
        SysTick_Wait10ms(176);
    }
}

void Servo_90_Degree(void)
{
    int i;
    for (i = 0; i < 50; i++)
    {
        GPIO_PORTA_DATA_R |= 0x10; /* make control pin high */
        SysTick_Wait10ms(14); 
        GPIO_PORTA_DATA_R &= ~0x10; /* make control pin low */
        SysTick_Wait10ms(186);
    }
}

void Servo_180_Degree(void)
{
    int i;
    for (i = 0; i < 50; i++)
    {
        GPIO_PORTA_DATA_R |= 0x10; /* make control pin high */
        SysTick_Wait10ms(6);
        GPIO_PORTA_DATA_R &= ~0x10; /* make trigger pin high */
        SysTick_Wait10ms(194);
    }
}