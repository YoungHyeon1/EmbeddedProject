#include "mbed.h"
#include "motordriver.h"

#define MOTOR_STOP						0
#define MOTOR_CW_ROTATION 		1
#define MOTOR_CCW_ROTATION		-1

#define FORWARD_DIR						0
#define BACKWARD_DIR					1

Motor::Motor(PinName pwm, PinName dir):
_pwm(pwm), _dir(dir)
{
	// Set initial condition of PWM
	_pwm.period(0.001);		// f = 1000Hz
	_pwm = 0;
	
	// Initial condition of ouput enables
	_dir = FORWARD_DIR;
	_sign = MOTOR_STOP;
}

void Motor::forward(double speed){
	if (_sign == MOTOR_CCW_ROTATION){
		_pwm = 0;
		wait(0.2);
	}
	_dir = FORWARD_DIR;
	_pwm = (speed < 0) ? 0 : ((speed > 1) ? 1.0 : speed);
	_sign = MOTOR_CW_ROTATION;
}

void Motor::backward(double speed){
	float temp = 0;
	if (_sign == MOTOR_CW_ROTATION){	
		_pwm = 0;
		wait(0.2);
	}
	_dir = BACKWARD_DIR;
	_pwm = (speed < 0) ? 0 : ((speed > 1) ? 1.0 : speed);
	_sign = MOTOR_CCW_ROTATION;
}

void Motor::stop(void){
	_pwm = 0;
	_sign = MOTOR_STOP;
}