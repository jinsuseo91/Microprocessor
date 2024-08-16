enum InputPorts{
  ADC0_AIN1_PIN_PE1 = 2,
	ADC0_AIN1_PIN_PE2 = 2,
	ADC0_AIN1_PIN_PE0 = 2
};

enum OutputPorts{
  LED_PIN_PF2 = 5
};

// ************TExaS_Init*****************
// Initialize grader, triggered by timer 5A
// This needs to be called once 
// Inputs: iport input(s) connected to this port
//         oport output(s) connected to this port
// Outputs: none
void TExaS_Init(enum InputPorts iport, enum InputPorts inport2, enum InputPorts iport3, enum OutputPorts oport);

// ************TExaS_Stop*****************
// Stop the transfer 
// Inputs:  none
// Outputs: none
void TExaS_Stop(void);