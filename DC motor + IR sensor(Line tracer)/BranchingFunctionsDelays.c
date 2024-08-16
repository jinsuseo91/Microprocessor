// DC Motor Driver for TM4C123
#include "TExaS.h"
#include "tm4c123gh6pm.h"
unsigned long H,L, PE3, PE2, PE1;
unsigned long pre1, pre2, pre3;
void DisableInterrupts(void);
void EnableInterrupts(void);
void PLL_Init(void);
void WaitForInterrupt(void);
void PORTA_init(void);
void PORTE_init(void);
void Switch_Init(void);

void Motor_Init(void){
   SYSCTL_RCGC2_R |= 0x00000001;
   L = H = 40000;
   GPIO_PORTA_AMSEL_R &= ~0x60;
   GPIO_PORTA_PCTL_R &= ~0x0FF00000;
   GPIO_PORTA_DIR_R |= 0x60;
   GPIO_PORTA_DR8R_R |= 0x60;
   GPIO_PORTA_AFSEL_R &= ~0x60;
   GPIO_PORTA_DEN_R |= 0x60;
   GPIO_PORTA_DATA_R &= ~0x60;
   NVIC_ST_CTRL_R = 0;
   NVIC_ST_RELOAD_R = L-1;
   NVIC_ST_CURRENT_R = 0;
   NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000;
   NVIC_ST_CTRL_R = 0x00000007;
}

void PORTA_init(void){
      unsigned long volatile delay;
      SYSCTL_RCGC2_R |= 0x00000001;
      delay = SYSCTL_RCGC2_R;
      GPIO_PORTA_AMSEL_R &= ~0x0C; //PA2,3 disable
      GPIO_PORTA_PCTL_R &= ~0x0000FF00; //configure PA2,3
      GPIO_PORTA_DIR_R |= 0x08; //PA3 output 1
      GPIO_PORTA_DIR_R &= ~0x04; //PA2 input 0
      GPIO_PORTA_AFSEL_R &= ~0x0C; //disable PA2,3
      GPIO_PORTA_DEN_R |= 0x0C; //set PA2,3
      GPIO_PORTA_PUR_R &= ~0x0C;//clear PA2,3
      GPIO_PORTA_DATA_R &= ~0X0C;   //clear PA3
}

void PORTE_init(void){
      unsigned long volatile delay;
      SYSCTL_RCGC2_R |= 0x0000010;
      delay = SYSCTL_RCGC2_R;
      GPIO_PORTE_AMSEL_R &= ~0x0E; //PE1,2,3 disable
      GPIO_PORTE_PCTL_R &= ~0x0000FFF0; //configure PE1,2,3
      GPIO_PORTE_DIR_R &= ~0x0E; //PE1,2,3 input 0
      GPIO_PORTE_AFSEL_R &= ~0x0E; //disable PE1,2,3
      GPIO_PORTE_DEN_R |= 0x0E; //set PE1,2,3
      GPIO_PORTE_PUR_R |= 0x0E;//clear PE1,2,3
      GPIO_PORTE_DATA_R &= ~0X0E;   //clear PE1,2,3
}

void SysTick_Handler(void) {
    if (GPIO_PORTA_DATA_R & 0x60) {
        GPIO_PORTA_DATA_R &= ~0x60;
        NVIC_ST_RELOAD_R = L - 1;
    } else {
        GPIO_PORTA_DATA_R |= 0x60;
        NVIC_ST_RELOAD_R = H - 1;
    }
}

int main(void){
   unsigned long volatile delay;
   DisableInterrupts();
   PLL_Init();
   Motor_Init();
   PORTA_init();
   PORTE_init();
   EnableInterrupts();
   while(1){
        WaitForInterrupt();
        PE3 = GPIO_PORTE_DATA_R&0x08;
        PE2 = GPIO_PORTE_DATA_R&0x04;
        PE1 = GPIO_PORTE_DATA_R&0x02;

						if(PE2){//직진
								L=70000;
								H = 80000 - L;
								pre1 = PE1;
								pre3 = PE3;
            }
            else if(PE1){
							if(PE3){//양쪽이 검은색일때
								L=70000;
								H = 80000 - L;
							}
							else{//왼쪽만 검은색일때
								GPIO_PORTA_DATA_R &= ~0x40; // 좌회전
								pre1 = PE1;
								pre3 = PE3;
							}
            }
            else if(PE3){//오른쪽만 검은색일때
               GPIO_PORTA_DATA_R &= ~0x20;//우회전
								pre1 = PE1;
								pre3 = PE3;
            }
						else{//검은색을 놓쳤을때
							if(pre1){//이전값이 왼쪽이 검은색이었다면
								GPIO_PORTA_DATA_R &= ~0x40; // 좌회전
							}
							else if(pre3){//이전값이 오른쪽이 검은색이었다면
								GPIO_PORTA_DATA_R &= ~0x20; // 우회전
							}	
							else{//직진
								L = 70000;
								H = 80000- L;
							}
            }
  }
}