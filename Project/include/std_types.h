#ifndef STD_TYPES_H_
#define STD_TYPES_H_
#include <stdint.h>
#define NULL_PTR ((void *)(0))
#define NULL_CHAR ('\0')
#define INITIAL_VALUE_ZERO (0)
#define E_NOK (0)
#define E_OK (1)
#define MSG_DELIM '#'

typedef struct
{
    uint8_t port;
    uint8_t pin;
} IO;
typedef float float32_t;
typedef double float64_t;
typedef long double float80_t;
typedef enum
{
    COM_DISCONNECT = 0b00,
    COM_NON_INVERTING = 0b10,
    COM_INVERTING = 0b11
} COM_Status_t;
#endif