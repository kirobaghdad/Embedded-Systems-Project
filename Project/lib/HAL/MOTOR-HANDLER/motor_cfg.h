#ifndef HAL_MOTOR_DRIVER_MOTOR_CONFIG_H_
#define HAL_MOTOR_DRIVER_MOTOR_CONFIG_H_

#include "DIO-DRIVER/dio_int.h"

// Motor control pins (e.g., IN1, IN2 for L298N H-bridge)
#define MOTOR_LEFT_PORT PORT_D
#define MOTOR_LEFT_PIN PIN_0
#define MOTOR_RIGHT_PORT PORT_D
#define MOTOR_RIGHT_PIN PIN_1

#endif /* HAL_MOTOR_DRIVER_MOTOR_CONFIG_H_ */