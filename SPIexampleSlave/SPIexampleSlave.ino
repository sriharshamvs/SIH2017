#include <dht.h>
#include <SFE_BMP180.h>
#include <Wire.h>

#define ALTITUDE 1655.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters

const int ldrPin = A0;

//volatile char tempbuff [20] = "Jai Sri Ram";
unsigned int pos;
bool Active;

unsigned char tempbuff[20];
bool SensorFlag = LOW;             // SensorFlag used to set the Flag
unsigned long previousMillis = 0;
const long interval = 2000;      
unsigned char counterFlag=0;
unsigned char ldrValue=0;

bool AnemometerFlag=LOW;
unsigned long previousMillis2 = 0;        // will store last time LED was updated
const long interval2 = 60000;           // interval at which to blink (milliseconds)
float windSpeed=0;
const byte IRinterruptPin = 3;
volatile byte count = LOW;
static unsigned char AnemometerCount=0;
unsigned char windSpeedTochar=0;

const unsigned char PHpin = A0;
unsigned char PHvalue=0;

dht DHT;
SFE_BMP180 pressure;

void setup (void){
  Serial.begin(115200);
  pinMode(IRinterruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IRinterruptPin), readShaft, RISING);
  pinMode(MISO, OUTPUT); // have to send on master in, *slave out*
  // Initialize the sensor (it is important to get calibration values stored on the device).
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else{
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
  SPCR |= bit(SPE); // turn on SPI in slave mode
  SPCR |= bit(SPIE); // turn on interrupts
}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect){
  
  byte c = SPDR;
  
  if (c == 1){  // starting new sequence?
    Active = true;
    pos = 0;
    SPDR = tempbuff [pos++];   // send first byte
    return;
    }

  if (!Active){
    SPDR = 0;
    return;
  }

  SPDR = tempbuff [pos];
  if (tempbuff [pos] == 0 || ++pos >= sizeof (tempbuff)){
     Active = false;
  }
  
 counterFlag++;
}  // end of interrupt service routine (ISR) SPI_STC_vect

void loop (void){
 
  unsigned long currentMillis = millis(); 
  if (currentMillis - previousMillis >= interval) {
   previousMillis = currentMillis;
   SensorFlag = HIGH;
   ReadSensors();  
  }   
  unsigned long currentMillis2 = millis();
  if (currentMillis2 - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis2;  // save the last time you blinked the LED
    AnemometerCount = 0;
    AnemometerFlag=HIGH;
    ReadSpeed();
   }// interval
}// end of loop

void readShaft(){
  AnemometerCount++;
}

void ReadSensors(){
 char status;
 double T,P,p0,a; 
 unsigned int a_x=0,p0_x=0,P_x=0;
 PHvalue = analogRead(PHpin);
 DHT.read11(7);
  
 while(SensorFlag){
  tempbuff[0] = char(DHT.temperature);
  tempbuff[1] = 1;
  tempbuff[2] = char(DHT.humidity);
  tempbuff[3] = 1;
  //ldrValue = analogRead(ldrPin);
  tempbuff[4]= char(analogRead(ldrPin));
  
  status = pressure.startPressure(3);
  
  Serial.print(tempbuff[0]);
  Serial.print(" *C\t");
  Serial.print(tempbuff[2]);
  Serial.print(" %\t");
  Serial.print(tempbuff[4]);
  Serial.println(" Cd\t");

  status = pressure.startTemperature();
  if (status != 0){
    // Wait for the measurement to complete:
    delay(status);
    
    status = pressure.getTemperature(T);
    if (status != 0){
      // Print out the measurement:
     /* Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");
      */
      status = pressure.startPressure(3);
      if (status != 0){
        // Wait for the measurement to complete:
        delay(status);

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          P_x = P;
          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          p0_x = p0;
          a = pressure.altitude(P,p0);
          a_x = a;
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          /*Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");
          
          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          p0_x = p0;
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");
          
          a = pressure.altitude(P,p0);
          a_x = a;
          Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");*/
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  tempbuff[5]= 1;
  tempbuff[6]= char(P_x/100);
  tempbuff[7]= char(P_x%100);
  tempbuff[8]= 1;
  windSpeedTochar=char(windSpeed);
  tempbuff[9]= char(windSpeedTochar);
  
  SensorFlag = LOW;  
 
 }// Whlile condition ends here
} // ReadSensor ends here

void ReadSpeed(void){
  if(AnemometerFlag){
    AnemometerFlag=LOW;
    if(AnemometerCount>0 && AnemometerCount<=50){
      if(AnemometerCount=10){ windSpeed = 17.60; }
      else if(AnemometerCount>10 && AnemometerCount<=20){ windSpeed  = 19.21; }
      else if(AnemometerCount>20 && AnemometerCount<=30){ windSpeed  = 20.82; }
      else if(AnemometerCount>30 && AnemometerCount<=40){ windSpeed  = 22.43; }
      else if(AnemometerCount>40 && AnemometerCount<=50){ windSpeed  = 24.04; } 
      Serial.println("Calm");
   }
   else if(AnemometerCount>50 && AnemometerCount<=200){
      if(AnemometerCount<=50){ windSpeed = 17.6; }
      else if(AnemometerCount>50 && AnemometerCount<=60){ windSpeed  = 25.65; }
      else if(AnemometerCount>60 && AnemometerCount<=70){ windSpeed  = 27.26; }
      else if(AnemometerCount>70 && AnemometerCount<=80){ windSpeed  = 28.87; }
      else if(AnemometerCount>80 && AnemometerCount<=90){ windSpeed  = 30.48; } 
      else if(AnemometerCount>90 && AnemometerCount<=100){ windSpeed  = 31.09; }
      else if(AnemometerCount>100 && AnemometerCount<=110){ windSpeed  = 33.60; }
      else if(AnemometerCount>110 && AnemometerCount<=120){ windSpeed  = 35.31; }
      else if(AnemometerCount>120 && AnemometerCount<=130){ windSpeed  = 36.92; } 
      else if(AnemometerCount>130 && AnemometerCount<=140){ windSpeed  = 38.53; }
      else if(AnemometerCount>140 && AnemometerCount<=150){ windSpeed  = 40.14; }
      else if(AnemometerCount>150 && AnemometerCount<=160){ windSpeed  = 41.74; }
      else if(AnemometerCount>160 && AnemometerCount<=170){ windSpeed  = 43.35; } 
      else if(AnemometerCount>170 && AnemometerCount<=180){ windSpeed  = 44.96; }
      else if(AnemometerCount>180 && AnemometerCount<=190){ windSpeed  = 46.57; }
      else if(AnemometerCount>190 && AnemometerCount<=200){ windSpeed  = 48.18; }      
      Serial.println("Light");
   }
 }
}



