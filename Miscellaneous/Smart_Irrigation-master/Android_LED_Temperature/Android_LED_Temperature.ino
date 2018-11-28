#include <ESP8266WiFi.h>

#define LED D1

const char* ssid = "your ssid";
const char* password = "your password";

float vref = 3.3;
float resolution = vref/1023;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(LED,OUTPUT);               // LED
  wifi_connect();                   // Connecting to WiFi
  server.begin();                   // Begin server 
}

void loop() {
  // put your main code here, to run repeatedly:
  
  built_in_led_blink();
 float t = analogRead(A0);
 t = (t*resolution);
 t = t*100;
  
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
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
            client.print("<p style = 'color: #FFDF00;'><strong style='font-size: large;'>&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp ROOM TEMPERATURE = ");
            client.print(t);
            client.print("&nbsp<sup>o</sup>C</span></strong></p>");
            client.print("<br>");
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
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED, LOW);                // GET /L turns the LED off
        }
      }// end of if(client.available())
    }// end of while(client.disconnect())
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  } // end of if(client)
        
}// void loop ends here...................................

void built_in_led_blink(void){
   digitalWrite(LED_BUILTIN, HIGH);
   delay(3000);
   digitalWrite(LED_BUILTIN, LOW);
   delay(50);
}

void wifi_connect(void){
  
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


