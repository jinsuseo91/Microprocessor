#include "tm4c123gh6pm.h"
#include "motor_right.h"

unsigned long HR, LR;

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void); // Enable interrupts
void WaitForInterrupt(void);

void Motor_right_port_Init(void){	//port A's 
   SYSCTL_RCGC2_R |= 0x00000001; // activate clock for port A
   LR = HR = 40000; // 50%
   GPIO_PORTA_AMSEL_R &= ~0x30; // disable analog functionality on PA5
   GPIO_PORTA_PCTL_R &= ~0x00F00000; // configure PA5 as GPIO
   GPIO_PORTA_DIR_R |= 0x30; // make PA5 out
   GPIO_PORTA_DR8R_R |= 0x30; // enable 8 mA drive on PA5
   GPIO_PORTA_AFSEL_R &= ~0x30; // disable alt funct on PA5
   GPIO_PORTA_DEN_R |= 0x30; // enable digital I/O on PA5
   GPIO_PORTA_DATA_R &= ~0x30; // make PA5 low
   NVIC_ST_CTRL_R = 0; // disable SysTick during setup
   NVIC_ST_RELOAD_R = LR-1; // reload value for 500us
   NVIC_ST_CURRENT_R = 0; // any write to current clears it
   NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
   NVIC_ST_CTRL_R = 0x00000007; // enable with core clock and interrupts
}

void SysTick_Handler(void){
   if(GPIO_PORTA_DATA_R&0x20){ // toggle PA5
      GPIO_PORTA_DATA_R &= ~0x20; // make PA5 low
      NVIC_ST_RELOAD_R = LR-1; // reload value for low phase
   } else{
      GPIO_PORTA_DATA_R |= 0x20; // make PA5 high
      NVIC_ST_RELOAD_R = HR-1; // reload value for high phase
   }
}

void Motor_right_Init(void){
	 DisableInterrupts(); // disable interrupts while initializing
   Motor_right_port_Init(); // output from PA5, SysTick interrupts
   EnableInterrupts(); // enable after all initialization are done
}

void Motor_stop_right(void){
	NVIC_ST_CTRL_R = 0x00000000;
	NVIC_ST_RELOAD_R = 0;
}

void Motor_go_right(void){
	NVIC_ST_RELOAD_R = HR;
	NVIC_ST_CTRL_R = 0x00000007;
}

void Set_Speed_right(unsigned long data){
	if ((data & 0x0E) == 0x00){
		LR = 40000;
	}
	else if(((data&0x04) == 0x04) || (((data & 0x02) == 0x02) && (LR > 20000))){
		LR = LR + 8000;
	} else if (((data & 0x08) == 0x08) && (LR < 60000)){
		LR = LR - 8000;
	}
	
	HR = 80000 - LR;
}

unsigned long Get_Waveform_right(void){
	unsigned long result;
	result = GPIO_PORTA_DATA_R & 0x30;			//PA5 : output waveform, PA4 : 0
	return result;
}