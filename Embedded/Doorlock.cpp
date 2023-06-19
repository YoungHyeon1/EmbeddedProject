#include "mbed.h"
#include "Doorlock.h"
#include "motordriver.h"
#include "Adafruit_GFX/Adafruit_SSD1306.h"

int X, Y;

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

/*	Door Function 	*/

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

/* System Function */

// Compare to user-entered password
int Checking_PWD(int *pwd, int *user_pwd){
	char pwd_str[4], user_pwd_str[4];
	*pwd_str = *pwd;
	*user_pwd_str = *user_pwd;
	
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

// Joystick function
void readJoystick(){
	X = xAxis * 100;
	Y = xAxis * 100;
	
	X = NEUTRAL_VALUE - X;
	if(abs(X) <= 1) X = 0;
	Y = NEUTRAL_VALUE - Y;
	if(abs(Y) <= 1) Y = 0;
}

void Control_Motor(){
	if(X > 0) motorA.setDir(FORWARD_DIR);
	else if(X < 0) motorA.setDir(BACKWARD_DIR);
	if(Y > 0) motorA.incSpeed(0.05);
	else if(Y < 0) motorA.incSpeed(-0.05);
}

// OLED Doorlock
void Doorlock_Display(){
	int textSize = 1;
	
	myOLED.clearDisplay();
	myOLED.drawRect(10, 10, 10, 10, WHITE);
	myOLED.setTextSize(textSize);
	for(int i = 1; i <= 9; i++){
		myOLED.setTextCursor(44, 14);
		myOLED.writeChar(0x30 + i);
	}
	
	myOLED.display();
}