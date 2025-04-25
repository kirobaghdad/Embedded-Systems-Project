#ifndef ADC_INT_H_
#define ADC_INT_H_
#include "std_types.h"

#define AUTO_TRIGGER 0
#define SOFTWARE_TRIGGER 1

#define _8_BIT_MODE 0
#define _10_BIT_MODE 1

#define INTERRUPT_MODE 0
#define POLLING_MODE 1

#define ADC_0 (uint8_t)0
#define ADC_1 (uint8_t)1
#define ADC_2 (uint8_t)2
#define ADC_3 (uint8_t)3
#define ADC_4 (uint8_t)4
#define ADC_5 (uint8_t)5

#define FREE_RUNNING 0
#define ANALOG_COMPARATOR 1
#define EXTERNAL_INT0 2
#define TIMER0_COMPARE 3
#define TIMER0_OVERFLOW 4
#define TIMER1_COMPARE_B 5
#define TIMER1_OVERFLOW 6
#define TIMER1_CAPTURE 7

#define AREF_VOLTAGE 0
#define VCC 1
#define INTERNAL_2_56V 2

/*
 * Function prototypes
 */
void ADC_vidAdcInit(uint8_t voltage_reference);
void ADC_vidAdcPinInit(uint8_t adc_pin);
void ADC_vidAdcGetRead(uint8_t adc_pin);
void ADC_vidGetAutoTriggerRead(void);
void ADC_vidSetAutoTriggerMode(uint8_t mode);
void ADC_vidSetReadingVariable(uint16_t *adc_read);
void ADC_vidSetCallBack(void (*func)(void));

#endif