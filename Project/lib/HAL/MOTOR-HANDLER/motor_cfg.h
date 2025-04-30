#ifndef MOTOR_CFG_H_
#define MOTOR_CFG_H_

#include "GPIO/GPIO_int.h"

// Motor control pins (e.g., IN1, IN2 for L298N H-bridge)
#define MOTOR_LEFT_PORT PORT_D
#define MOTOR_LEFT_PIN PIN_4
#define MOTOR_RIGHT_PORT PORT_D
#define MOTOR_RIGHT_PIN PIN_5
#define MOTOR_ENABLE_PORT PORT_D
#define MOTOR_ENABLE_PIN PIN_3 // PWM pin (OC2B)
#endif