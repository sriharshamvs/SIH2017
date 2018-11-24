//#include <ThingSpeak.h>
#include <ESP8266WiFi.h>


/*#define relay D4
#define relay_s1 D5
#define relay_s2 D6
*/
char ssid[] = "ssid";          //  your network SSID (name) 
char pass[] = "passwd";   // your network password

int status = WL_IDLE_STATUS;
WiFiClient  client;
/*
unsigned int motor_status; // variable to save channel field reading
unsigned int sect_1,sect_2;
const unsigned int PHpin = A0;
float PHvalue=0.0;
    
unsigned long myChannelNumber = 485101; // modify this with your own Channel Number
const char * myReadAPIKey = "FLGDCHXQ1RW7W4MY";
const char * myWriteAPIKey = "C62BNANND9JG11QX";
*/

void setup() {
  Serial.begin(115200); // for debugging reasons
/*  pinMode(relay, OUTPUT);
  pinMode(relay_s1, OUTPUT);
  pinMode(relay_s2, OUTPUT);    
  digitalWrite(relay, HIGH);
  digitalWrite(relay_s1, HIGH);
  digitalWrite(relay_s2, HIGH);*/
 WiFiConnect();
 // ThingSpeak.begin(client);
  delay(2000);
}
void loop(){
  
}
/*
void loop() {
  // Read the latest value from field 1 of your channel
  motor_status = ThingSpeak.readIntField(myChannelNumber, 1, myReadAPIKey);
  sect_1=ThingSpeak.readIntField(myChannelNumber, 2, myReadAPIKey);
  sect_2=ThingSpeak.readIntField(myChannelNumber, 3, myReadAPIKey);
  ReadPH();
  ThingSpeak.writeField(myChannelNumber, 4, PHvalue, myWriteAPIKey);
   
  Serial.print("motor_status  = "); // debugging instrument
  Serial.println(motor_status);    // debugging instrument

  Serial.print("Sector 1_status  = "); // debugging instrument
  Serial.println(sect_1);    // debugging instrument

  Serial.print("Sector 2_status  = "); // debugging instrument
  Serial.println(sect_2);
  
  if ( motor_status == 1){
    digitalWrite(relay, LOW); // please notice if you need to modify this to LOW 
  }else{
    digitalWrite(relay, HIGH); // please notice if you need to modify this to HIGH 
  }

  if( sect_1 == 1){
    digitalWrite(relay_s1,LOW);
  }else{
      digitalWrite(relay_s1,HIGH);
  }

  if( sect_2 == 1){
    digitalWrite(relay_s2,LOW);
  }else{
     digitalWrite(relay_s2,HIGH);
  }
  
}// void loop ends here.
*/
void WiFiConnect(){
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println(WiFi.localIP());
  Serial.println("WiFi connected"); 
  delay(5000);
}
/*
void ReadPH(){
  PHvalue = analogRead(PHpin); 
  
  if(PHvalue>= 70 && PHvalue <=108){ // for neutral 
    PHvalue = 7; 
  }
  if(PHvalue>= 170){ // for air
     PHvalue = 5.7;                 
  }
  if(PHvalue>= 0 && PHvalue <=20){  // base
    if(PHvalue==20)PHvalue = 14;
  }
  if(PHvalue>= 28 && PHvalue <=38){ // acidic
    PHvalue = 0;
  }
}*/
