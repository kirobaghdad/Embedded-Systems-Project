#ifndef STD_TYPES_H_
#define STD_TYPES_H_
#include<stdint.h>
#define NULL_PTR ((void *)0)
#define NULL_CHAR ('\0')
#define INITIAL_VALUE_ZERO (0)

/*
 * Error status codes
 */
#define E_OK (uint8_t)0
#define E_NOK (uint8_t)1

typedef float float32_t;
typedef double float64_t;
typedef long double float80_t;

#endif