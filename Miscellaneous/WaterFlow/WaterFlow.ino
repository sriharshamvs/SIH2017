#include <ESP8266WiFi.h>  
#include <WiFiClient.h> 


void pulseCounter(void);
void CONNECT_TO_WIFI(void);
void WEB_PAGE(void);
void DATA_TO_CLOUD(void);
void READ_SENSOR(void);

WiFiClient client; 
WiFiClient localclient; 

const char* ssid = "SRI_Device";  
const char* password = "Welcome12#";  
unsigned long myChannelNumber = 720210;  
String myWriteAPIKey = "8AC9AK250X9W8P7R";  
const char * myReadAPIKey  = "AMKDM81DYZBJ8CX8";
const char* SERVER = "api.thingspeak.com";

bool SendData = false;
bool ReadSensor = false;

const int FlowSensor = D2; // variable for D2 pin
float calibrationFactor = 4.5; 
volatile byte pulseCount = 0;
float flowRate = 0;
unsigned int flowMilliLitres = 0;
unsigned long totalMilliLitres = 0;
unsigned long oldTime = 0;
byte sensorInterrupt = 0; // 0 digital pin 2

unsigned long previousMillis_1 = 0;     
const long interval_1 = 2000;           

unsigned long previousMillis_2 = 0;        
const long interval_2 = 15000;           

WiFiServer server(80);

void setup() {
  Serial.begin(115200); 
  delay(10);
  server.begin();
  CONNECT_TO_WIFI(); // Connect to WiFi network
  pinMode(FlowSensor, INPUT);
  digitalWrite(FlowSensor, HIGH);
  attachInterrupt(digitalPinToInterrupt(FlowSensor), pulseCounter, CHANGE);
} // void setup ends here

void loop() {

  unsigned long currentMillis_1 = millis();
  if(currentMillis_1 - previousMillis_1 >= interval_1) {
    previousMillis_1 = currentMillis_1;
    PrintValues();
  }
  unsigned long currentMillis_2 = millis();
  if(currentMillis_2 - previousMillis_2 >= interval_2) {
    previousMillis_2 = currentMillis_2;
    SendData = true;
  } 
  
  READ_SENSOR();   
  if(SendData){
    SendData = 0;
    DATA_TO_CLOUD();
  }
 attachInterrupt(digitalPinToInterrupt(FlowSensor), pulseCounter, CHANGE);
} // void loop ends here

void PrintValues(void){
  Serial.print("  Current Liquid Flowing: "); // Output separator
  Serial.print(flowMilliLitres);
  Serial.print("mL/Sec");

  Serial.print("  Current Liquid Flowing: "); // Output separator
  Serial.print(totalMilliLitres);
  Serial.print("mL");
}

void pulseCounter(void) {
    pulseCount++;     // Increment the pulse counter
}

void READ_SENSOR(void){
  
  flowRate = 0;
  flowMilliLitres = 0;

  if ((millis() - oldTime) > 1000) {
    detachInterrupt(sensorInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    pulseCount = 0;
    oldTime = millis(); 
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    unsigned int frac = 0;
    
//    Serial.print("Flow rate: ");
//    Serial.print(int(flowRate)); 
//    Serial.print("."); // Print the decimal point
//    
    frac = (flowRate - int(flowRate)) * 10;
//    Serial.print(frac, DEC);
//    Serial.print("L/min");
//    
//    Serial.print("  Current Liquid Flowing: "); // Output separator
//    Serial.print(flowMilliLitres);
//    Serial.print("mL/Sec");
//    
//    Serial.print("  Output Liquid Quantity: "); // Output separator
//    Serial.print(totalMilliLitres);
//    Serial.println("mL");
    sensorInterrupt = 0;
 }
}

void DATA_TO_CLOUD(void) {
  WiFiClient client; 
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

/*void WEB_PAGE(void){
  localclient = server.available();
  if (!localclient) {
    return;
  }
  
  String request = localclient.readStringUntil('\n');
  Serial.println(request);
  localclient.flush();
   
  localclient.println("HTTP/1.1 200 OK"); 
  localclient.println("Content-Type: text/html");
  localclient.println("");
  localclient.println("<!DOCTYPE HTML>");
  localclient.println("<head>");
  localclient.println("<meta http-equiv=\"refresh\" content=\"5\" >");
  localclient.println("</head>"); 
  localclient.println("<style>");
  localclient.println("body {background-color: #8baee8;}"); 
  localclient.println("</style>");
  localclient.print("<h1><b>VOLUME OF WATER FLOWING </b></h1>");
  localclient.println("<br><br><br>");
  localclient.print("VOLUME :");
  localclient.print(totalMilliLitres);
  localclient.println("ml");
  localclient.println("FLOW RATE :");
  localclient.print(flowRate);
  localclient.println("L/min");
  localclient.println("</html>");
  delay(5000);    
}*/
  
