#include <SPI.h>
#include <ESP8266WiFi.h>  
#include <WiFiClient.h>

bool CloudFlag=LOW;
unsigned char SPIbuff [20];
unsigned long previousMillis = 0;
const int interval = 10000;
unsigned char tempValue=0,humidValue=0,ldrValue=0,windSpeed=0;
String absolutePressure="";

const char* ssid = "ssid";  
const char* password = "passwd";  
unsigned long myChannelNumber = 484331;  
String myWriteAPIKey = "B0A7CY83BOG657SG";  
const char * myReadAPIKey  = "D0X86BTH40LT3VQI";
const char* SERVER = "api.thingspeak.com";

WiFiClient client;

void setup (void){
  Serial.begin (115200);
  CONNECT_TO_WIFI();
  Serial.println ("Starting");
  digitalWrite(SS, HIGH);
  SPI.begin ();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}  // end of setup

void loop (void){
  
   unsigned char SPIbuff[10];
  // enable Slave Select
  digitalWrite(SS, LOW);    
  SPI.transfer (1);   // initiate transmission
  for (int pos = 0; pos < sizeof (SPIbuff) - 1; pos++){
    delayMicroseconds (15);
    SPIbuff[pos] = SPI.transfer(0);
    if (SPIbuff [pos] == 0){
      break;
    }
  }
  SPIbuff [sizeof (SPIbuff) - 1] = '\0';  // ensure terminating null
  // disable Slave Select
  digitalWrite(SS, HIGH);
  
  Serial.print ("We received: ");
  /*unsigned char i=0;
  for(i=0;i<3;i++){
    Serial.print (SPIbuff[i]);
  }*/
  tempValue=SPIbuff[0];
  humidValue=SPIbuff[2]; 
  ldrValue=SPIbuff[4];
  absolutePressure += SPIbuff[6];
  absolutePressure += SPIbuff[7];
  windSpeed=SPIbuff[9];
  
  Serial.print(tempValue);
  Serial.print(" *C");
  Serial.print("\t");
  Serial.print(humidValue);
  Serial.print(" %\t");
  Serial.print(ldrValue);
  Serial.println(" Cd");
  Serial.print(absolutePressure);
  Serial.println(" mb\t");

  Serial.print(windSpeed);
  Serial.println(" kmph\t");
    
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    CloudFlag=HIGH;
  }
  
  DATA_TO_CLOUD();
  absolutePressure="";
  Serial.println('\n');
  //delayMicroseconds(100);  // 1 second delay
}  // end of loop

/*
void RecieveSPIdata(){
 
 while(SPIflag){ 
  digitalWrite(SS, LOW);    
  SPI.transfer (1);   // initiate transmission
  for (int pos = 0; pos < sizeof (SPIbuff) - 1; pos++){
    delayMicroseconds (15);
    SPIbuff[pos] = SPI.transfer(0);
    if (SPIbuff [pos] == 0){
      break;
    }
  }
  SPIbuff [sizeof (SPIbuff) - 1] = '\0';  // ensure terminating null
  // disable Slave Select
  digitalWrite(SS, HIGH);
  
  Serial.print ("We received: ");
  unsigned char i=0;
  for(i=0;i<3;i++){
    Serial.print (SPIbuff[i]);
    Serial.print("\t");
  }
  SPIflag=LOW;
 
 }// while condition ends here
}// Function ends here
*/

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

void DATA_TO_CLOUD(void) {
   if(!CloudFlag){
    return;
   }
    if (client.connect(SERVER,80)){   //   "184.106.153.149" or api.thingspeak.com
    String postStr = myWriteAPIKey;
    postStr +="&field1=";
    postStr += String(tempValue);
    postStr +="&field2=";
    postStr += String(humidValue);
    postStr +="&field3=";
    postStr += String(absolutePressure);
    postStr +="&field5=";
    postStr += String(windSpeed);
    postStr +="&field6=";
    postStr += String(ldrValue);
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
  CloudFlag=LOW;
  //delay(10000);  // Thingspeak needs delay of minimum 15 seconds
}

