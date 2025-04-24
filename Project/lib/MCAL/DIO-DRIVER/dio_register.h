#ifndef DIO_REGISTER_H_
#define DIO_REGISTER_H_
#include "std_types.h"

#define PORTB_REG (*((volatile uint8_t *)0x25))
#define PINB_REG (*((volatile uint8_t *)0x23))
#define DDRB_REG (*((volatile uint8_t *)0x24))

#define PORTC_REG (*((volatile uint8_t *)0x28))
#define PINC_REG (*((volatile uint8_t *)0x26))
#define DDRC_REG (*((volatile uint8_t *)0x27))

#define PORTD_REG (*((volatile uint8_t *)0x2B))
#define PIND_REG (*((volatile uint8_t *)0x29))
#define DDRD_REG (*((volatile uint8_t *)0x2A))
#endif
