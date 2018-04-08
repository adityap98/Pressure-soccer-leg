/*
  Pressure detector
  when you detect 3 peaks in X period, unlock
  when you cross pressureThreshold for 3 continuous seconds, lock

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/
#include <Servo.h>


Servo myservo;
int locked = 180;
int unlocked = 90;

int numTaps = 0;
int tapTimer = 0;
int flexTimer = 0;

int howLongHeld;
boolean isPeak;
boolean countingTaps;
boolean flexing = false;
int basePressureVoltage;
int baseEmgVoltage;

const int pressureSensor = A0;
const int emgSensor = A1;
const int pressureThreshold = 400;
const int emgThreshold = 200;

const int calibrationTime = 300; // 300 * 10 ms = 3 sec;
const int timeToUnlock = 100; // 100 * 10 ms = 1 sec
const int timeToLock = 150; // 150 * 10 ms = 1.5 sec
const int timeToFlex = 200; // 200 * 10 ms = 2 sec
const int tapsToUnlock = 3;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  myservo.attach(9);
  myservo.write(locked);
  calibrate();
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  float pressureVoltage;
  float emgVoltage;
  pressureVoltage = analogRead(pressureSensor) - basePressureVoltage;
  emgVoltage = analogRead(emgSensor) - baseEmgVoltage;

  if (emgVoltage > emgThreshold){
    flexing = true;
    flexTimer = 0;
  }
  if (flexing){
    flexTimer += 1;
    if (flexTimer <= timeToFlex) {
          if (pressureVoltage < pressureThreshold) {
      howLongHeld = 0;
      isPeak = false;
      if (!countingTaps){
        tapTimer = 0;
      }
    }
  
    if (pressureVoltage > pressureThreshold) {
    
      // Holding
      howLongHeld = howLongHeld + 1;
      if (howLongHeld >= timeToLock) {
        myservo.write(locked);
      }
    
      // Number of taps
      if (!isPeak) {
        numTaps += 1;
        isPeak = true;
      }
      if (!countingTaps){
        countingTaps = true;
      }
      if (numTaps >= tapsToUnlock){
        myservo.write(unlocked);
       }
    }
    }
  }

    if (countingTaps){
          tapTimer += 1;
     }
    if (tapTimer > timeToUnlock){
         numTaps = 0;
         tapTimer = 0;
         countingTaps = false;
      }

  
  if (flexTimer > timeToFlex){
    flexing = false;
  }
  
  
  Serial.println(pressureVoltage);
  delay(10);        // delay in between reads for stability
   // Run stats script every 5 secs.
}

void calibrate() {
  for (int i = 0; i < calibrationTime; i+= 1) {
    delay(10);
  }
  basePressureVoltage = analogRead(pressureSensor);
  baseEmgVoltage = analogRead(emgSensor);
  
}

void pressureFunction(float voltage){
    if (voltage < pressureThreshold) {
      howLongHeld = 0;
      isPeak = false;
      if (!countingTaps){
        tapTimer = 0;
      }
    }
  
    if (voltage > pressureThreshold) {
    
      // Holding
      howLongHeld = howLongHeld + 1;
      if (howLongHeld >= timeToLock) {
        myservo.write(locked);
      }
    
      // Number of taps
      if (!isPeak) {
        numTaps += 1;
        isPeak = true;
      }
      if (!countingTaps){
        countingTaps = true;
      }
      if (numTaps >= tapsToUnlock){
        myservo.write(unlocked);
       }
    }

    if (countingTaps){
          tapTimer += 1;
     }
    if (tapTimer > timeToUnlock){
         numTaps = 0;
         tapTimer = 0;
         countingTaps = false;
      }
}

