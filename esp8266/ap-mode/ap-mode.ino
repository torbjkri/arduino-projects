#include <ESP8266WiFi.h>
#include "/home/wubster/git/arduino-projects/esp8266/config.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  WiFi.softAP(ssid, pw);
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");
  Serial.print("IP adrres:\t");
  Serial.println(WiFi.softAPIP());

}

void loop() {
  // put your main code here, to run repeatedly:

}
