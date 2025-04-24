#ifndef EXE_INTERRUPT_REGISTER_H_
#define EXE_INTERRUPT_REGISTER_H_

#define MCUCR (*((volatile uint8_t *)0x55))  // External Interrupt Control Register A
#define MCUCSR (*((volatile uint8_t *)0x54)) // External Interrupt Control Register B (for INT2, unused)
#define EIMSK (*((volatile uint8_t *)0x3D))  // External Interrupt Mask Register (replaces GICR)
#define EIFR (*((volatile uint8_t *)0x3C))   // External Interrupt Flag Register (replaces GIFR)

#endif /* MCAL_EXTERNAL_INTERRUPT_DRIVER_EXE_INTERRUPT_REGISTER_H_ */