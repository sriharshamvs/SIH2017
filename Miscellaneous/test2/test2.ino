#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "test";
const char *password = "password";

String apiKey = "Z5OYIF4ST8MSVOWT";
const char* logServer = "api.thingspeak.com";

ESP8266WebServer server(80);

void handle_index() {
  server.send(200, "text/plain", "My Server!");
}

// Handling the /feed page from my server
void handle_feed() {
  String t = server.arg("temp");
  server.send(200, "text/plain", "This is response to client");
  setupStMode(t);
  Serial.print("\tTemperature = ");
  Serial.print(t);
  Serial.println(" *C");
}

void setupAccessPoint(){
  Serial.println("** SETUP ACCESS POINT **");
  Serial.println("- disconnect from any other modes");
  WiFi.disconnect();
  Serial.println("- start ap with SID: "+ String(ssid));
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("- AP IP address is :");
  Serial.println(myIP);
  setupServer();
}

void setupServer(){
  Serial.println("** SETUP SERVER **");
  Serial.println("- starting server :");
  server.on("/", handle_index);
  server.on("/feed", handle_feed);
  server.begin();
}

void setupStMode(String t){
  Serial.println("** SETUP STATION MODE **");
  Serial.println("- disconnect from any other modes");
  WiFi.disconnect();
  Serial.println();
  Serial.println("- connecting to Home Router SID: **********");
  WiFi.begin("Startrek", "Face fade12#");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("- succesfully connected");
  Serial.println("- starting client");
  
  WiFiClient client;

  Serial.println("- connecting to Database server: " + String(logServer));
  if (client.connect(logServer, 80)) {
    Serial.println("- succesfully connected");
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(t);
    postStr += "\r\n\r\n";
    Serial.println("- sending data...");
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop();
  Serial.println("- stopping the client");
  /** If your ESP does not respond you can just
  *** reset after each request sending 
  Serial.println("- trying reset");
  ESP.reset();
  **/
}

void setup() {
  delay(2000);
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  setupAccessPoint();
}

void loop() {
  delay(20000);
  setupAccessPoint();
  server.handleClient();
}
