#include "mbed.h"
#include "Doorlock.h"
#include "motordriver.h"
#include "Adafruit_GFX/Adafruit_SSD1306.h"
#include "DHT22.h"

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

// OLED Doorlock
int Password_OLED(int *enter_pwd){
	int number = 0;
	
	while(enter_pwd[4] != '\0'){
		myOLED.clearDisplay();
		myOLED.drawRect(30, 10, 40, 50 ,WHITE);
		myOLED.display();
		
		myOLED.setTextSize(4);
		myOLED.setTextCursor(44, 14);
		myOLED.writeChar(0x30 + number);
		
		if(X > 70 && Y > 70) number++;
		if(X < -5 && Y < -3) number--;
		myOLED.display();
	}
	
	return *enter_pwd;
}

// OLED temperature & humidity
void sda_OLED(){
	float temp, humidity;
	
	if(dht22.sample()){
		myOLED.clearDisplay();
		temp = dht22.getTemperature() / 10.0;
		humidity = dht22.getHumidity() / 10.0;
			
		printf("Temp = %+4.1f C\t Humidity = %4.1f %% \n\n", temp, humidity);
		myOLED.display();
	}
}