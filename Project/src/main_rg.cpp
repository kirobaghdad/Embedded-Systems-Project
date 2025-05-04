// #include <arduino.h>
#include <util/delay.h>

#include "BLUETOOTH-HANDLER\bluetooth_int.h"
#include "ULTRASONIC-HANDLER/ultrasonic_int.h"
#include "GPIO/GPIO_int.h"
#include "TIMER1/TIMER1_int.h"
#include "std_types.h"
#include "MOTOR-HANDLER/motor_int.h"
#include <HardwareSerial.h>

#define MOV_DELAY 2000
#define DISTANCE_THRESHOLD 20
#define MOTOR_SPEED 150 // Speed of the car (0-255)
#define DELAY_PER_CM 20
#define PARKING_GAP_MARGIN 5
#define CAR_LENGTH 22
#define CAR_WIDTH 10

// #define DISTANCE_THRESHOLD 0.5
static MOTOR_CONFIG motor_config_L = {{{PORT_C, PIN_1}, {PORT_C, PIN_0}, {PORT_D, PIN_6}}, .PWM_Callback = TIMER0_CallBack_A};
static MOTOR_CONFIG motor_config_R = {{{PORT_C, PIN_2}, {PORT_C, PIN_3}, {PORT_D, PIN_5}}, .PWM_Callback = TIMER0_CallBack_B};
static Timer0_params Timer0_CFG = {
    .timer0_mode = TIMER0_FAST_PWM_MODE,
    .timer0_prescaler = TIMER0_PRESCALER_64, // Have No idea why
    .compare_val = 255,                      // Dummy but Works for CTC.
    .com_rega = COM_NON_INVERTING,
    .com_regb = COM_NON_INVERTING,
    .OCR0A_val = MOTOR_SPEED,
    .OCR0B_val = MOTOR_SPEED,
};

static ultrasonic_config_t FRONT_SENSOR = {.echo = {PORT_B, PIN_0}, .trigger = {PORT_D, PIN_2}};
static ultrasonic_config_t RIGHT_SENSOR = {.echo = {PORT_B, PIN_0}, .trigger = {PORT_D, PIN_4}};
static ultrasonic_config_t LEFT_SENSOR = {.echo = {PORT_B, PIN_0}, .trigger = {PORT_D, PIN_7}};
static uint8_t PARKING_TYPE = '|';
static Timer1_params timer1_conf = {
    .timer1_mode = TIMER1_ICU_MODE,
    .timer1_prescaler = TIMER1_PRESCALER_64, // Have No idea why
    .compare_val = 255,                      // Dummy but Works for CTC.
    .com_rega = COM_NON_INVERTING,
    .com_regb = COM_NON_INVERTING,
    .OCR1A_val = 175,
    .OCR1B_val = 175,
    .icu_edge = ICU_RISING_EDGE};

static uint8_t parking_side;                            // 'R' or 'L'
static ultrasonic_config_t *SLOT_SIDE_SENSOR = nullptr; // Function declarations

void initMotors();
void moveForward();
void moveBackward();
void moveRight();
void moveLeft();
void stopMotors();
void Car_RotateRight90();
void Car_RotateLeft90();

void blinkLed(){
    GPIO_SetPinMode(PORT_B, PIN_5, OUTPUT); 
    for (uint8_t i = 0; i < 10; i++) {
        SET_BIT(PORTB, PIN_5); 
        _delay_ms(100);        
        CLR_BIT(PORTB, PIN_5); 
        _delay_ms(100);        
    }
}
void initMotors()
{
    MOTOR_u8MotorInit(&motor_config_L, &Timer0_CFG);
    MOTOR_u8MotorInit(&motor_config_R, &Timer0_CFG);
    // Ensure motors are stopped initially
    // stopMotors();
}

void perform_parking()
{
    stopMotors();
    _delay_ms(1000);

    // return a little bit
    moveBackward();
    _delay_ms(300);

    if (parking_side == 'R')
    {
        stopMotors();
        _delay_ms(250);
        Car_RotateLeft90();
        stopMotors();
        _delay_ms(10);
        moveBackward();
        _delay_ms(350);
        if (PARKING_TYPE == '-')
        {
            stopMotors();
            _delay_ms(1000);
            Car_RotateRight90();
        }
    }
    else
    {
        stopMotors();
        _delay_ms(250);
        Car_RotateRight90();
        stopMotors();
        _delay_ms(10);
        moveBackward();
        _delay_ms(350);
        if (PARKING_TYPE == '-')
        {
            stopMotors();
            _delay_ms(1000);
            Car_RotateLeft90();
        }
    }
    blinkLed();
    stopMotors();
 
}
void Car_RotateRight90()
{
    moveRight();
    // if(PARKING_TYPE=='|')
    // {
    _delay_ms(490);
    // }
    // else
    // _delay_ms(200);
    stopMotors();
}
void Car_RotateLeft90()
{
    moveLeft();
    // if(PARKING_TYPE=='|')
    // {
    _delay_ms(490);
    // }
    // else
    //     _delay_ms(200); // TODO: Should be adjusted based on actual testing
    stopMotors();
}

void moveForward()
{
    MOTOR_u8RightRotate(&motor_config_L, MOTOR_SPEED, &Timer0_CFG);
    MOTOR_u8RightRotate(&motor_config_R, MOTOR_SPEED, &Timer0_CFG);
}

void moveBackward()
{
    // Left motors backward
    MOTOR_u8LeftRotate(&motor_config_L, MOTOR_SPEED, &Timer0_CFG);
    MOTOR_u8LeftRotate(&motor_config_R, MOTOR_SPEED, &Timer0_CFG);
}

void moveRight()
{
    MOTOR_u8RightRotate(&motor_config_L, MOTOR_SPEED, &Timer0_CFG);
    MOTOR_u8LeftRotate(&motor_config_R, MOTOR_SPEED, &Timer0_CFG);
}
void moveLeft()
{
    MOTOR_u8LeftRotate(&motor_config_L, MOTOR_SPEED, &Timer0_CFG);
    MOTOR_u8RightRotate(&motor_config_R, MOTOR_SPEED, &Timer0_CFG);
}
void slot_detection_and_parking()
{
    uint16_t right_distance = 0;
    uint16_t left_distance = 0;
    uint16_t measured_gap = 0;
    uint8_t gap_found = 0;

    moveForward();

    while (1)
    {
        GPIO_SetPinValue(PORT_B, PIN_4, LOW); // Set echo pin to low
        GPIO_SetPinValue(PORT_B, PIN_5, LOW);
        right_distance = Ultrasonic_readDistance(&RIGHT_SENSOR, &timer1_conf);
        GPIO_SetPinValue(PORT_B, PIN_4, HIGH); // Set echo pin to low
        GPIO_SetPinValue(PORT_B, PIN_5, LOW);
        left_distance = Ultrasonic_readDistance(&LEFT_SENSOR, &timer1_conf);

        if (right_distance > DISTANCE_THRESHOLD)
        {
            if (!gap_found)
            {
                gap_found = 1;
                measured_gap = 0;
                parking_side = 'R';
                SLOT_SIDE_SENSOR = &RIGHT_SENSOR;
            }

            measured_gap += 1;
            _delay_ms(DELAY_PER_CM);

            if ((PARKING_TYPE == '-' && measured_gap >= (CAR_LENGTH + PARKING_GAP_MARGIN) / 2) ||
                (PARKING_TYPE == '|' && measured_gap >= (CAR_WIDTH + PARKING_GAP_MARGIN) / 2))
            {
                perform_parking();
                stopMotors();
                break;
            }
        }
        else if (left_distance > DISTANCE_THRESHOLD)
        {
            if (!gap_found)
            {
                gap_found = 1;
                measured_gap = 0;
                parking_side = 'L';
                SLOT_SIDE_SENSOR = &LEFT_SENSOR;
            }
            measured_gap += 1;
            _delay_ms(DELAY_PER_CM);
            if ((PARKING_TYPE == '-' && measured_gap >= (CAR_LENGTH + PARKING_GAP_MARGIN) / 2) ||
                (PARKING_TYPE == '|' && measured_gap >= (CAR_WIDTH + PARKING_GAP_MARGIN) / 2))
            {
                perform_parking();
                stopMotors();
                break;
            }
        }
        else
        {
            if (gap_found)
            {
                if ((PARKING_TYPE == '-' && measured_gap >= (CAR_LENGTH + PARKING_GAP_MARGIN) / 2) ||
                    (PARKING_TYPE == '|' && measured_gap >= (CAR_WIDTH + PARKING_GAP_MARGIN) / 2))
                {
                    stopMotors();
                    perform_parking();
                    stopMotors();
                    stopMotors();
                    perform_parking();
                    stopMotors();
                    
                    break;
                }
                else
                {
                    gap_found = 0;
                    measured_gap = 0;
                }
            }
            _delay_ms(100);
        }
    }
    
}

void stopMotors()
{
    // Stop all motors
    MOTOR_u8MotorOff(&motor_config_L, &Timer0_CFG);
    MOTOR_u8MotorOff(&motor_config_R, &Timer0_CFG);
}

void setup()
{
    BLUETOOTH_init();

    initMotors();
    GPIO_SetPinMode(PORT_B, PIN_4, OUTPUT);
    GPIO_SetPinMode(PORT_B, PIN_5, OUTPUT);

    Ultrasonic_init(&FRONT_SENSOR, &timer1_conf);
    Ultrasonic_init(&RIGHT_SENSOR, &timer1_conf);
    Ultrasonic_init(&LEFT_SENSOR, &timer1_conf);
}

void loop()
{
    char receivedChar = (char)BLUETOOTH_receiveChar();

    Serial.print(receivedChar);

    // start parking
    while (receivedChar != 's')
    {
        receivedChar = (char)BLUETOOTH_receiveChar();
    }

    receivedChar = (char)BLUETOOTH_receiveChar();

    while (1)
    {
        if (receivedChar == 'L')
        {
            PARKING_TYPE = '|';
            break;
        }
        else if (receivedChar == 'R')
        {
            PARKING_TYPE = '-';
            break;
        }

        receivedChar = (char)BLUETOOTH_receiveChar();
    }

    slot_detection_and_parking();

    stopMotors();
}

int main(void)
{
    setup();

    while (1)
        loop();

    return 0;
}