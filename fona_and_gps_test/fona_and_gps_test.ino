#include "Adafruit_FONA.h"
//#include "Gps.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define FONA_RX 12 
#define FONA_TX 13
#define FONA_RST 2

TinyGPSPlus gps;

SoftwareSerial fonaSS (FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

//  HardwareSerial *fonaSerial = &Serial;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

char replybuffer[255];
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
uint8_t type;

long timeData;

const int green =  16;
const int red =  0;
int ledState = LOW;

unsigned long previousMillis = 0;   

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);

  fonaSerial->begin(4800);
  
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    blynk(500, "red", 4); 
  }
  else {
    blynk(500, "green", 4);
  }
  
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  while (Serial.available() > 0) {
    if (gps.encode(Serial.read()));
    digitalWrite(16, HIGH);
    digitalWrite(0, HIGH);
  }
  timeData = gps.time.value();
  
  if (fona.available()) {
    fona.read();
  }
  if (!fona.available()) {
    blynk(100, "red", 8);
  }

        // send an SMS!
        char sendto[21] = "+421903442253"; 
        char message[35];
        
        //smartDelay(3000);
        
        dtostrf(timeData, 10, 0, message);
        
        if (!fona.sendSMS(sendto, message)) {
          digitalWrite(red, HIGH);
          delay(5000);
          digitalWrite(red, LOW);
        } else {
          digitalWrite(green, HIGH);
          delay(5000);
          digitalWrite(green, LOW);
        }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



static void smartDelay(unsigned long ms)
{

  unsigned long start = millis();
  do 
  {
    while (Serial.available())
      gps.encode(Serial.read());
  } while (millis() - start < ms);
  timeData = gps.time.value();
  //pinMode(4, INPUT);
}



void blynk(const long INTERVAL, char LED[]) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= INTERVAL) {
    previousMillis = currentMillis;


    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    if (LED == "green") digitalWrite(green, ledState);
    else if (LED == "red") digitalWrite(red, ledState);
  }
}



void blynk(const long INTERVAL, char LED[], int LOOPS) {
  unsigned long currentMillis = millis();

  for (int i=0; i<LOOPS; i++) {
    if (currentMillis - previousMillis >= INTERVAL) {
      previousMillis = currentMillis;
  
  
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
  
      if (LED == "green") digitalWrite(green, ledState);
      else if (LED == "red") digitalWrite(red, ledState);
    }
  }
}
