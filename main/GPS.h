
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "LED.h"

TinyGPSPlus gps;
LED myled;

class GPS {
  
  private:    
                  
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
  
    void Startup(){
      pinMode(GPSEnablePin, OUTPUT);
      digitalWrite(GPSEnablePin, HIGH);      
    }    

    /*----------------------------------------------------------------------------------------------------------------------------------------*/
    
    void Init(){
      Serial.begin(GPSBaud);
      smartDelay(1000); 
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
    
    int signalStrength() {   
      if (hdop <= 4) {
        myled.Light("green", 250);
        signalIndex = 3;
      }
      else if (hdop > 4 && hdop <= 15) {
        myled.Light("orange", 250);      
        signalIndex = 2;
      }        
      else if (hdop > 15 && hdop < 99) {
        myled.Light("red", 250);
        signalIndex = 1;
      }        
      else if (hdop >= 99) {
        myled.Light("red");      
        signalIndex = 0;        
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