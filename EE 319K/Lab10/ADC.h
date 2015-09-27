// ADC.h
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0

// Student names: Michael Darden & Brent Atchison
// Last modification date: 4/30/15

#ifndef ADC_H
#define ADC_H
// ADC initialization function 
// Input: none
// Output: none
void ADC0_Init(void);

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
void ADC0_In(uint32_t data[]);
#endif
