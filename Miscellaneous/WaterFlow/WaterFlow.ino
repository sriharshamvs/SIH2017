#include <ESP8266WiFi.h>  
#include <WiFiClient.h> 


void pulseCounter(void);
void CONNECT_TO_WIFI(void);
void DATA_TO_CLOUD(void);
void READ_SENSOR(void);

const char* ssid = "SRI_Device";  
const char* password = "Welcome12#";  
unsigned long myChannelNumber = 720210;  
String myWriteAPIKey = "8AC9AK250X9W8P7R";  
const char * myReadAPIKey  = "AMKDM81DYZBJ8CX8";
const char* SERVER = "api.thingspeak.com";
bool SendData = false;
bool ReadSensor = false;

WiFiClient client; 

const int FlowSensor = D2; // variable for D2 pin
float calibrationFactor = 4.5; 
volatile byte pulseCount = 0;
float flowRate = 0;
unsigned int flowMilliLitres = 0;
unsigned long totalMilliLitres = 0;
unsigned long oldTime = 0;

void setup() {
  Serial.begin(115200); 
  delay(10);
  CONNECT_TO_WIFI(); // Connect to WiFi network   
  pinMode(FlowSensor, INPUT);
  digitalWrite(FlowSensor, HIGH);
  attachInterrupt(digitalPinToInterrupt(FlowSensor), pulseCounter, CHANGE);
  ReadSensor = true;
} // void setup ends here

void loop() {
  READ_SENSOR();
  DATA_TO_CLOUD();
  delay (1000);
  attachInterrupt(digitalPinToInterrupt(FlowSensor), pulseCounter, CHANGE);
} // void loop ends here

void pulseCounter(void) {
    pulseCount++;     // Increment the pulse counter
}

void READ_SENSOR(void){
  flowRate = 0;
  flowMilliLitres = 0;
  while(ReadSensor != false){
    ReadSensor = false;
    if ((millis() - oldTime) > 1000) {
      detachInterrupt(FlowSensor);
      flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
      oldTime = millis(); 
      flowMilliLitres = (flowRate / 60) * 1000;
      totalMilliLitres += flowMilliLitres;
      unsigned int frac = 0;
      
      Serial.print("Flow rate: ");
      Serial.print(int(flowRate)); 
      Serial.print("."); // Print the decimal point
      
      frac = (flowRate - int(flowRate)) * 10;
      Serial.print(frac, DEC);
      Serial.print("L/min");
      
      Serial.print("  Current Liquid Flowing: "); // Output separator
      Serial.print(flowMilliLitres);
      Serial.print("mL/Sec");
      
      Serial.print("  Output Liquid Quantity: "); // Output separator
      Serial.print(totalMilliLitres);
      Serial.println("mL");
    }
    SendData = true;
 }
}

void DATA_TO_CLOUD(void) {
  while(SendData != false){
    SendData = false;
    if (client.connect(SERVER,80)){   //   "184.106.153.149" or api.thingspeak.com
      String postStr = myWriteAPIKey;
      postStr +="&field1=";
      postStr += String(flowMilliLitres);
      postStr +="&field2=";
      postStr += String(totalMilliLitres);
      postStr += "\r\n\r\n";
    
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+myWriteAPIKey+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
    }
    client.stop();
    Serial.println("Waiting...");
    Serial.println();  
    delay(6000);  // Thingspeak needs delay of minimum 15 seconds
 }
 ReadSensor = true;
}

void CONNECT_TO_WIFI(void){
  Serial.begin(115200);
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println(WiFi.localIP());
  delay(10000);
}
