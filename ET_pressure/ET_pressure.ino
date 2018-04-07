/*
  Pressure detector
  when you detect 3 peaks in X period, unlock
  when you cross threshold for 3 continuous seconds, lock

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/
#include <Servo.h>
#include <stdio.h>
#include <stdlib.h>

Servo myservo;
int locked = 0;
int unlocked = 135;

int numTaps = 0;
int tapTimer = 0;

int howLongHeld;
boolean isPeak;
boolean countingTaps;
int baseVoltage;

const int sensor = A0;
const int threshold = 75;
//const int second_threshold = 400;
const int timeToUnlock = 100; // 100 * 10 ms = 1 sec
const int timeToLock = 150; // 150 * 10 ms = 1.5 sec
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
  float voltage;
  voltage = analogRead(sensor) - baseVoltage;
  
  if (voltage < threshold) {
    howLongHeld = 0;
    isPeak = false;
    if (!countingTaps){
      tapTimer = 0;
    }
  }
  
  if (voltage > threshold) {
    
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
  
  Serial.println(voltage);
  delay(10);        // delay in between reads for stability
   // Run stats script every 5 secs.
}

void calibrate() {
  for (int i = 0; i < 300; i+= 1) {
    delay(10);
  }
  baseVoltage = analogRead(sensor);
}

