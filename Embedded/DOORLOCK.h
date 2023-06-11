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

int x, y;

// edge Button
typedef enum{
	NO_EDGE=0, RISING_EDGE, FALLING_EDGE
}	edge_t;

edge_t OPEN_BTN_EDGE(){
	edge_t edge = NO_EDGE;
	
	static int prevState = 1;
	int currState = openBtn;
	
	buzzer = 0.5;
	if(currState != prevState) {
		wait_ms(50);		// debounsing delay 50m/s
		currState = openBtn;
		if(currState != prevState){
			if(currState == 1) edge = RISING_EDGE;
			else edge = FALLING_EDGE;
			prevState = currState;
		}
	}
	
	return edge;
}

edge_t ENTER_BTN_EDGE(){
	edge_t edge = NO_EDGE;
	
	static int prevState = 1;
	int currState = enterBtn;
	
	buzzer = 0.5;
	if(currState != prevState) {
		wait_ms(50);		// debounsing delay 50m/s
		currState = enterBtn;
		if(currState != prevState){
			if(currState == 1) edge = RISING_EDGE;
			else edge = FALLING_EDGE;
			prevState = currState;
		}
	}
	
	return edge;
}


// Door function
// Opening Door
void Open_Door(){
	door_work.forward(0.5);
	greenLed = 1;
	redLed = 0;
	if(motortm.read() > 1.0) {
		door_work.stop();
		motortm.reset();
	}
}

// Closing Door
void Close_Door() {
	door_work.backward(0.5);
	greenLed = 0;
	redLed = 1;
	if(motortm.read() > 1.0) {
		door_work.stop();
		motortm.reset();
	}
}

// If incorrect password
void Incorrect_Number_Beep(){
	if(buzzertm.read_ms() >= 250){
		buzzer = buzzer.read() == 0 ? 0.5 : 0;
		buzzertm.reset();
	}
}

// if correct password or when Door is open
void Open_Door_Beep(){
	int trans = 0;
	while(trans <= 5){
		if(buzzertm.read_ms() >= 500) {
			buzzer = buzzer.read() == 0 ? 0.5 : 0;
			buzzertm.reset();
			trans++;
		}
		switch(trans){
			case 2:
				buzzer.period_us(BEEP_HALF_PERIOD - 396);
			case 4:
				buzzer.period_us(BEEP_HALF_PERIOD - 640);
		}
	}
	
	buzzer.period_us(BEEP_HALF_PERIOD);
}

// Close door
void Close_Door_Beep(){
	int trans = 0;
	buzzer.period_us(BEEP_HALF_PERIOD - 640);
	
	while(trans <= 5){
		if(buzzertm.read_ms() >= 500) {
			buzzer = buzzer.read() == 0 ? 0.5 : 0;
			buzzertm.reset();
			trans++;
		}
		switch(trans){
			case 2:
				buzzer.period_us(BEEP_HALF_PERIOD + 244);
			case 4:
				buzzer.period_us(BEEP_HALF_PERIOD + 640);
		}
	}
}

int Checking_PWD(int pwd, int user_pwd){
	char pwd_str[5], user_pwd_str[5];
	*pwd_str = pwd;
	*user_pwd_str = user_pwd;
	
	bool check = true;
	
	for(int i = 0; pwd_str[i] != '\0'; i++){
		if(pwd_str[i] != user_pwd_str[i]){
			check = false;
			break;
		}
	}
	
	if(check == false) return Incorrect;
	else if(check == true) return Correct;
}

void readJoystick(){
	x = xAxis * 100;
	y = xAxis * 100;
	
	x = NEUTRAL_VALUE - x;
	if(abs(x) <= 1) x = 0;
	y = NEUTRAL_VALUE - y;
	if(abs(y) <= 1) y = 0;
}

void Control_Motor(){
	if(x > 0) motorA.setDir(FORWARD_DIR);
	else if(x < 0) motorA.setDir(BACKWARD_DIR);
	if(y > 0) motorA.incSpeed(0.05);
	else if(y < 0) motorA.incSpeed(-0.05);
}

#endif