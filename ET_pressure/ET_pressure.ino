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
int unlocked = 45;

int numTaps = 0;
int tapTimer = 0;

int howLongHeld;
boolean isPeak;
boolean countingTaps;

int startTime = 150;
const int sensor = A0;
const int threshold = 300;
const int timeToUnlock = 150; // 150 * 10 ms = 1.5 sec
const int timeToLock = 300; // 300 * 10 ms = 3 sec

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  myservo.attach(9);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  float voltage;
  voltage = analogRead(sensor);
  
  if (voltage < threshold) {
    //howLongHeld = 0;
    myservo.write(locked);
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
    if (countingTaps){
      tapTimer += 1;
    } 

    if (tapTimer > timeToUnlock){
     if (numTaps >= 3){
      myservo.write(unlocked);
     } else {
      numTaps = 0;
      countingTaps = false;
    }
  }
 }

  
  Serial.println(numTaps);
  delay(10);        // delay in between reads for stability
   // Run stats script every 5 secs.
}

int timer(int timeRemaining) {
  if (timeRemaining != 0) {
    timeRemaining = timeRemaining - 10;
  }
  return timeRemaining;
}

