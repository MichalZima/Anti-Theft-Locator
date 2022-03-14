
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
    float Speed;
    float Hdop = 100.0;    
    uint8_t sats = 0;
    double Lat;
    double Lng;
    bool hasLocation = false;
    uint8_t signalIndex = 0;
        
    /*----------------------------------------------------------------------------------------------------------------------------------------*/   
  
    void Startup(){
      pinMode(GPSEnablePin, OUTPUT);
      digitalWrite(GPSEnablePin, HIGH); 
      delay(1000);     
    }    

    /*----------------------------------------------------------------------------------------------------------------------------------------*/
    
    void Init(){
      Serial.begin(GPSBaud);
      delay(1000); 
    }
    
    /*----------------------------------------------------------------------------------------------------------------------------------------*/

     void updateData(){
      if (gps.charsProcessed() < 10) {
        myled.Light("green");
        delay(5000);
      }
      else if (gps.charsProcessed() >= 10) {            
        if (gps.hdop.isUpdated()) Hdop = gps.hdop.hdop();
        if (gps.satellites.isValid()) sats = gps.satellites.value();      
        if (gps.location.isValid()) {
          Lat = gps.location.lat();                           
          Lng = gps.location.lng();
          hasLocation = true;          
        }
        signalStrength();
      }
    }        
        
    /*----------------------------------------------------------------------------------------------------------------------------------------*/    
    
    int signalStrength() {   
      if (Hdop <= 4) {
        //myled.Light("green");
        signalIndex = 3;
      }
      else if (Hdop > 4 && Hdop <= 15) {
        //myled.Light("orange");      
        signalIndex = 2;
      }        
      else if (Hdop > 15 && Hdop < 99) {
        //myled.Light("red");
        signalIndex = 1;
      }        
      else if (Hdop >= 99) {
        //myled.Light("red");      
        signalIndex = 0;        
      }
    }

    /*----------------------------------------------------------------------------------------------------------------------------------------*/ 
        
    void smartDelay(unsigned long period) {
      unsigned long start = millis();
      do {
        while (Serial.available()) {
          gps.encode(Serial.read());
        }                    
        updateData();
      } while (millis() - start < period);  
          
    }    
    
    /*----------------------------------------------------------------------------------------------------------------------------------------*/ 

}; 
