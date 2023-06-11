#include "mbed.h"
#include "DOORLOCK.h"
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

void Open_Close_Door(){
	Open_Door();
	yellowLed = 1;
	// user click button, close Door
	if(OPEN_BTN_EDGE() == FALLING_EDGE){
		yellowLed = 0;
		tm.reset();
		Close_Door();
	} else if(tm.read() > 30.0){		// open time over 30 sec
		yellowLed = 0;
		tm.reset();
		Close_Door();
	}
}

int main() {
	while(1){
		if(OPEN_BTN_EDGE() == FALLING_EDGE){
			if(greenLed == 1) Close_Door();		// if Door work is open, close door
			else Open_Close_Door();
		}
	}
}
