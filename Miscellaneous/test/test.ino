#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

#define TEMP A0

float vref = 3.3;
float resolution = vref/1024;

void CONNECT_TO_WIFI(void);

//Replace your wifi credentials here
const char* ssid     = "test";
const char* password = "password";

//change your channel number here
unsigned long myChannelNumber = 383464;
const char * myWriteAPIKey = "Z5OYIF4ST8MSVOWT";

WiFiClient  client;

void setup() {
  delay(100);
  Serial.begin(115200);   // Set Baudrate to 115200
  CONNECT_TO_WIFI();
  // We start by connecting to a WiFi network
}

void loop() {
  
 float t = analogRead(TEMP);
 t = (t*resolution);
 t = t*100;
 Serial.print(t);
 Serial.println(" *C");
 ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
 delay(20000);
}


void CONNECT_TO_WIFI(void){
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  ThingSpeak.begin(client);
  delay(10000);
}

