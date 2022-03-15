
#include <TinyGPS++.h>
#include "VARIABLES.h"

TinyGPSPlus gps;

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
      Serial2.begin(GPSBaud, SERIAL_8N1, GPS_RXD2, GPS_TXD2);
      delay(1000); 
    }
    
    /*----------------------------------------------------------------------------------------------------------------------------------------*/

     void updateData(){
      if (gps.charsProcessed() < 10) {
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
    
    void signalStrength() {   
      if (Hdop <= 4) {
        signalIndex = 3;
      }
      else if (Hdop > 4 && Hdop <= 15) {
        signalIndex = 2;
      }        
      else if (Hdop > 15 && Hdop < 99) {
        signalIndex = 1;
      }        
      else if (Hdop >= 99) {
        signalIndex = 0;        
      }
    }

    /*----------------------------------------------------------------------------------------------------------------------------------------*/ 
        
    void smartDelay(unsigned long period) {
      unsigned long start = millis();
      do {
        while (Serial2.available()) {
          Serial.write(gps.encode(Serial2.read()));
        }                    
        updateData();
      } while (millis() - start < period);  
          
    }    
    
    /*----------------------------------------------------------------------------------------------------------------------------------------*/ 

}; 