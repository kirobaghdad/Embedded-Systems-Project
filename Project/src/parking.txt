#include "std_types.h"
#include <util/delay.h>
extern "C" {
    #include "DIO-DRIVER/dio_int.h"
    #include "ULTRASONIC-HANDLER/ultrasonic_int.h"
    #include "LED-HANDLER/led_int.h"
    #include "BLUETOOTH-HANDLER/bluetooth_int.h"
}

// ====== Configuration ======
#define CAR_SPEED 50
#define PARKING_GAP_MARGIN 10
#define CAR_LENGTH 30
#define DISTANCE_THRESHOLD 15

#define LED_PORT PORT_B
#define LED_PIN PIN_5

static MOTOR_CONFIG motor_config_FL = { { {PORT_B, PIN_2}, {PORT_B, PIN_0}, {PORT_B, PIN_1} }, TIMER1_CallBack };
static MOTOR_CONFIG motor_config_FR = { { {PORT_B, PIN_4}, {PORT_B, PIN_5}, {PORT_D, PIN_3} }, TIMER2_CallBack };
static MOTOR_CONFIG motor_config_RL = { { {PORT_C, PIN_0}, {PORT_C, PIN_1}, {PORT_B, PIN_1} }, TIMER1_CallBack };
static MOTOR_CONFIG motor_config_RR = { { {PORT_D, PIN_0}, {PORT_D, PIN_1}, {PORT_D, PIN_3} }, TIMER2_CallBack };

static ultrasonic_config_t front_sensor = {{PORT_B, PIN_0}, {PORT_D, PIN_2}};
static ultrasonic_config_t right_sensor = {{PORT_B, PIN_0}, {PORT_D, PIN_4}};
static ultrasonic_config_t left_sensor  = {{PORT_B, PIN_0}, {PORT_D, PIN_7}};

static ultrasonic_config_t* SLOT_SIDE_SENSOR = NULL;
static uint8_t parking_side; // 'R' or 'L'

// ====== Function Prototypes ======
void Car_Init();
void Car_MoveForward();
void Car_MoveBackward();
void Car_TurnLeft();
void Car_TurnRight();
void Car_Stop();
void Car_RotateLeftInPlace();
void Car_RotateRightInPlace();
void Car_RotateRight45();
void Car_RotateLeft45();
void perform_parallel_parking();
void indicate_parking_success();
void wait_for_start_command();
void slot_detection_and_parking();

// ====== Car Movement Functions ======
void Car_Init() {
    MOTOR_u8MotorInit(&motor_config_FL);
    MOTOR_u8MotorInit(&motor_config_FR);
    MOTOR_u8MotorInit(&motor_config_RL);
    MOTOR_u8MotorInit(&motor_config_RR);
}

void Car_MoveForward() {
    MOTOR_u8RightRotate(&motor_config_FL, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_FR, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_RL, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_RR, CAR_SPEED);
}

void Car_MoveBackward() {
    MOTOR_u8LeftRotate(&motor_config_FL, CAR_SPEED);
    MOTOR_u8LeftRotate(&motor_config_FR, CAR_SPEED);
    MOTOR_u8LeftRotate(&motor_config_RL, CAR_SPEED);
    MOTOR_u8LeftRotate(&motor_config_RR, CAR_SPEED);
}

void Car_TurnLeft() {
    MOTOR_u8RightRotate(&motor_config_FL, CAR_SPEED/2);
    MOTOR_u8RightRotate(&motor_config_FR, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_RL, CAR_SPEED/2);
    MOTOR_u8RightRotate(&motor_config_RR, CAR_SPEED);
}

void Car_TurnRight() {
    MOTOR_u8RightRotate(&motor_config_FL, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_FR, CAR_SPEED/2);
    MOTOR_u8RightRotate(&motor_config_RL, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_RR, CAR_SPEED/2);
}

void Car_Stop() {
    MOTOR_u8MotorOff(&motor_config_FL);
    MOTOR_u8MotorOff(&motor_config_FR);
    MOTOR_u8MotorOff(&motor_config_RL);
    MOTOR_u8MotorOff(&motor_config_RR);
}

void Car_RotateLeftInPlace() {
    MOTOR_u8LeftRotate(&motor_config_FL, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_FR, CAR_SPEED);
    MOTOR_u8LeftRotate(&motor_config_RL, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_RR, CAR_SPEED);
}

void Car_RotateRightInPlace() {
    MOTOR_u8RightRotate(&motor_config_FL, CAR_SPEED);
    MOTOR_u8LeftRotate(&motor_config_FR, CAR_SPEED);
    MOTOR_u8RightRotate(&motor_config_RL, CAR_SPEED);
    MOTOR_u8LeftRotate(&motor_config_RR, CAR_SPEED);
}

void Car_RotateRight45() {
    Car_RotateRightInPlace();
    _delay_ms(300);
    Car_Stop();
}

void Car_RotateLeft45() {
    Car_RotateLeftInPlace();
    _delay_ms(300);
    Car_Stop();
}

// ====== Core Parking Functions ======

void wait_for_start_command() {
    uint8_t cmd;
    BLUETOOTH_init();
    BLUETOOTH_sendString((uint8_t *)"Send 'P' to start parking...\n");

    do {
        cmd = BLUETOOTH_receiveChar();
    } while (cmd != 'P');

    BLUETOOTH_sendString((uint8_t *)"Searching for slot automatically...\n");
}

void perform_parallel_parking() {
    if (parking_side == 'R') {
        Car_MoveBackward();
        Car_RotateRight45();
        _delay_ms(500);
        Car_Stop();

        Car_MoveBackward();
        Car_RotateLeft45();
        _delay_ms(500);
        Car_Stop();
    } else {
        Car_MoveBackward();
        Car_RotateLeft45();
        _delay_ms(500);
        Car_Stop();

        Car_MoveBackward();
        Car_RotateRight45();
        _delay_ms(500);
        Car_Stop();
    }

    Car_MoveBackward();
    _delay_ms(500);
    Car_Stop();
}

void indicate_parking_success() {
    LED_u8LedOn(LED_PORT, LED_PIN);
    Car_Stop();
    _delay_ms(3000);
    LED_u8LedOff(LED_PORT, LED_PIN);
}

void slot_detection_and_parking() {
    uint16_t right_distance = 0;
    uint16_t left_distance = 0;
    uint16_t measured_gap = 0;
    uint8_t gap_found = 0;

    Car_MoveForward();

    while (1) {
        right_distance = Ultrasonic_readDistance(&right_sensor);
        left_distance = Ultrasonic_readDistance(&left_sensor);

        if (right_distance > DISTANCE_THRESHOLD) {
            if (!gap_found) {
                gap_found = 1;
                measured_gap = 0;
                parking_side = 'R';
                SLOT_SIDE_SENSOR = &right_sensor;
            }
            measured_gap += 1;
            _delay_ms(100);
        } else if (left_distance > DISTANCE_THRESHOLD) {
            if (!gap_found) {
                gap_found = 1;
                measured_gap = 0;
                parking_side = 'L';
                SLOT_SIDE_SENSOR = &left_sensor;
            }
            measured_gap += 1;
            _delay_ms(100);
        } else {
            if (gap_found) {
                if (measured_gap >= (CAR_LENGTH + PARKING_GAP_MARGIN)/2) {
                    Car_Stop();
                    perform_parallel_parking();
                    indicate_parking_success();
                    break;
                } else {
                    gap_found = 0;
                    measured_gap = 0;
                }
            }
            _delay_ms(100);
        }
    }
}

// ====== Main ======

int main(void) {
    Car_Init();
    Ultrasonic_init(&front_sensor);
    Ultrasonic_init(&right_sensor);
    Ultrasonic_init(&left_sensor);
    DIO_u8SetPinMode(LED_PORT, LED_PIN, OUTPUT);

    while (1) {
        wait_for_start_command();
        slot_detection_and_parking();
    }

    return 0;
}
