#ifndef DOORLOCK
#define DOORLOCK

// include header file list
#include "mbed.h"
#include "motordriver.h"
#include "Adafruit_GFX/Adafruit_SSD1306.h"

// Define Pin
#define MOTOR_PWM_PIN		PA_7
#define MOTOR_DIR_PIN		PC_8
#define GREEN_LED_PIN		PA_13
#define YELLOW_LED_PIN	PB_10
#define RED_LED_PIN			PA_4
#define FIRST_BTN_PIN		PA_14
#define THIRD_BTN_PIN		PC_4

// Set Pin
DigitalIn	openBtn(FIRST_BTN_PIN);
DigitalIn enterBtn(THIRD_BTN_PIN);
DigitalOut greenLed(GREEN_LED_PIN);
DigitalOut yellowLed(YELLOW_LED_PIN);
DigitalOut redLed(RED_LED_PIN);
Motor door_work(MOTOR_PWM_PIN, MOTOR_DIR_PIN);
Timer tm;

// edge Button
typedef enum{
	NO_EDGE=0, RISING_EDGE, FALLING_EDGE
}	edge_t;

edge_t OPEN_BTN_EDGE(){
	edge_t edge = NO_EDGE;
	
	static int prevState = 1;
	int currState = openBtn;
	
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
	if(tm.read() > 1.0) {
		door_work.stop();
		tm.reset();
	}
}

// Closing Door
void Close_Door() {
	door_work.backward(0.5);
	greenLed = 0;
	redLed = 1;
	if(tm.read() > 1.0) {
		door_work.stop();
		tm.reset();
	}
}
#endif