#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "LED.h"

//static const int RXPin = 1, TXPin = 3; //HARDWARE(PROGRAM) UART
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
//SoftwareSerial ss(RXPin, TXPin);

LED myled;



class GPS {
  
  private:

    uint8_t enablePin = 5;      
                  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  public:
    long Time;
    float speed;
    float hdop = 100.0;    
    uint8_t sats = 0;
    double lat;
    double lng;
    bool hasLocation = false;
    uint8_t signalIndex = 0;
        
    /*----------------------------------------------------------------------------------------------------------------------------------------*/   
  
    void Init(){
      pinMode(enablePin, OUTPUT);
      digitalWrite(enablePin, HIGH);
      //delay(1000);
      Serial.begin(9600);      
    }    

    /*----------------------------------------------------------------------------------------------------------------------------------------*/   
    
    int signalStrength() {   
      if (hdop <= 4) {
        myled.Blik(250, "green");
        signalIndex = 3;
      }
      else if (hdop > 4 && hdop <= 15) {
        myled.Blik(250, "orange");      
        signalIndex = 2;
      }        
      else if (hdop > 15 && hdop < 99) {
        myled.Blik(250, "red");
        signalIndex = 1;
      }        
      else if (hdop >= 99) {
        myled.Light("red");      
        signalIndex = 0;        
      }
    }

    /*----------------------------------------------------------------------------------------------------------------------------------------*/ 

    bool updateData(){
      if (gps.charsProcessed() < 10) {
        myled.Blik(100, "red", 3);
        return false;
      }
      else if (gps.charsProcessed() >= 10) {            
        if (gps.hdop.isUpdated() && gps.hdop.isValid()) hdop = gps.hdop.hdop();
        if (gps.satellites.isUpdated() && gps.satellites.isValid()) sats = gps.satellites.value();      
        if (gps.location.isUpdated() && gps.location.isValid()) {
          lat = gps.location.lat();                           
          lng = gps.location.lng();
          hasLocation = true;          
        }
        signalStrength();
        return true;
      }
    }        
        
    /*----------------------------------------------------------------------------------------------------------------------------------------*/ 
        
    void smartDelay(unsigned long period) {
      unsigned long start = millis();
      do {
        while (Serial.available()) {
          gps.encode(Serial.read());
          updateData();
        }                    
      } while (millis() - start < period);  
          
    }    
    
    /*----------------------------------------------------------------------------------------------------------------------------------------*/ 

}; 