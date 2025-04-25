#ifndef I2C_INTERFACE_H_
#define I2C_INTERFACE_H_

#include "std_types.h"
#include "I2C-DRIVER/I2C_Configuration.h"

// Master Functions
uint8_t I2C_InitMaster(I2C_Clock_t clock, I2C_Prescaler_t prescaler);
uint8_t I2C_MasterSendStartCondition(void);
uint8_t I2C_MasterSendStopCondition(void);
uint8_t I2C_MasterSendRepeatedStartCondition(void);
uint8_t I2C_MasterSendSlaveAddressWithWrite(I2C_SlaveAddress_t slaveAddress);
uint8_t I2C_MasterSendSlaveAddressWithRead(I2C_SlaveAddress_t slaveAddress);
uint8_t I2C_MasterTransmitDataByte(uint8_t data);
uint8_t I2C_MasterReceiveDataByteWithACK(uint8_t *data);
uint8_t I2C_MasterReceiveDataByteWithNoACK(uint8_t *data);

// Slave Functions
uint8_t I2C_InitSlave(I2C_SlaveAddress_t slaveAddress);
uint8_t I2C_SlaveTransmitDataByte(uint8_t data);
uint8_t I2C_SlaveReceiveDataByte(uint8_t *data);

#endif 