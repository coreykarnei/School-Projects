// ADC_Init intializes PE2 as an analog input, ADC_In returns input data from 0-4095
// put Michael Darden mkd788 & Brent Atchison bma862 4/28/2015
#include <stdint.h>
#include "tm4c123gh6pm.h"
uint32_t delay=0;

// ADC initialization function 
// Input: none
// Output: none
/*void ADC1_Init(void){ 
// Setup PE2 as analog input
	SYSCTL_RCGCGPIO_R |= 0x10;		// 1) activate clock for Port E
	while ((SYSCTL_PRGPIO_R&0x10) == 0){};
	GPIO_PORTE_DIR_R &= ~0x04;		// 2) make PE2 input
	GPIO_PORTE_AFSEL_R |= 0x04;		// 3) enable alternate fun on PE2
	GPIO_PORTE_DEN_R &= ~0x04;		// 4) disable digital I/O on PE2
	GPIO_PORTE_AMSEL_R |= 0x04;		// 5) enable analog fun on PE2
	SYSCTL_RCGCADC_R |= 0x01;			// 6) activate ADC0 
	delay = SYSCTL_RCGCADC_R;     // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;     // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;     // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;
	ADC0_PC_R = 0x01;             // 7) configure for 125K 
	ADC0_SSPRI_R = 0x0123;        // 8) Seq 3 is highest priority
	ADC0_ACTSS_R &= ~0x0008;      // 9) disable sample sequencer 3
	ADC0_EMUX_R &= ~0xF000;       // 10) seq3 is software trigger
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1;  // 11) Ain1 (PE2)
	ADC0_SSCTL3_R = 0x0006;       // 12) no TS0 D0, yes IE0 END0
	ADC0_IM_R &= ~0x0008;         // 13) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;       // 14) enable sample sequencer 3
}*/

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
/*uint32_t ADC0_In(void){  
	uint32_t data;
	ADC0_PSSI_R = 0x08;
	while((ADC0_RIS_R&0x08)==0){};
	data = ADC0_SSFIFO3_R&0xFFFF;
	ADC0_ISC_R = 0x08;
  return data; 
}*/

// Initializes ADC8 and ADC9 sampling
// 125k max sampling
// SS2 triggering event: software trigger, busy-wait sampling
// SS2 1st sample source: Ain9 (PE4)
// SS2 2nd sample source: Ain8 (PE5)
// SS2 interrupts: enabled after 2nd sample but not promoted to controller
void ADC0_Init(void){ 
  volatile uint32_t delay;                         
//  SYSCTL_RCGC0_R |= 0x00010000; // 1) activate ADC0 (legacy code)
  SYSCTL_RCGCADC_R |= 0x00000001; // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // 1) activate clock for Port E
  delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to stabilize
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTE_DIR_R &= ~0x30;      // 3) make PE4 PE5 input
  GPIO_PORTE_AFSEL_R |= 0x30;     // 4) enable alternate function on PE4 PE5
  GPIO_PORTE_DEN_R &= ~0x30;      // 5) disable digital I/O on PE4 PE5
                                  // 5a) configure PE4 as ?? (skip this line because PCTL is for digital only)
  GPIO_PORTE_PCTL_R = GPIO_PORTE_PCTL_R&0xFF00FFFF;
  GPIO_PORTE_AMSEL_R |= 0x30;     // 6) enable analog functionality on PE4 PE5
  ADC0_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x3210;          // 9) Sequencer 3 is lowest priority
  ADC0_ACTSS_R &= ~0x0004;        // 10) disable sample sequencer 2
  ADC0_EMUX_R &= ~0x0F00;         // 11) seq2 is software trigger
  ADC0_SSMUX2_R = 0x0089;         // 12) set channels for SS2
  ADC0_SSCTL2_R = 0x0060;         // 13) no TS0 D0 IE0 END0 TS1 D1, yes IE1 END1
  ADC0_IM_R &= ~0x0004;           // 14) disable SS2 interrupts
  ADC0_ACTSS_R |= 0x0004;         // 15) enable sample sequencer 2
}

//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: two 12-bit result of ADC conversions
// Samples ADC8 and ADC9 
// 125k max sampling
// software trigger, busy-wait sampling
// data returned by reference
// data[0] is ADC8 (PE5) 0 to 4095
// data[1] is ADC9 (PE4) 0 to 4095
void ADC0_In(uint32_t data[2]){ 
  ADC0_PSSI_R = 0x0004;            // 1) initiate SS2
  while((ADC0_RIS_R&0x04)==0){};   // 2) wait for conversion done
  data[1] = ADC0_SSFIFO2_R&0xFFF;  // 3A) read first result
  data[0] = ADC0_SSFIFO2_R&0xFFF;  // 3B) read second result
  ADC0_ISC_R = 0x0004;             // 4) acknowledge completion
}
