#ifndef DOORLOCK
#define DOORLOCK

// include header file list
#include "mbed.h"
#include "motordriver.h"
#include "Adafruit_GFX/Adafruit_SSD1306.h"

// Define Pin
#define BUZZER_PIN			PC_9
#define MOTOR_PWM_PIN		PA_7
#define MOTOR_DIR_PIN		PC_8
#define GREEN_LED_PIN		PA_13
#define YELLOW_LED_PIN	PB_10
#define RED_LED_PIN			PA_4
#define FIRST_BTN_PIN		PA_14
#define THIRD_BTN_PIN		PC_4
#define BEEP_HALF_PERIOD 1915
#define JS_X_AXIS		PC_2
#define JS_Y_AXIS		PC_3
#define MOTOR_A_PWM_PIN D11
#define MOTOR_A_DIR_PIN PC_8
#define Correct					1
#define Incorrect				0
#define NEUTRAL_VALUE  72

// Set Pin
PwmOut buzzer(BUZZER_PIN);
DigitalIn	openBtn(FIRST_BTN_PIN);
DigitalIn enterBtn(THIRD_BTN_PIN);
DigitalOut greenLed(GREEN_LED_PIN);
DigitalOut yellowLed(YELLOW_LED_PIN);
DigitalOut redLed(RED_LED_PIN);
Motor door_work(MOTOR_PWM_PIN, MOTOR_DIR_PIN);
I2C i2cMaster(I2C_SDA, I2C_SCL);
Adafruit_SSD1306_I2c myOLED(i2cMaster, D13, 0x78, 64, 128);
Timer motortm;
Timer buzzertm;
Serial pc(USBTX, USBRX, 9600);
AnalogIn xAxis(JS_X_AXIS);
AnalogIn yAxis(JS_Y_AXIS);
Ticker jsTicker;
Ticker ctrlTicker;
Motor motorA(MOTOR_A_PWM_PIN, MOTOR_A_DIR_PIN);

// edge Button
typedef enum{
	NO_EDGE=0, RISING_EDGE, FALLING_EDGE
}	edge_t;

edge_t OPEN_BTN_EDGE();
edge_t ENTER_BTN_EDGE();
void Open_Door();
void Close_Door();
void Incorrect_Number_Beep();
void Open_Door_Beep();
void Close_Door_Beep();
int Checking_PWD(int pwd, int user_pwd);
void readJoystick();
void Control_Motor();

#endif