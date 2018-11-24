#include <ESP8266WiFi.h>

#define MOTOR_RELAY D7
#define relay_s1 D5
#define relay_s2 D6

char ssid[] = "ssid";          //  your network SSID (name) 
char pass[] = "passwd";   // your network password

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  pinMode(MOTOR_RELAY,OUTPUT);
  pinMode(relay_s1,OUTPUT);             
  pinMode(relay_s2,OUTPUT);           
  digitalWrite(MOTOR_RELAY,HIGH);
  digitalWrite(relay_s1,HIGH);
  digitalWrite(relay_s2,HIGH);


                 
  wifi_connect();                   // Connecting to WiFi
  server.begin();                   // Begin server 
}

void loop() {
  // put your main code here, to run repeatedly:
  
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
            
            client.print("&nbsp<sup>o</sup>C</span></strong></p>");
            client.print("<p style = 'color: #FFDF00;'><strong style='font-size: large;'>&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp HUMIDITY = ");
           
            client.print("&nbsp&nbsp %");
            client.print("&nbsp<sup>o</sup>C</span></strong></p>");
            client.print("<br>");;
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
          digitalWrite(MOTOR_RELAY, LOW);               // GET /H turns the LED on
          Serial.println("Motor ON");
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(MOTOR_RELAY, HIGH);                // GET /L turns the LED off
          Serial.println("Motor OFF");
        }
        if (currentLine.endsWith("GET /A")) {
          digitalWrite(relay_s1, LOW);               // GET /H turns the LED on
          Serial.println("Sol 1 ON");
        }
        if (currentLine.endsWith("GET /B")) {
          digitalWrite(relay_s1, HIGH);                // GET /L turns the LED off
          Serial.println("Sol 1 OFF");
        }
        if (currentLine.endsWith("GET /C")) {
          digitalWrite(relay_s2, LOW);               // GET /H turns the LED on
          Serial.println("Sol 2 ON");
        }
        if (currentLine.endsWith("GET /D")) {
          digitalWrite(relay_s2, HIGH);                // GET /L turns the LED off
          Serial.println("Sol 2 OFF");
        }
      }// end of if(client.available())
    }// end of while(client.disconnect())
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  } // end of if(client)
  delay(1250);      
}// void loop ends here...................................

void wifi_connect(void){
  
  Serial.begin(115200);
  delay(10);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass); 
  
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


