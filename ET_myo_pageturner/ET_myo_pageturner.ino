
const int sensor = A0;
#include <Servo.h>
Servo myservo;
//int servopin = 9;
int pos = 0;
float EMA_a = 0.3;    //initialization of EMA alpha
int EMA_S = 0;        //initialization of EMA S
int highpass = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  //pinMode(servopin, OUTPUT);
  Serial.begin(9600);
  myservo.attach(9);
  EMA_S = analogRead(sensor);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  float voltage;
  boolean isFlexed;
  voltage = analogRead(sensor);
  EMA_S = (EMA_a*voltage) + ((1-EMA_a)*EMA_S);  //run the EMA
  highpass = voltage - EMA_S;
  //voltage = 180 * voltage / 1000;
  //myservo.write(voltage);
  //delay(15);
  if (voltage > 400) {
    isFlexed = true;
  }
  if (voltage < 150) {
    isFlexed = false;
  }
  if (isFlexed) {
    myservo.write(180);              // tell servo to go to position in variable 'pos'
    //delay(15);                       // waits 15ms for the servo to reach the position
  }
  else {
    myservo.write(0);              // tell servo to go to position in variable 'pos'
  }
  delay(15);                       // waits 15ms for the servo to reach the position
  //print out the value you read:
  //Serial.println(voltage);
  //delay(10);        // delay in between reads for stability
   // Run stats script every 5 secs.
}
