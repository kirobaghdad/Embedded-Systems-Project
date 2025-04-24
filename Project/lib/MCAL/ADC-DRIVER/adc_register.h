#ifndef ADC_REGISTER_H_
#define ADC_REGISTER_H_

#define ADMUX (*((volatile uint8_t *)0x7C))  // ADC Multiplexer Selection Register
#define ADCSRA (*((volatile uint8_t *)0x7A)) // ADC Control and Status Register A
#define ADCSRB (*((volatile uint8_t *)0x7B)) // ADC Control and Status Register B
#define ADC (*((volatile uint16_t *)0x78))   // ADC Data Register (16-bit)
#define ADCL (*((volatile uint8_t *)0x78))   // ADC Data Register Low Byte
#define ADCH (*((volatile uint8_t *)0x79))   // ADC Data Register High Byte

#endif /* MCAL_ADC_DRIVER_ADC_REGISTER_H_ */