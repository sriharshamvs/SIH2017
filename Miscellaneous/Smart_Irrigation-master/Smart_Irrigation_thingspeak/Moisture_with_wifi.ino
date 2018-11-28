/*
 * Created on : 06/12/2017  11:00 AM
 * Author     : MVS Sriharsha
 * Info       : Smart Irrigation using NodeMCU
 */

#include <ESP8266WiFi.h>
#include <dht.h>

#define   DHT11_PIN       D0  // Initilizing digital pin D0 on NodeMCU to DHT11
#define   MOISTURE        A0  // Initilizing analog pin A0 on NodeMCU to Moisture sensor
#define   MOTOR_RELAY     D1  // Initilizing digtal pin D1 on NodeMCU to Realy
#define   MOTOR_ON_LED    D2  // Initilizing digtal pin D2 on NodeMCU to LED_1
#define   MOTOR_OFF_LED   D3  // Initilizing digtal pin D3 on NodeMCU to LED_2

void CHECK_DHT11_HEALTH(int);
void DHT11_DISPLAY(void);
int READ_MOISTURE_VALUE(void);
void PRINT_MOISTURE_VALUE(int);
void DATA_TO_CLOUD(void);
void CONNECT_TO_WIFI(void);

float MoistureValue = 0;

const char* ssid = "your_ssid";
const char* password = "password";

String API_KEY = "your_api_key"; 
const char* SERVER = "api.thingspeak.com";

dht DHT;
WiFiClient client;

void setup(){
  
  Serial.begin(115200);   // Set Baudrate to 115200
  pinMode(MOTOR_RELAY,OUTPUT);  // Set MOTOR_RELAY pin to Output
  pinMode(MOTOR_ON_LED,OUTPUT);  // Set MOTOR_ON_LED pin to Output
  pinMode(MOTOR_OFF_LED,OUTPUT);  // Set MOTOR_OFF_LED pin to Output
  CONNECT_TO_WIFI();                   // Connecting to WiFi
  
  delay(3000);
  
}/* void SETUP ends here */

void loop(){
  
  int check = DHT.read11(DHT11_PIN);    // Reading DHT PIN
  int MOISTURE_VALUE = 0;
  
  CHECK_DHT11_HEALTH(check);    // Passing DHT "check" value to CHECK_DHT_11 funciton for checking health of the sensor
  DHT11_DISPLAY();    // Displaying DHT Values on Serial Moinitor    
  MOISTURE_VALUE = READ_MOISTURE_VALUE();    // Reading and Printing the the 
  PRINT_MOISTURE_VALUE(MOISTURE_VALUE);   // Printing the Moisture Values
  DATA_TO_CLOUD();
  
  if(MOISTURE_VALUE >= 75){
    delay(1000);
    digitalWrite(MOTOR_RELAY,HIGH); // Turning the "MOTOR_RELAY" ON
    delay(1000);
    digitalWrite(MOTOR_ON_LED,HIGH);  // Turning "Motor_ON_LED" ON
    delay(250);
    digitalWrite(MOTOR_OFF_LED,LOW);  // Turning "Motor_OFF_LED" OFF
    delay(250);
  }else{
    delay(1000);
    digitalWrite(MOTOR_RELAY,LOW);  // Turning the "MOTOR_RELAY" ON
    delay(1000);
    digitalWrite(MOTOR_ON_LED,LOW);   // Turning "Motor_OFF_LED" ON
    delay(250);
    digitalWrite(MOTOR_OFF_LED,HIGH);     // Turning "Motor_ON_LED" OFF
    delay(250);     
  }
  
  delay(2000);
}/* void LOOP ends here */

/*-------------------------------------------------------------------------------------------------------------------------------*/

void CHECK_DHT11_HEALTH(int CHECK){
  /* Checking the Health of DHT11 */
  
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
 /*  Displaying the DHT sensor Values on the Serial Moniotr */
 
  Serial.print("Humidity = ");
  Serial.print(DHT.humidity,1);  // Printing Humidity sensor Values.
  Serial.print(" %\t");
  Serial.print("Temperature = ");
  Serial.print(DHT.temperature,1); // Printing Temperature sensor Values.
  Serial.print(" *C"); 

}
/*-------------------------------------------------------------------------------------------------------------------------------*/
int READ_MOISTURE_VALUE(void){
   /* Reading the Moisture Value */
   
   MoistureValue= analogRead(MOISTURE); // Reading the Moisture sensor value
   MoistureValue = MoistureValue/10;  
   
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
  Serial.println("Server Ready");
}

void DATA_TO_CLOUD(void) {
 
  float HUMIDITY = DHT.humidity;
  float TEMPERATURE = DHT.temperature;
      
  if (isnan(TEMPERATURE) || isnan(HUMIDITY)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (client.connect(SERVER,80)){   //   "184.106.153.149" or api.thingspeak.com
    String postStr = API_KEY;
    postStr +="&field1=";
    postStr += String(TEMPERATURE);
    postStr +="&field2=";
    postStr += String(HUMIDITY);
    postStr +="&field3=";
    postStr += String(MoistureValue);
    postStr += "\r\n\r\n";
  
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+API_KEY+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop();
  Serial.println("Waiting...");
  Serial.println();  
  delay(10000);  // Thingspeak needs delay of minimum 15 seconds
 
}


