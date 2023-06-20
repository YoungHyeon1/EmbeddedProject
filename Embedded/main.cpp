#include "mbed.h"
#include "motordriver.h"
#include "Adafruit_GFX/Adafruit_SSD1306.h"
#include "DHT22.h"

/*
		Team project by Embedded.
		
		The purpose of this project is to develop a door lock system and implement an app,
		and to verify it with the MCU.

		This code defines only arbitrary functions.
		Some content should be defined in the ".h" file, and additional content will be added later.
		Currently, files that need to be added include "motor", "temperature", "humidity" and "buzzer".
*/

#define BUZZER_PIN			PC_9
#define MOTOR_PWM_PIN		PA_7
#define MOTOR_DIR_PIN		PC_8
#define GREEN_LED_PIN		PA_13
#define YELLOW_LED_PIN	PB_10
#define RED_LED_PIN			PA_4
#define FIRST_BTN_PIN		PA_14
#define THIRD_BTN_PIN		PC_4
#define SDA_PIN					PB_2
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
Serial pc(USBTX, USBRX, 9600);
AnalogIn xAxis(JS_X_AXIS);
AnalogIn yAxis(JS_Y_AXIS);
Motor motorA(MOTOR_A_PWM_PIN, MOTOR_A_DIR_PIN);
DHT22 dht22(SDA_PIN);
Timer autotm;
Timer motortm;
Timer buzzertm;
Ticker jsTicker;
Ticker ctrlTicker;

int X, Y;
int doorStatus = 0;

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


void OpendoorBuzzer(){
	// Open door buzzer sound
	buzzer = 0.5;
	for(int cnt = 1;cnt <= 3;){
		wait(0.5);
		switch(cnt){
			case 1:
				buzzer.period_us(BEEP_HALF_PERIOD - 396);
				cnt++;
				break;
			case 2:
				buzzer.period_us(BEEP_HALF_PERIOD - 640);
				cnt++;
				break;
			case 3:
				buzzer = 0;
				cnt++;
				break;
		}
	}
}

void ClosedoorBuzzer(){
	// Close door buzzer sound
	buzzer = 0.5;
	for(int cnt = 1;cnt <= 3;){
		wait(0.5);
		switch(cnt){
			case 1:
				buzzer.period_us(BEEP_HALF_PERIOD + 396);
				cnt++;
				break;
			case 2:
				buzzer.period_us(BEEP_HALF_PERIOD + 640);
				cnt++;
				break;
			case 3:
				cnt++;
				buzzer = 0;
				break;
		}
	}
}
void wrongBuzzer(){
	// wrong password buzzer sound
	for(int cnt = 1;cnt <= 3; cnt++){
		buzzer = 0.5;
		wait(0.3);
		buzzer = 0;
		wait(0.3);
	}
}
void OpenCloseDoor(){
	/*
		if door is open, door result is true
		else door is close, door result is false
	*/
			// doorStatus = 1;
			OpendoorBuzzer();
			greenLed = 1;
			redLed = 0;
			motortm.reset();
			motortm.start();
			door_work.forward(0.5);
			wait(0.5);
			door_work.stop();
			motortm.stop();
			motortm.reset();
			autotm.reset();
			myOLED.clearDisplay();
			myOLED.setTextCursor(38, 1);
			myOLED.setTextSize(1);
			myOLED.printf("DOOR OPEN! \n");
			myOLED.display();
			while(autotm.read() <= 30){
				if(OPEN_BTN_EDGE() == FALLING_EDGE) break;
			}
			ClosedoorBuzzer();
			redLed = 1;
			greenLed = 0;
			motortm.reset();
			motortm.start();
			door_work.backward(0.5);
			wait(0.5);
			doorStatus = 0;
			door_work.stop();
			motortm.stop();
			motortm.reset();
			autotm.reset();
}

void readJoystick(){
	// read joystick
	X = xAxis * 100;
	Y = xAxis * 100;
	
	X = NEUTRAL_VALUE - X;
	if(abs(X) <= 1) X = 0;
	Y = NEUTRAL_VALUE - Y;
	if(abs(Y) <= 1) Y = 0;
}

bool checkPWD(){
	int i = 0, textSize = 1;
	int savePWD[4] = {1, 2, 3, 4};
	int enterPWD[4] = {-1, -1, -1, -1};
	bool correctPWD = true;
	
		for(int j = 0; j < 4;){
		readJoystick();
		myOLED.clearDisplay();
		myOLED.drawRect(30, 10, 40, 50 ,WHITE);
			
		if(X >= 70 || Y >= 70) {
			i--;
			if(i < 0) i = 0;
		}
		if(X <= -5 || Y <= -4) {
			i++;
			if(i > 9) i = 9;
		}
		wait(0.2);
		myOLED.setTextSize(4);
		myOLED.setTextCursor(44, 14);
		myOLED.writeChar(0x30 + i);
		myOLED.display();
		
		if(ENTER_BTN_EDGE() == FALLING_EDGE){	
				enterPWD[j] = i;
				printf("password %d: %d \n", j+1, enterPWD[j]);
				j++;
		}
	}
		
		for(int j = 0; j < 4; j++){
			if(savePWD[j] != enterPWD[j]){
				correctPWD = false;
			}
		}
		if(correctPWD == true){
			return true;
		} else {
			return false;
		}
}
void SDA_OLED(){
	int textSize = 1;
	float temp, humidity;
	
	if(dht22.sample()){
		myOLED.clearDisplay();
		temp = dht22.getTemperature() / 10.0;
		humidity = dht22.getHumidity() / 10.0;
			
		printf("Temp = %+4.1f C\t Humidity = %4.1f %% \n\n", temp, humidity);
	}
	myOLED.setTextCursor(1, 1);
	myOLED.setTextSize(textSize);
	myOLED.printf("Temp = %+4.1f C\n Humidity = %4.1f %% \n\n", temp, humidity);
	myOLED.display();
	wait(0.8);
}
void OLEDsetup(){
	int textSize = 1;
	
	i2cMaster.frequency(400000);
	myOLED.clearDisplay();
	
	myOLED.setTextSize(textSize);
	myOLED.printf("FLAG DOOR LOCK \nDisplay \r\n");
	myOLED.display();
	wait(2.0);
	myOLED.clearDisplay();
	myOLED.display();
}

void setup(){
	OLEDsetup();
	redLed = 1;
	buzzer.period_us(BEEP_HALF_PERIOD);
	buzzertm.start();
	buzzer = 0;
	autotm.start();
	jsTicker.attach(&readJoystick, 0.2);
}

int main(){
	setup();
	while(1){
		SDA_OLED();
		if(OPEN_BTN_EDGE() == FALLING_EDGE) {
			doorStatus = 1;
			OpenCloseDoor();
		}
		if(ENTER_BTN_EDGE() == FALLING_EDGE) {
			if(checkPWD()){
				doorStatus = 1;
				OpenCloseDoor();
			} else {
				wrongBuzzer();
			}
		}
	}
}