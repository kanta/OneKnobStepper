/*
 Name:		oneKnobStepper.ino
 Created:	2020/06/12 10:55:23
 Author:	kanta

 Target: Arduino ProMini with ATmega328P/3.3V/8MHz
*/


#include <SPI.h>
#include <SparkFundSPINConstants.h>
#include <SparkFunAutoDriver.h> // https://github.com/sparkfun/SparkFun_AutoDriver_Arduino_Library

#define L6470RESETpin	A0
#define BUSYpin	7
#define CSpin   10
#define speedPodPin	A6
#define kvalTrimPin		A2	// option
#define maxSpeedTrimPin	A1	// option
#define FLAGpin	8	// not in use
#define STCKpin	9	// not in use

float maxSpeed = 820.; // depends on the motor and input voltage

AutoDriver stepper(0, CSpin, L6470RESETpin);

void setup() {
    pinMode(L6470RESETpin, OUTPUT);
    digitalWrite(L6470RESETpin, HIGH);
    digitalWrite(L6470RESETpin, LOW); // reset L6470
    SPI.begin();
    SPI.setDataMode(SPI_MODE3);
    
    stepper.SPIPortConnect(&SPI);
    stepper.configSyncPin(BUSY_PIN, 0);
    stepper.hardHiZ();
    stepper.configStepMode(STEP_FS_128);
    stepper.setMaxSpeed(maxSpeed);
    stepper.setFullSpeed(maxSpeed+10);
    stepper.setAcc(2000.);
    stepper.setDec(2000.);
    stepper.setSlewRate(SR_530V_us);
    stepper.setOCThreshold(OC_3000mA);
    stepper.setOCShutdown(OC_SD_ENABLE);
    stepper.setSwitchMode(SW_USER);
    stepper.setOscMode(INT_16MHZ);
    stepper.setRunKVAL(255);
    stepper.setAccKVAL(255);
    stepper.setDecKVAL(255);
    stepper.setHoldKVAL(32);
    delay(1);
    stepper.getStatus();

    Serial.begin(9600);
}

void loop() {
    float speed;
    int p = analogRead(speedPodPin);
    if (p <= 509)
    {
        speed = pow((float)(509 - p) / 509, 2.7) * maxSpeed;
        stepper.run(1, speed);
        //Serial.println(speed);
    }
    else if (p <= 513)
    {
        stepper.softStop();
    }
    else {
        speed = pow((float)(p - 514) / 509, 2.7) * maxSpeed;
        stepper.run(0, speed);
    }

    delay(20);
}
