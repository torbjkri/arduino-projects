

int sensorValue;
int sensorLow = 1023;
int sensorHigh = 0;

const int ledPin = 13;


void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  while (millis() < 5000) {
  
    sensorValue = analogRead(A0);
    
    if (sensorValue > sensorHigh) {
      sensorHigh =  sensorValue;
    }
    if (sensorValue < sensorLow) {
      sensorLow = sensorValue;
    }
  
  
  }
  digitalWrite(ledPin, LOW);
}

void loop ()
{
  sensorValue = digitalRead(A0);
}
