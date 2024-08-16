#define TM_BASE_PWM 0x40031000

#define RCGC_TIMER_R *(volatile unsigned long *) 0x400FE604
#define RCGC_GPIO_R *(volatile unsigned long *) 0x400FE608
#define CLOCK_GPIOF 0x00000020
#define SYS_CLOCK_FREQUENCY 16000000

#define GPTM_TA_PRESCALE_PWM_R *(volatile unsigned long *) (TM_BASE_PWM + 0x038)
#define GPTM_TA_PRESCALE_MATCH_R *(volatile unsigned long *) (TM_BASE_PWM + 0x040)
#define GPTM_CONFIG_PWM_R *(volatile unsigned long *) (TM_BASE_PWM + 0x000)
#define GPTM_TA_MODE_PWM_R *(volatile unsigned long *) (TM_BASE_PWM + 0x004)
#define GPTM_CONTROL_PWM_R *(volatile unsigned long *) (TM_BASE_PWM + 0x00C)
#define GPTM_INT_MASK_PWM_R *(volatile unsigned long *) (TM_BASE_PWM + 0x018)
#define GPTM_INT_CLEAR_PWM_R *(volatile unsigned long *) (TM_BASE_PWM + 0x024)
#define GPTM_TA_IL_PWM_R *(volatile unsigned long *) (TM_BASE_PWM + 0x028)
#define GPTM_TA_MATCH_PWM_R *(volatile unsigned long *) (TM_BASE_PWM + 0x030)
	
#define GPIO_PORTF_AFSEL_R *((volatile unsigned long *) 0x40025420)
#define GPIO_PORTF_PCTL_R *((volatile unsigned long *) 0x4002552C)
#define GPIO_PORTF_DEN_R *((volatile unsigned long *) 0x4002551C)

#define TIM_16_BIT_CONFIG 0x00000004
#define TIM_PERIODIC_MODE 0x00000002
#define TIM_A_ENABLE      0x00000001


#define TIM_PWM_MODE      0x0000000A
#define TIM_CAPTURE_PWM_MODE  0x00000004

#define TIM_A_INTERVAL_PWM 64000
#define TIM_A_MATCH 32000

#define TIM_A_PRESCALE_VALUE 0x14
#define TIM_A_PRESCALE_MATCH_VALUE 0x14

void Timer1A_Init(void);
void servoNeutral(void);
void servoRight( void);
void servoLeft( void);
void servoConfig(void);
