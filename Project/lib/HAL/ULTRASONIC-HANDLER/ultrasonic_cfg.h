#include "DIO-DRIVER/dio_int.h"
#ifndef ULTRASONIC_CFG_H_
#define ULTRASONIC_CFG_H_
#include "CPU_Configuration.h"

#define Ultrasonic_ECHO_PORT	(PORT_D)
#define Ultrasonic_ECHO_PIN		(PIN_6)
#define Ultrasonic_TRIGGER_PORT		(PORT_D)
#define Ultrasonic_TRIGGER_PIN		(PIN_7)
#define TRIGGER_DELAY        (10u)
#define F_PRESCALER     (8u)
#define SPEED_OF_SOUND  (34000UL)


#endif 