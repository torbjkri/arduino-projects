#include <Servo.h>

Servo servo;

double runningAvg = 0.0f;

enum ServoState {
  ST,
  WT,
  FW,
};


int time;
int presTime;
int prevTime;
double sensorValue;
ServoState servoState = ST;
int timeSinceFw = 0;
int timeSinceInc= 0;


const int ledPin = 13;


void setup() {
  Serial.begin(9600);
  servo.attach(9);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  servo.write(90);
  runningAvg =  (double)analogRead(A0);
  
  while (millis() < 5000) {
  
    sensorValue = (double)analogRead(A0);
    runningAvg = runningAvg*0.5 + sensorValue * 0.5;
  }
  digitalWrite(ledPin, LOW);
  prevTime = millis();
}

void loop ()
{
  sensorValue = (double)analogRead(A0);
  presTime = millis();
  time = presTime -  prevTime;
  prevTime = presTime;
  double diff = runningAvg - sensorValue;
  runningAvg = runningAvg*0.99 + sensorValue*0.01;
  
  timeSinceInc = timeSinceInc + time;
  timeSinceFw = timeSinceFw + time;
  
  if (servoState == ST) {
    servo.write(90);
    if (diff > 2.0) {
      servoState = WT;
      timeSinceInc = 0;
    }
  }
  if (servoState == WT) {
    if (timeSinceInc > 350) {
      servoState = FW;
      timeSinceFw = 0;
    }
  }
  if (servoState == FW) {
    servo.write(0);
    if (timeSinceFw > 100) {
      servoState = ST;
    }
  }
  
  delay(20);
  
}
