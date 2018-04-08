/*
 * Pressure detector to move a motor. Built for EnableTech Makeathon 2018. Team
 * Members: Aditya Palacharla, Akshay Battu, Ashna Mangla, Kamala Pillai, Radhika
 * Mardikar. Intended to help with Francisco's soccer leg. When you detect 3 peaks
 * (3 taps) in 1 second, unlock (move to 120 degrees). When you cross the threshold
 * for 3 continuous seconds (holding), lock (move to 180 degrees).
*/
#include <Servo.h>

// servo motor
Servo myservo;
// locked state - simulates straight knee
int locked = 180;
// unlocked state - simulates bent knee
int unlocked = 120;

// count for number of taps within 1 second
int numTaps = 0;
// timer for 1 second
int tapTimer = 0;
// detects the first time you cross the threshold
boolean isTap;
// true for 1 second after you have tapped for the first time, false otherwise
boolean countingTaps;

// timer for 1.5 seconds
int howLongHeld;
// calibration voltage, subtracted from all voltage measurements
int baseVoltage;

// pressure-sensitive resistor is in pin A0
const int pressureSensor = A0;
// arbitrary voltage threshold based on observation
const int threshold = 315;
// times for calibration, unlocking, and locking
const int calibrationTime = 300; // 300 * 10 ms = 3 sec;
const int timeToUnlock = 100; // 100 * 10 ms = 1 sec
const int timeToLock = 150; // 150 * 10 ms = 1.5 sec
// number of taps required within timeToUnlock to unlock motor
const int tapsToUnlock = 3;

/*
 * Attaches servo motor and sets in locked state. Calibrates for 3 seconds
 * to find the base voltage.
 */
void setup() {
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
  // servo motor is in pin 9
  myservo.attach(9);
  // begin in locked state
  myservo.write(locked);
  // calibrate for 3 seconds to get base voltage
  calibrate();
}

/*
 * Reads the voltage from the pressure-sensitive resistor. Calls
 * pressureFunction, and delays for 10 milliseconds.
 */
void loop() {
  // read the input on pin A0
  float pressureVoltage;
  pressureVoltage = analogRead(pressureSensor) - baseVoltage;
  // does all the processing with the voltage reading
  pressureFunction(pressureVoltage);
  // displays the pressureVoltage, useful for testing/debugging
  Serial.println(pressureVoltage);
  // 10 ms delay in between reads for stability
  delay(10);
}

/*
 * Calibrates the pressure-sensitive resistor. Waits for 3 seconds, 
 * then reads the voltage and sets the baseVoltage to that value.
 */
void calibrate() {
  // waits 3 seconds
  for (int i = 0; i < calibrationTime; i+= 1) {
    delay(10);
  }
  baseVoltage = analogRead(pressureSensor);
}

/*
 * Processes the pressureVoltage. Checks if the voltage is below or 
 * above the threshold. If below the threshold, then we reset our 
 * holding timer and tapping timer. If above the threshold, then we
 * start our holding timer and check if it is our first tap. Tap timer
 * and checking for numTaps happens every function call.
 */
void pressureFunction(float voltage){
    if (voltage < threshold) {
      // reset holding timer
      howLongHeld = 0;
      // no longer considered a tap
      isTap = false;
      // reset tapTimer if we aren't counting the taps
      if (!countingTaps){
        tapTimer = 0;
      }
    }
  
    if (voltage > threshold) {
      // Holding
      howLongHeld = howLongHeld + 1;
      // if held for 1.5 seconds, lock the motor
      if (howLongHeld >= timeToLock) {
        myservo.write(locked);
      }
      // increment numTaps if this is the first time we crossed threshold
      if (!isTap) {
        numTaps += 1;
        isTap = true;
      }
      // we should be counting taps if we are above threshold
      if (!countingTaps){
        countingTaps = true;
      }
      // if we have exceeded the number required to unlock, then unlock motors
      if (numTaps >= tapsToUnlock){
        myservo.write(unlocked);
       }
    }
    // increment our tapTimer if we are supposed to be counting taps
    if (countingTaps){
          tapTimer += 1;
    }
    // if the tapTimer exceeds 1 second, then we should reset all our tapping variables
    if (tapTimer > timeToUnlock){
         numTaps = 0;
         tapTimer = 0;
         countingTaps = false;
     }
}

