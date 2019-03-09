#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define FAN     D2
#define LIGHT   D3 

IPAddress local_IP(192,168,4,100);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

/* This are the WiFi access point settings. Update them to your likin */
const char *ssid = "FLController";
const char *password = "Welcome12#";

WiFiServer server(80);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(FAN,OUTPUT);              // FAN
  pinMode(LIGHT,OUTPUT);            // LIGHT
  digitalWrite(FAN,LOW);
  digitalWrite(LIGHT,LOW);
  WIFI_CONNECT();                   // Connecting to WiFi
  server.begin();                   // Begin server 
}

void loop() {
  // put your main code here, to run repeatedly:
  built_in_led_blink();
  // Check to see if the client request was "GET /H" or "GET /L":
 WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
            if (c == '\n') {                // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();  
            client.println("<html>"); // Start of HTML
            client.println("<head>");
            client.println("<meta http-equiv='refresh' content='1' />");  // Refresh webpage every 3 seconds
            client.println("</head>");

            client.println("<body style = 'background-color: black'>");
            client.println("</body>"); // Start of HTML      
            client.println("</html>"); // Start of HTML
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /FH")) {
          digitalWrite(FAN, LOW);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /FL")) {
          digitalWrite(FAN, HIGH);                // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /LH")) {
          digitalWrite(LIGHT, LOW);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /LO")) {
          digitalWrite(LIGHT, HIGH);                // GET /L turns the LED off
        }
      }// end of if(client.available())
    }// end of while(client.disconnect())
    // close the connection:
   // client.stop();
    //Serial.println("client disconnected");
  } // end of if(client)
        
}// void loop ends here...................................

void built_in_led_blink(void){
   digitalWrite(LED_BUILTIN, HIGH);
   delay(3000);
   digitalWrite(LED_BUILTIN, LOW);
   delay(50);
}

void WIFI_CONNECT(void){
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  delay(1000);
  
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid,password) ? "Ready" : "Failed!");
  delay(1000);
  
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  delay(1000);
  Serial.println("Server Ready");
}
