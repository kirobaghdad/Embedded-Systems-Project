#ifndef I2C_ADDRESS_H_
#define I2C_ADDRESS_H_

#include "std_types.h"

// TWI (I2C) Registers
#define TWCR (*((volatile uint8_t *)0xBC))  // TWI Control Register
#define TWSR (*((volatile uint8_t *)0xB9))  // TWI Status Register
#define TWDR (*((volatile uint8_t *)0xBB))  // TWI Data Register
#define TWBR (*((volatile uint8_t *)0xB8))  // TWI Bit Rate Register
#define TWAR (*((volatile uint8_t *)0xBA))  // TWI Address Register
#define TWAMR (*((volatile uint8_t *)0xBD)) // TWI Address Mask Register

// TWCR Bit Positions
#define TWINT 7 // Interrupt Flag
#define TWEA 6  // Enable Acknowledge
#define TWSTA 5 // Start Condition
#define TWSTO 4 // Stop Condition
#define TWWC 3  // Write Collision
#define TWEN 2  // TWI Enable
#define TWIE 0  // Interrupt Enable

// TWSR Bit Positions
#define TWPS0 0 // Prescaler Bit 0
#define TWPS1 1 // Prescaler Bit 1
#define TWS3 3  // Status Bit 3
#define TWS4 4  // Status Bit 4
#define TWS5 5  // Status Bit 5
#define TWS6 6  // Status Bit 6
#define TWS7 7  // Status Bit 7

// I2C Status Codes (masked with 0xF8)
#define I2C_START_TRANSMITTED 0x08          // Start condition sent
#define I2C_REPEATED_START_TRANSMITTED 0x10 // Repeated start sent
#define I2C_SLA_W_ACK 0x18                  // Slave address + write ACK
#define I2C_SLA_W_NO_ACK 0x20               // Slave address + write NACK
#define I2C_DATA_TRANSMITTED_ACK 0x28       // Data byte sent, ACK received
#define I2C_DATA_TRANSMITTED_NO_ACK 0x30    // Data byte sent, NACK received
#define I2C_SLA_R_ACK 0x40                  // Slave address + read ACK
#define I2C_SLA_R_NO_ACK 0x48               // Slave address + read NACK
#define I2C_DATA_RECEIVED_ACK 0x50          // Data byte received, ACK sent
#define I2C_DATA_RECEIVED_NO_ACK 0x58       // Data byte received, NACK sent

#endif /* MCAL_I2C_DRIVER_I2C_REGISTER_H_ */