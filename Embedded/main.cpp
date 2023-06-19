#include "mbed.h"
#include "Doorlock.h"
#include "motordriver.h"
#include "Adafruit_GFX/Adafruit_SSD1306.h"

/*
		Team project by Embedded.
		
		The purpose of this project is to develop a door lock system and implement an app,
		and to verify it with the MCU.

		This code defines only arbitrary functions.
		Some content should be defined in the ".h" file, and additional content will be added later.
		Currently, files that need to be added include "motor", "temperature", "humidity" and "buzzer".
*/

Timer tm;

enum { Waiting, Door_open, Door_waiting, Door_Close, Pwd_Mode, Doorlock_OLED } work;

void setup(){
	work = Waiting;
	redLed = 1;
	yellowLed = 0;
	greenLed = 0;
	Close_Door();
	tm.start();
}

// Beep Setup Transliteration C
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
	ctrlTicker.attach(&Control_Motor, 1.0);
}

int main() {
	int enter_pwd[4];		// interim measures
	int pwd[4] = {1,2,3,4};
	setup();
	Beep_Setup();
	OLED_setup();
	
	while(1){
		switch(work){
			case Waiting:
				// Add a Status Code While Waiting
				// if(user enter pwd)
				// work = check_pwd;
				break;
			
			case Door_open:
				Open_Door();					// 1. turn left motor
				Open_Door_Beep();			// 2. beep sound on
				greenLed = 0;					// 3. turn out green LED
				tm.reset();						// 4. reset & check 30 sec of timer
				tm.start();
				work = Door_waiting;	// 5. change door is waiting case
				break;
			
			case Door_waiting:
				yellowLed = 1;				// waiting LED
				// if timer over 30 sec or user click enter button
				if(tm.read_ms() >= 30000 || ENTER_BTN_EDGE() == FALLING_EDGE){
					tm.reset();
					work = Door_Close;	// change Close case
				}
				break;
				
			case Door_Close:
				yellowLed = 0;				// 1. turn off waiting LED
				Close_Door();					// 2. turn right motor
				Close_Door_Beep();		// 3. beep sound on
				work = Waiting;				// 4. change Waiting case
				break;
			
			case Pwd_Mode:
				if(Checking_PWD(*pwd, *enter_pwd) == 1) work = Door_open;
				else {
					Incorrect_Number_Beep();
					work = Waiting;
				}
				break;
			
			case Doorlock_OLED:
				// user enter number
				for(int i = 0; i < 4 ;){
				Doorlock_Display();
				// user control number
				/*
					if(joystick >= right){
						
					} else if(joystick >= left){}
						else if(joysitck >= up){}
						else if(joystick >= down){}
				*/
					if(ENTER_BTN_EDGE() == FALLING_EDGE){
						// *enter_pwd[i] = number;
					}
				}
				work = Pwd_Mode;
				break;
		}
	}
}
