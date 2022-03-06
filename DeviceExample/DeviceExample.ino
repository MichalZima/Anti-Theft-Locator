#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;
int ledGreen = 2;
int ledRed = 4;
int ENABLE = 5;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device

void setup()
{
  Serial.begin(4800);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, HIGH);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial.available() > 0)
    if (gps.encode(Serial.read())){
      digitalWrite(ledRed, LOW);
      digitalWrite(ledGreen, HIGH);
    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
    while(true);
  }
}
