#include "mbed.h"
#include "Doorlock.h"
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

Timer autotm;

enum {Enter_pwd , Door_open, Door_waiting, Door_Close} work;

// Setup the equipment of MCU
void setup(){
	redLed = 1;
	yellowLed = 0;
	greenLed = 0;
	Close_Door();
	autotm.start();
}
void Beep_Setup(){
	buzzer.period_us(BEEP_HALF_PERIOD);
	buzzer = 0;
	buzzertm.start();
}

void OLED_setup(){
	int textSize = 1;
	
	i2cMaster.frequency(400000);
	
	wait(2.0);
	myOLED.clearDisplay();
	
	myOLED.setTextSize(textSize);
	myOLED.printf("FLAG DOOR LOCK Display \r\n");
	myOLED.display();
}

void joystick_setup(){
	jsTicker.attach(&readJoystick, 0.2);
}
// End Setup functions

int main() {
	int enter_pwd[4];		// interim measures
	int pwd[4] = {1,2,3,4};
	bool interrupt = false;
	setup();
	Beep_Setup();
	OLED_setup();
	joystick_setup();
	while(1){
		/*
		Temperature & humidity code here
		sda_OLED();
		if(ENTER_BTN_EDGE() == FALLING_EDGE) {
			work = Enter_pwd;
			interrupt = true;
		}
		*/
		
		while(interrupt){
			switch(work){
				case Enter_pwd:
					// If the User enters the password
					*enter_pwd = Password_OLED(enter_pwd);
				
					if(Checking_PWD(*pwd, *enter_pwd) == 1) work = Door_open;
					else {
						Incorrect_Number_Beep();
						work = Enter_pwd;
					}
					break;
				
				case Door_open:
					// If the password is correct
					Open_Door();					// 1. turn left motor
					Open_Door_Beep();			// 2. beep sound on
					greenLed = 0;					// 3. turn out green LED
					autotm.reset();						// 4. reset & check 30 sec of timer
					autotm.start();
					work = Door_waiting;	// 5. change door is waiting case
					break;
				
				case Door_waiting:
					// After the door opens and closes
					yellowLed = 1;				// waiting LED
					// if timer over 30 sec or user click enter button
					if(autotm.read_ms() >= 30000 || OPEN_BTN_EDGE() == FALLING_EDGE){
						autotm.reset();
						work = Door_Close;	// change Close case
					}
					break;
					
				case Door_Close:
					// a door closes
					yellowLed = 0;				// 1. turn off waiting LED
					Close_Door();					// 2. turn right motor
					Close_Door_Beep();		// 3. beep sound on
					work = Enter_pwd;				// 4. change Waiting case
					interrupt = false;
					break;
			}
		}
	}
}
