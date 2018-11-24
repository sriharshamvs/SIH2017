#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define   MOISTURE     A0

void CONNECT_TO_WIFI(void);
float READ_MOISTURE_VALUE(void);
void PRINT_MOISTURE_VALUE(float);

float MoistureValue = 0;

const char* ssid = "test";
const char* password = "password";

// Local ESP web-server address
String serverHost = "http://192.168.4.1/feed";
String data;
// DEEP_SLEEP Timeout interval
int sleepInterval = 5;
// DEEP_SLEEP Timeout interval when connecting to AP fails
int failConnectRetryInterval = 2;
int counter = 0;

IPAddress ip(192, 168, 4, 6);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiClient client;

void setup() {
  delay(2000);
  ESP.eraseConfig();
  WiFi.persistent(false);
  Serial.begin(115200);
  Serial.println();
  Serial.println("**************************");
  Serial.println("**************************");
  Serial.println("******** BEGIN ***********");
  delay(500);
  Serial.println("- set ESP STA mode");
  WiFi.mode(WIFI_STA);
  Serial.println("- connecting to wifi");
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    if(counter > 20){
       Serial.println("- can't connect, going to sleep");    
       hibernate(failConnectRetryInterval);
    }
    delay(500);
    Serial.print(".");
    counter++;
  }
  
  Serial.println("- wifi connected");
  Serial.println("- build DATA stream string");
  buildDataStream();
  Serial.println("- send GET request");
  sendHttpRequest();
  Serial.println();
  //hibernate(sleepInterval);
  delay(15000);
}

void sendHttpRequest() {
  HTTPClient http;
  http.begin(serverHost);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.POST(data);
  http.writeToStream(&Serial);
  http.end();
}

float READ_MOISTURE_VALUE(void){
   /* Reading the Moisture Value */
   MoistureValue = analogRead(MOISTURE); // Reading the Moisture sensor value
  // MoistureValue = map( MoistureValue,550,0,0,100);
   delay(5000);
   return MoistureValue;
}

void PRINT_MOISTURE_VALUE(float Moisture_Value){
  /*  Displaying the Mositure sensor Values on the Serial Moniotr */
  Serial.println();
  Serial.print("Moisture Value = ");  // Printing Mositure sensor Values.
  Serial.println(Moisture_Value); 
}


void buildDataStream() {
  float MOISTURE_VALUE=0;
  MOISTURE_VALUE = READ_MOISTURE_VALUE();  
  data = "Moisture2=";
  data += String(MOISTURE_VALUE);
  data += "\n";                            //Add a new line
  Serial.println("- data stream: "+data);
}

void hibernate(int pInterval) {
  WiFi.disconnect();
  ESP.deepSleep(10 * 600000 * pInterval, WAKE_RFCAL);
  delay(100);
}

void loop() {
  
  Serial.println("- build DATA stream string");
  buildDataStream();
  Serial.println("- send GET request");
  sendHttpRequest();
  Serial.println();
  delay(15000);
}
