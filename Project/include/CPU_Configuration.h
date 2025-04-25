#ifndef CPU_CONFIGURATION_H_
#define CPU_CONFIGURATION_H_

    #ifndef F_CPU
        #define F_CPU 16000000UL // 16 MHz
    #endif
// Bit Manipulation Macros
#define SET_BIT(REG, BIT) ((REG) |= (1 << (BIT)))
#define CLR_BIT(REG, BIT) ((REG) &= ~(1 << (BIT)))
#define GET_BIT(REG, BIT) ((REG) & (1 << (BIT)))
#define TOGGLE_BIT(REG, BIT) ((REG) ^= (1 << (BIT)))

#endif