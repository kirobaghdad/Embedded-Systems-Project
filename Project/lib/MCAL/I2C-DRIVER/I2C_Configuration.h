#ifndef I2C_CONFIGURATION_H_
#define I2C_CONFIGURATION_H_

#include "CPU_Configuration.h"
#include "DIO-DRIVER/dio_int.h"

// I2C clock frequency (TWBR values for 16 MHz CPU, prescaler=1)
typedef enum
{
	I2C_CLOCK_100KHZ = 72, // TWBR for 100 kHz: (16e6/(16+2*72*1)) = 100 kHz
	I2C_CLOCK_400KHZ = 12  // TWBR for 400 kHz: (16e6/(16+2*12*1)) = 400 kHz
} I2C_Clock_t;

// I2C prescaler options
typedef enum
{
	I2C_PRESCALER_1 = 0b00,
	I2C_PRESCALER_4 = 0b01,
	I2C_PRESCALER_16 = 0b10,
	I2C_PRESCALER_64 = 0b11
} I2C_Prescaler_t;

// Slave address type (7-bit)
typedef uint8_t I2C_SlaveAddress_t;

// Default configuration
#define I2C_DEFAULT_CLOCK I2C_CLOCK_100KHZ
#define I2C_DEFAULT_PRESCALER I2C_PRESCALER_1
#define I2C_DEFAULT_SLAVE_ADDR 0x01
#define I2C_DEFAULT_PULLUP PULLUP_ENABLED // Enable internal pull-ups for SDA/SCL

// I2C pins (fixed on ATmega328P)
#define I2C_SDA_PORT PORT_C
#define I2C_SDA_PIN PIN_4 // PC4
#define I2C_SCL_PORT PORT_C
#define I2C_SCL_PIN PIN_5 // PC5

#endif /* MCAL_I2C_DRIVER_I2C_CONFIG_H_ */