#include <ESP8266WiFi.h>  
#include <WiFiClient.h>  

#define   MOISTURE        A0
//#define   DHT11_PIN       D0 
//#define   MOTOR_RELAY     D1  
//#define   MOTOR_ON_LED    D2  // Initilizing digtal pin D2 on NodeMCU to LED_1

int READ_MOISTURE_VALUE(void);
void PRINT_MOISTURE_VALUE(int);
void CONNECT_TO_WIFI(void);
void DATA_TO_CLOUD(void);

float MoistureValue = 0;
int MOISTURE_VALUE = 0;

const char* ssid = "ssid";  
const char* password = "passwd";  
unsigned long myChannelNumber = 484883;  
String myWriteAPIKey = "A7FICY1L7Q1Y9CWF";  
const char * myReadAPIKey  = "JCBCTWOTPPLTQCFZ";
const char* SERVER = "api.thingspeak.com";

WiFiClient client;  
//dht DHT;

void setup(){  
  Serial.begin(115200);    
  CONNECT_TO_WIFI();     // Connect to WiFi network   
  delay(5);
}  

void loop(){   
  //int check = DHT.read11(DHT11_PIN);  
  //CHECK_DHT_11(check);    // Passing DHT "check" value to CHECK_DHT_11 funciton for checking health of the sensor
  //DHT11_DISPLAY();   
  MOISTURE_VALUE = READ_MOISTURE_VALUE();    // Reading and Printing the the 
  PRINT_MOISTURE_VALUE(MOISTURE_VALUE);  // Printing the Moisture Value
  DATA_TO_CLOUD();
  delay(3000);
}
 
int READ_MOISTURE_VALUE(void){
   /* Reading the Moisture Value */
   MoistureValue = analogRead(MOISTURE); // Reading the Moisture sensor value
   MoistureValue = map( MoistureValue,1024,0,0,100);
   return MoistureValue;
}
void PRINT_MOISTURE_VALUE(int Moisture_Value){
  /*  Displaying the Mositure sensor Values on the Serial Moniotr */
  Serial.println();
  Serial.print("Moisture Value = ");  // Printing Mositure sensor Values.
  Serial.println(Moisture_Value); 
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
/*void CHECK_DHT_11(int CHECK){
   Checking the Health of DHT11 
  Serial.print("DHT11 Health : ");
  switch(CHECK)
  {
    case DHTLIB_OK:  
    Serial.println("DHT11 OK"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.println("Checksum error"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.println("Time out error"); 
    break;
    default: 
    Serial.println("Unknown error"); 
    break;
  }
}
void DHT11_DISPLAY(void){
 /*  Displaying the DHT sensor Values on the Serial Moniotr 
  Serial.print("Humidity = ");
  Serial.print(DHT.humidity, 1);  // Printing Humidity sensor Values.
  Serial.print(" %\t");
  Serial.print("Temperature = ");
  Serial.print(DHT.temperature, 1); // Printing Temperature sensor Values.
  Serial.print(" *C"); 
}*/
void DATA_TO_CLOUD(void) {
  /*float HUMIDITY = DHT.humidity;
  float TEMPERATURE = DHT.temperature;
      
  if (isnan(TEMPERATURE) || isnan(HUMIDITY)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }*/

  if (client.connect(SERVER,80)){   //   "184.106.153.149" or api.thingspeak.com
    String postStr = myWriteAPIKey;
    /*postStr +="&field3=";
    postStr += String(TEMPERATURE);
    postStr +="&field4=";
    postStr += String(HUMIDITY);*/
    postStr +="&field2=";
    postStr += String(MOISTURE_VALUE);
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

