/*
* OneKnobStepper.ino
*
* Created: 7/31/2018 11:41:22 PM
* Author: kanta
* target: Arduino ProMini with ATmega328P/3.3V/8MHz
* L6470 Library: https://github.com/ameyer/Arduino-L6470 
*/
#include <Arduino.h>
#include <SPI.h>
#include <L6470.h>
L6470 stepper(10);
#define RESET	A0
#define BUSYN	7

#define speedPodPin	A6
#define kvalTrim		A2	// option
#define maxSpeedTrim	A1	// option
#define FLAGpin	8	// not in use
#define STCKpin	9	// not in use

uint16_t maxSpeed	= 600; // depends on the motor.

void setup()
{
	Serial.begin(9600);
	pinMode(13, OUTPUT);
	stepper.init();
	stepper.setMicroSteps(128);
	stepper.setThresholdSpeed(1023);
}

void loop()
{
	int16_t speed;
	int p = analogRead(speedPodPin);
	if ( p <= 509 )
	{
		speed = pow((float)(509-p)/509,2.7) * maxSpeed;
		stepper.run(1, speed);
	} 
	else if ( p <= 513 )
	{
		stepper.softStop();
	} 
	else {
		speed = pow((float)(p-514)/509,2.7) * maxSpeed;
		stepper.run(0, speed);
	}
	
	delay(20);
}
