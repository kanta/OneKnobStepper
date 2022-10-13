#include <Arduino.h>
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

float maxSpeed = 1200;//650;//820.; // depends on the motor and input voltage

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

    #define KVAL_ALL    30
    // stepper.setRunKVAL(238);
    // stepper.setAccKVAL(238);
    // stepper.setDecKVAL(238);
    // stepper.setHoldKVAL(16);
    // stepper.setParam(ST_SLP, 7895);
    // stepper.setParam(FN_SLP_ACC, 234);
    // stepper.setParam(FN_SLP_DEC, 234);

    // 17HS08-1004S_24V
    stepper.setRunKVAL(KVAL_ALL);
    stepper.setAccKVAL(KVAL_ALL);
    stepper.setDecKVAL(KVAL_ALL);
    stepper.setHoldKVAL(5);
    stepper.setParam(INT_SPD, 8783);
    stepper.setParam(ST_SLP, 11);
    stepper.setParam(FN_SLP_ACC, 31);
    stepper.setParam(FN_SLP_DEC, 31);

    delay(1);
    stepper.getStatus();

    Serial.begin(9600);
}

void loop() {
    float speed;
    int p = analogRead(speedPodPin);
    if (p <= 509)
    {
        speed = powf((float)(509.0f - p) / 509.0f, 2.7f) * (float)maxSpeed;
        stepper.run(1, speed);
        //Serial.println(speed);
    }
    else if (p <= 513)
    {
        // stepper.softStop();
        stepper.softHiZ();
    }
    else {
        speed = powf((float)(p - 514.0f) / 509.0f, 2.7f) * (float)maxSpeed;
        stepper.run(0, speed);
    }

    delay(20);
}