#include "tm4c123gh6pm.h"
#include "motor_left.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void); // Enable interrupts
void PLL_Init(void);
void WaitForInterrupt(void);

void Motor_left_port_Init(void){	//port A's 
   SYSCTL_RCGC2_R |= 0x00000001; // activate clock for port A
   GPIO_PORTA_AMSEL_R &= ~0x0C; // disable analog functionality on PA5
   GPIO_PORTA_PCTL_R &= ~0x00F00000; // configure PA5 as GPIO
   GPIO_PORTA_DIR_R |= 0x0C; // make PA5 out
   GPIO_PORTA_DR8R_R |= 0x0C; // enable 8 mA drive on PA5
   GPIO_PORTA_AFSEL_R &= ~0x0C; // disable alt funct on PA5
   GPIO_PORTA_DEN_R |= 0x0C; // enable digital I/O on PA5
   GPIO_PORTA_DATA_R &= ~0x0C; // make PA5 low
}

void Delay(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 40000;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms
  }
}

void Motor_left_Init(void){
	 DisableInterrupts(); // disable interrupts while initializing
   PLL_Init();
   Motor_left_port_Init(); // output from PA5, SysTick interrupts
   EnableInterrupts(); // enable after all initialization are done
}

void go_left(void){
		if ((GPIO_PORTA_DATA_R & 0x08) == 0x80){
			GPIO_PORTA_DATA_R &= ~0x80;
		}
		else{
			GPIO_PORTA_DATA_R |= 0x80;
		}
		
		Delay(1);

		if ((GPIO_PORTA_DATA_R & 0x08) == 0x80){
			GPIO_PORTA_DATA_R &= ~0x80;
		}
		else{
			GPIO_PORTA_DATA_R |= 0x80;
		}
		
		Delay(1);
}

unsigned long Get_Waveform_left(void){
	unsigned long result;
	result = GPIO_PORTA_DATA_R & 0x0C;			//PA3 : output waveform, PA2 : 0
	return result;
}


/*
#include "tm4c123gh6pm.h"
#include "motor_left.h"

unsigned long HL, LL;

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void); // Enable interrupts
void PLL_Init(void);
void WaitForInterrupt(void);

void Motor_left_port_Init(void){	//port A's 
   SYSCTL_RCGC2_R |= 0x00000001; // activate clock for port A
   LL = HL = 40000; // 50%
   GPIO_PORTA_AMSEL_R &= ~0x0C; // disable analog functionality on PA5
   GPIO_PORTA_PCTL_R &= ~0x00F00000; // configure PA5 as GPIO
   GPIO_PORTA_DIR_R |= 0x0C; // make PA5 out
   GPIO_PORTA_DR8R_R |= 0x0C; // enable 8 mA drive on PA5
   GPIO_PORTA_AFSEL_R &= ~0x0C; // disable alt funct on PA5
   GPIO_PORTA_DEN_R |= 0x0C; // enable digital I/O on PA5
   GPIO_PORTA_DATA_R &= ~0x0C; // make PA5 low
   NVIC_ST_CTRL_R = 0; // disable SysTick during setup
   NVIC_ST_RELOAD_R = LL-1; // reload value for 500us
   NVIC_ST_CURRENT_R = 0; // any write to current clears it
   NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
   NVIC_ST_CTRL_R = 0x00000007; // enable with core clock and interrupts
}

void SysTick_Handler(void){
   if(GPIO_PORTA_DATA_R&0x08){ // toggle PA5
      GPIO_PORTA_DATA_R &= ~0x08; // make PA5 low
      NVIC_ST_RELOAD_R = LL-1; // reload value for low phase
   } else{
      GPIO_PORTA_DATA_R |= 0x08; // make PA5 high
      NVIC_ST_RELOAD_R = HL-1; // reload value for high phase
   }
}

void Motor_left_Init(void){
	 DisableInterrupts(); // disable interrupts while initializing
   PLL_Init();
   Motor_left_port_Init(); // output from PA5, SysTick interrupts
   EnableInterrupts(); // enable after all initialization are done
}

void Set_Speed_left(unsigned long data){
	if(((data & 0x02) == 1) && (LL > 20000)){
		LL = LL - 8000;
	} else if (((data & 0x08) == 1) && (LL < 60000)){
		LL = LL + 8000;
	}
	
	HL = 80000 - LL;
}

unsigned long Get_Waveform_left(void){
	unsigned long result;
	result = GPIO_PORTA_DATA_R & 0x0C;			//PA3 : output waveform, PA2 : 0
	return result;
}
*/