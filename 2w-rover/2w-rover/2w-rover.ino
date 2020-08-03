
// Servo
#include <Servo.h>
Servo servo;


// Ultrasonic pins
const int trigPin = 13; // 10 ms high pulse causes chirp, wait 50 us
const int echoPin = 12; // Width of pulse indicates distance

// Servo motor that aims ultrasonic sensor
const int servoPin = 11; // PWM output for hobby servo

// Motor control pins: L98N H bridge
const int enAPin = 6; // Left motor PWM spped control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 5; // Left motor Direction 2
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2
const int enBPin = 3; // Right motor PWM speed control

enum Motor{LEFT, RIGHT};

// Set motor speed: 255 full ahead, -255 full reverse, 0 stop
void go(enum Motor m, int speed)
{
  digitalWrite(m == LEFT ? in1Pin: in3Pin, speed > 0 ? HIGH: LOW);
  digitalWrite(m == LEFT ? in2Pin: in4Pin, speed <= 0 ? HIGH: LOW);
  analogWrite(m == LEFT ? enAPin: enBPin, speed < 0 ? -speed: speed);
}

// Initial motor test:
// left motor forward then back
// right motor forward then back

void testMotors ()
{
  static int speed[8] = {128, 255, 128, 0, -128, -255, -128};
  go(RIGHT, 0);

  for(unsigned char i = 0; i < 8; i++) {
    go(LEFT, speed[i]);
    delay(200);  
  }

  for(unsigned char i = 0; i < 8; i++) {
    go(RIGHT, speed[i]);
    delay(200);  
  }
}

// Read distance from the ultrasonic sensor, return distance in mm
//
// Speed of sound in dry air, 20C is 343 m/s
// pulseIn returns time in microseconds (10^-6)
// 2d = p * 10^-6 s * 343 m/s = p * 0.00343 m = p * 0.343 mm /us
unsigned int readDistance()
{
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned long period = pulseIn(echoPin, HIGH);
  return period * 343 / 2000;  
}

#define NUM_ANGLES 7
const unsigned char sensorAngles[NUM_ANGLES] = {60, 70, 80, 90, 100, 110, 120};
unsigned int distance[NUM_ANGLES];

// Scan the area ahead by sweeping the ultrasonic sensor left and right
// and recording the distance observed/ This takes a reading, then
// sends the servod to the next angles. Call repeatedyl once every 50ms or so.
void readNextDistance()
{
  static unsigned char angleIndex = 0;
  static signed char step = 1;

    distance[angleIndex] = readDistance();
    angleIndex += step;
    if (angleIndex == NUM_ANGLES - 1) {
      step = -1;
    }
    else if (angleIndex == 0) {
      step = 1;
    }
    servo.write(sensorAngles[angleIndex]);
}


//Initial config
//
// Config input and output pins
// center servo
// Turn off motors
// Test motors
// Scand the surroindings once
//
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);

  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);

  servo.attach(servoPin);
  servo.write(90);

  go(LEFT, 0);
  go(RIGHT, 0);

  testMotors();

  // Scan before starting
  servo.write(sensorAngles[0]);
  delay(200);
  for(unsigned char i = 0; i < NUM_ANGLES; i++) {
    readNextDistance();
    delay(200);  
  }
}

// Main loop
//
// get next sensor reading
// if anything is too close, back up
// else go forward
//
void loop() {
  readNextDistance();

  // Check if stuff too close
  unsigned char tooClose = 0;
  for (unsigned char i = 0; i < NUM_ANGLES; i++) {
    if (distance[i] < 300) {
      tooClose = 1;
    }
  }

  if (tooClose) {
    // Something close back up left
    go(LEFT, -180);
    go(RIGHT, -180);  
  }
  else {
    // Safe ahead!
    go(LEFT, 255);
    go(RIGHT, 255);  
  }

  // Check again in 50 ms
  delay(50);

}
