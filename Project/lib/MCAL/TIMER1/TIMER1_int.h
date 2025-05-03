#ifndef TIMER1_INT_H_
#define TIMER1_INT_H_
#include <avr/interrupt.h>
// #include "TIMER1/TIMER1_address.h"
#include "CPU_Configuration.h"
#include "std_types.h"
#define IS_VALID_MILLISECONDS(ms) ((ms) > 0)
#define IS_VALID_CALLBACK(func) ((func) != NULL_PTR)
typedef enum
{
    TIMER1_NORMAL_MODE,
    TIMER1_CTC_MODE,
    TIMER1_FAST_PWM_MODE,
    TIMER1_ICU_MODE
} TIMER1_Mode_t;
typedef enum
{
    ICU_FALLING_EDGE,
    ICU_RISING_EDGE
} ICU_Edge_t;
typedef enum
{
    TIMER1_PRESCALER_NO = 0b001,
    TIMER1_PRESCALER_8 = 0b010,
    TIMER1_PRESCALER_64 = 0b011,
    TIMER1_PRESCALER_256 = 0b100,
    TIMER1_PRESCALER_1024 = 0b101
} TIMER1_Prescaler_t;

typedef struct
{
    TIMER1_Mode_t timer1_mode;
    TIMER1_Prescaler_t timer1_prescaler;
    uint8_t compare_val;
    COM_Status_t com_rega;
    COM_Status_t com_regb;
    uint8_t OCR1A_val;
    uint8_t OCR1B_val;
    ICU_Edge_t icu_edge;
} Timer1_params;
static TIMER1_Mode_t TIMER1_Mode = TIMER1_NORMAL_MODE;   // Default mode
static void (*TIMER1_OverflowCallback)(void) = nullptr; // For Normal mode
static void (*TIMER1_CompareACallback)(void) = nullptr; // For CTC or PWM (OC0A)
static void (*TIMER1_CompareBCallback)(void) = nullptr; // For PWM (OC0B)
static void (*TIMER1_ICUCallback)(void) = nullptr;      // For ICU mode
static uint16_t TIMER1_RequiredMatches = 0;              // For CTC mode
static uint16_t TIMER1_CurrentMatches = 0;               // For CTC mode
static uint8_t TIMER1_CalculateTicks(Timer1_params *timer1_conf, uint16_t milliseconds, uint16_t *matches);
void TIMER1_SetICUCallback(void (*callbackFunc)(void));
void TIMER1_SetOverflowCallback(void (*callbackFunc)(void));
void TIMER1_INIT(Timer1_params *timer_conf);
void TIMER1_SetCompareACallback(void (*callbackFunc)(void));
void TIMER1_SetCompareBCallback(void (*callbackFunc)(void));
void TIMER1_Set_PWM_DutyCycle_A(Timer1_params *timer_conf, uint8_t speed);
void TIMER1_Set_PWM_DutyCycle_B(Timer1_params *timer_conf, uint8_t speed);
#endif