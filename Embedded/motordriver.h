#ifndef MBED_MOTOR_H
#define MBED_MOTOR_H

#include "mbed.h"

#define FORWARD_DIR						0
#define BACKWARD_DIR					1

class Motor{
	public:
		Motor(PinName pwm, PinName dir);
		void forward(double speed);
		void backward(double speed);
		void stop(void);
		void setDir(int dir);
		void incSpeed(double step);
		void decSpeed(double step);
	
	protected:
		PwmOut _pwm;
		DigitalOut _dir;
		int _sign;
};

#endif // MOTORDRIVER_H