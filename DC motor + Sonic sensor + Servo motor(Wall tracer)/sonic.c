#include <stdio.h>   // standard C library 
#include <math.h>
#include <stdint.h>
#include "TExaS.h"   // Lab grader functions
#include "PLL.h"     // Set up core clock
#include "sonic.h"
#include "tm4c123gh6pm.h"
// ***** 2. Global Declarations Section *****
unsigned long volatile delay;
unsigned long Distance = 0;   // in cm
unsigned long period;          // in us

void timer1A_delayus(int ttime)
{ 
SYSCTL_RCGCTIMER_R |= 0x02;             // (1) timer 0번 사용
TIMER1_CTL_R = 0;                    // (2) 초기화를 위한 비활성화
TIMER1_CFG_R = 0x00;                 // (3) 32bit의 타이머로 설정
TIMER1_TAMR_R = 0x01;                // (4) one-shot mode and down-counter 
TIMER1_TAILR_R = 80 * ttime - 1;     // (5) 80MHz이므로 time microsecond 설정
TIMER1_ICR_R = 0x01;                  // (6) flag 초기화
TIMER1_CTL_R |= 0x01;                // (7) timer 활성화
while( (TIMER1_RIS_R & 0x01) == 0);   // (8) 설정 시간만큼 busy wait
}

// Send Trigger Pulse
void Trigger_Pulse(void)
{
   timer1A_delayus(800);   // Wait 60 ms 1.25Hz
   GPIO_PORTE_DATA_R |= 0x20;               // Start Trigger High
   timer1A_delayus(10);      // Wait 10 us
   GPIO_PORTE_DATA_R &= 0x00;               // Set Trigger Low
}

// Measure the time difference between the rising edge and falling edge of the ECHO pin
unsigned long Timer0A_periodCapture(void)
{
   
int risingEdge, fallingEdge;

TIMER0_ICR_R = 0x04;                 // clear flag 
   
while((TIMER0_RIS_R & 0x04) == 0);       //위의 줄까지  timer를 초기화

risingEdge = TIMER0_TAR_R;        // 시작 시각 기록

TIMER0_ICR_R = 0x04;                 // clear timer0A capture flag 

while((TIMER0_RIS_R & 0x04) == 0);	//대기

fallingEdge = TIMER0_TAR_R;       //끝나는 시각 기록

return (fallingEdge - risingEdge); // 차를 통해 시간 반환
}

void Calculate_Distance(void)
{   
  period = Timer0A_periodCapture();
  Distance = (period*0.0343)/200;
}

void Timer0Capture_Init(void)
{
SYSCTL_RCGCTIMER_R |= 0x01;          // enable clock to Timer Block 0 
SYSCTL_RCGC2_R |= 0x02;              // enable clock to PORTB
delay = SYSCTL_RCGC2_R;
GPIO_PORTB_DIR_R &= ~0x40;        // make PB6 an input pin 
GPIO_PORTB_DEN_R |= 0x40;         // make PB6 as digital pin 
GPIO_PORTB_AFSEL_R |= 0x40;       // use PB6 alternate function 
GPIO_PORTB_PCTL_R &= ~0x0F000000; // configure PB6 for T0CCP0 
GPIO_PORTB_PCTL_R |= 0x07000000;
TIMER0_CTL_R &= ~0x01;               // disable timer0A during setup 
TIMER0_CFG_R = 0x04;                 // 16-bit timer로 설정
TIMER0_TAMR_R = 0x17;             // up-count, edge-time
TIMER0_CTL_R |= 0x0C;             // both edges 설정
TIMER0_CTL_R |= 0x01;                // enable timer0A 
}


void Init_PortE(void){
  SYSCTL_RCGC2_R |= 0x10;           // Port E clock
  delay = SYSCTL_RCGC2_R;           // wait 3-5 bus cycles
  GPIO_PORTE_DIR_R |= 0x20;         // PE5 output
  GPIO_PORTE_AFSEL_R &= ~0x20;      // not alternative
  GPIO_PORTE_AMSEL_R &= ~0x20;      // no analog
  GPIO_PORTE_DEN_R |= 0x20;         // enable PE1
  GPIO_PORTE_DATA_R &= 0x00;                            // Start Trigger Low
}

// ***** 3. Subroutines Section *****
void sonic_Init(void) {
   PLL_Init();                            // initialize PLL
   Timer0Capture_Init();                  // initialize echo timer
   Init_PortE();            
}


unsigned long Get_data(void){
	Trigger_Pulse();

	Calculate_Distance(); 
	
	if(Distance > 70){
		return 70;
	}
	else{
		return Distance;
	}
}
