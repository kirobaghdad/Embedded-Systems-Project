#ifndef SPI_INTERFACE_H_
#define SPI_INTERFACE_H_

#include "std_types.h"
#include "SPI-DRIVER/SPI_Configuration.h"

// SPI Function Prototypes
uint8_t SPI_InitMaster(SPI_Mode_t mode, SPI_DataOrder_t dataOrder, SPI_ClockRate_t clockRate);
uint8_t SPI_InitSlave(SPI_Mode_t mode, SPI_DataOrder_t dataOrder);
uint8_t SPI_Transmit(uint8_t data);
uint8_t SPI_Receive(uint8_t *data);
uint8_t SPI_Transceive(uint8_t data, uint8_t *received_data);

#endif /* MCAL_SPI_DRIVER_SPI_INT_H_ */