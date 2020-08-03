#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include "/home/wubster/git/arduino-projects/esp8266/wifi_config.h"

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);

const int led = 2;

void handleRoot();
void handleLED();
void handleNotFound();

void setup()
{
    Serial.begin(115200);
    delay(10);
    Serial.println('\n');

    pinMode(led, OUTPUT);

    wifiMulti.addAP(ssid, pw);

    Serial.println("Connecting.....");
    int i = 0;
    while(wifiMulti.run() != WL_CONNECTED) {
        delay(250);
        Serial.print('.');
    }
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp8266")) {
        Serial.println("mDNS responder started");
    } else {
        Serial.println("Error setting up mDNS responder!");
    }

    server.on("/", HTTP_GET,handleRoot);
    server.on("/LED", HTTP_POST,handleLED);
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");

}

void loop()
{
    server.handleClient();
}

void handleRoot()
{
    server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}

void handleLED()
{
    digitalWrite(led, !digitalRead(led));
    server.sendHeader("Location","/");
    server.send(303);
}

void handleNotFound()
{
    server.send(404, "text/plain", "404: Not found");
}