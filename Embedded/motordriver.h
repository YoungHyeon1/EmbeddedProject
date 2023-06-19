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
	
	protected:
		PwmOut _pwm;
		DigitalOut _dir;
		int _sign;
};

#endif // MOTORDRIVER_H