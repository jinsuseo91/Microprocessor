#include <stdio.h> 
#include <math.h>
#include <stdint.h>
#include "uart.h"    // functions to implment input/output
#include "TExaS.h"   // Lab grader functions
#include "PLL.h"     // Set up core clock
#include "tm4c123gh6pm.h"

unsigned long volatile delay;
uint32_t Distance = 0;   // in cm
uint32_t period;          // in us
uint32_t ARRAY[50];
int i = 0;
// Define output pin
#define PD1                     (*((volatile unsigned long *)0x40007008))

void EnableInterrupts(void);
uint32_t Timer0A_periodCapture(void);
void Calculate_Distance(void);
void Timer0Capture_Init(void);
void PortD_Init(void);
void Trigger_Pulse(void);
void timer1A_delayus(int ttime);

void PortD_Init(void){
  SYSCTL_RCGC2_R |= 0x08;
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTD_DIR_R |= 0x02;
  GPIO_PORTD_AFSEL_R &= ~0x02;
  GPIO_PORTD_AMSEL_R &= ~0x02;
  GPIO_PORTD_DEN_R |= 0x02;
}

int main (void) {
   
  UART_Init();
   PLL_Init();
   Timer0Capture_Init(); 
   PortD_Init();  
  EnableInterrupts(); 
   PD1 = 0x00;
   
   
  while(1) {
      // Send Trigger
      Trigger_Pulse();
      //Calculate Distance
      Calculate_Distance(); 
      // Print Distance
       printf("\nDistance = %d cm\n",  Distance);
      if(i <50){
         if(Distance >= 70)
            {
               ARRAY[i] = 70; 
            }
         else {
               ARRAY[i] = Distance;
                     }
               
      i++;
   }
 }
}

// Send Trigger Pulse
void Trigger_Pulse(void)
{
   timer1A_delayus(600000);   // Wait 0.5s
   PD1 = 0x02;               // Start Trigger High
   timer1A_delayus(10);      // Wait 10 us
   PD1 = 0x00;               // Set Trigger Low
}


void Calculate_Distance(void)
{   

   // Find Period of Time
  period = Timer0A_periodCapture();

   // Calculate Distance in cm
   Distance = (period*0.034*1.25)/200;
    if (Distance > 70)
    {
       Distance = 70;
    }
       
}
uint32_t Timer0A_periodCapture(void)
{
   
int risingEdge, fallingEdge;

TIMER0_ICR_R = 4;                 // clear timer0A capture flag 
   
while((TIMER0_RIS_R & 4) == 0);                                 // wait till captured 

risingEdge = TIMER0_TAR_R;        // save the timestamp 

TIMER0_ICR_R = 4;                 // clear timer0A capture flag 

while((TIMER0_RIS_R & 4) == 0);                                 // wait till captured 

fallingEdge = TIMER0_TAR_R;       // save the timestamp 

return (fallingEdge - risingEdge); // return the time difference 
}



/* millisecond delay using one-shot mode */
void timer1A_delayus(int ttime)
{ 
SYSCTL_RCGCTIMER_R |= 2;
TIMER1_CTL_R = 0;
TIMER1_CFG_R = 0x00;
TIMER1_TAMR_R = 0x01;
TIMER1_TAILR_R = 80 * ttime - 1;
TIMER1_ICR_R = 0x1;
TIMER1_CTL_R |= 0x01;
while( (TIMER1_RIS_R & 0x1) == 0);
}
/* Functions to initialize Timer0A for edge-time capture mode to
* measure the period of a square-wave input signal
*
* square wave signal should be fed to PB6 pin.
* Make sure it is 3.3 to 5V peak-to-peak.
* Initialize Timer0A in edge-time mode to capture rising edges.
* The input pin of Timer0A is PB6.
*
*/
void Timer0Capture_Init(void)
{
SYSCTL_RCGCTIMER_R |= 1;          // enable clock to Timer Block 0 
SYSCTL_RCGC2_R |= 2;              // enable clock to PORTB 
GPIO_PORTB_DIR_R &= ~0x40;        // make PB6 an input pin 
GPIO_PORTB_DEN_R |= 0x40;         // make PB6 as digital pin 
GPIO_PORTB_AFSEL_R |= 0x40;       // use PB6 alternate function 
GPIO_PORTB_PCTL_R &= ~0x0F000000; // configure PB6 for T0CCP0 
GPIO_PORTB_PCTL_R |= 0x07000000;
TIMER0_CTL_R &= ~1;               // disable timer0A during setup 
TIMER0_CFG_R = 4;                 // 16-bit timer mode 
TIMER0_TAMR_R = 0x17;             // up-count, edge-time, capture mode 
TIMER0_CTL_R |= 0x0C;             // capture the both edges to start
TIMER0_CTL_R |= 1;                // enable timer0A 
}