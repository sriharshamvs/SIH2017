#define LED 5
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 60000;           // interval at which to blink (milliseconds)
float windSpeed=0;
int statusLED=0;
//added
const byte IRinterruptPin = 3;
volatile byte count = LOW;
static unsigned char AnemometerCount=0;

void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT); 
  pinMode(IRinterruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IRinterruptPin), readShaft, RISING);
}
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // save the last time you blinked the LED
    Serial.print("Total AnemometerCount = ");
    Serial.println(AnemometerCount);
   AnemometerCount = 0;
    ReadSpeed();
    digitalWrite(LED,!statusLED);
   }// interval
   Serial.println(AnemometerCount);
}
void readShaft(){
  AnemometerCount++;
}
void ReadSpeed(void){
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


  /* else if(AnemometerCount>200 && AnemometerCount<=390){
      if(AnemometerCount<=210){ windSpeed = 1.60; }
      else if(AnemometerCount>100 && AnemometerCount<=110){ windSpeed  = 14.70; }
      else if(AnemometerCount>110 && AnemometerCount<=120){ windSpeed  = 19.31; }
      else if(AnemometerCount>120 && AnemometerCount<=130){ windSpeed  = 20.92; } 
      else if(AnemometerCount>130 && AnemometerCount<=140){ windSpeed  = 22.53; }
      else if(AnemometerCount>140 && AnemometerCount<=150){ windSpeed  = 24.14; }
      else if(AnemometerCount>150 && AnemometerCount<=160){ windSpeed  = 25.74; }
      else if(AnemometerCount>160 && AnemometerCount<=170){ windSpeed  = 27.35; } 
      else if(AnemometerCount>170 && AnemometerCount<=180){ windSpeed  = 28.96; }
      else if(AnemometerCount>180 && AnemometerCount<=190){ windSpeed  = 30.57; }
      else if(AnemometerCount>190 && AnemometerCount<=200){ windSpeed  = 32.18; }      
      else if(AnemometerCount>100 && AnemometerCount<=110){ windSpeed  = 14.70; }
      else if(AnemometerCount>110 && AnemometerCount<=120){ windSpeed  = 19.31; }
      else if(AnemometerCount>120 && AnemometerCount<=130){ windSpeed  = 20.92; } 
      else if(AnemometerCount>130 && AnemometerCount<=140){ windSpeed  = 22.53; }
      else if(AnemometerCount>140 && AnemometerCount<=150){ windSpeed  = 24.14; }
      else if(AnemometerCount>150 && AnemometerCount<=160){ windSpeed  = 25.74; }
      else if(AnemometerCount>160 && AnemometerCount<=170){ windSpeed  = 27.35; } 
      else if(AnemometerCount>170 && AnemometerCount<=180){ windSpeed  = 28.96; }
      else if(AnemometerCount>180 && AnemometerCount<=190){ windSpeed  = 30.57; }
      else if(AnemometerCount>190 && AnemometerCount<=200){ windSpeed  = 32.18; }      
      else if(AnemometerCount>100 && AnemometerCount<=110){ windSpeed  = 14.70; }
      else if(AnemometerCount>110 && AnemometerCount<=120){ windSpeed  = 19.31; }
      else if(AnemometerCount>120 && AnemometerCount<=130){ windSpeed  = 20.92; } 
      else if(AnemometerCount>130 && AnemometerCount<=140){ windSpeed  = 22.53; }
      else if(AnemometerCount>140 && AnemometerCount<=150){ windSpeed  = 24.14; }
      else if(AnemometerCount>150 && AnemometerCount<=160){ windSpeed  = 25.74; }
      else if(AnemometerCount>160 && AnemometerCount<=170){ windSpeed  = 27.35; } 
      else if(AnemometerCount>170 && AnemometerCount<=180){ windSpeed  = 28.96; }
      else if(AnemometerCount>180 && AnemometerCount<=190){ windSpeed  = 30.57; }
      else if(AnemometerCount>190 && AnemometerCount<=200){ windSpeed  = 32.18; }      
      Serial.println("Calm");
  }*/
}

