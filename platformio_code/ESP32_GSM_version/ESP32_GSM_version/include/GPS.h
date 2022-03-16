
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

    void printData() {
        static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;

        printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
        printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
        printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
        printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
        printInt(gps.location.age(), gps.location.isValid(), 5);
        printDateTime(gps.date, gps.time);
        printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
        printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
        printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
        printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);

        unsigned long distanceKmToLondon =
          (unsigned long)TinyGPSPlus::distanceBetween(
            gps.location.lat(),
            gps.location.lng(),
            LONDON_LAT, 
            LONDON_LON) / 1000;
        printInt(distanceKmToLondon, gps.location.isValid(), 9);

        double courseToLondon =
          TinyGPSPlus::courseTo(
            gps.location.lat(),
            gps.location.lng(),
            LONDON_LAT, 
            LONDON_LON);

        printFloat(courseToLondon, gps.location.isValid(), 7, 2);

        const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);

        printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);

        printInt(gps.charsProcessed(), true, 6);
        printInt(gps.sentencesWithFix(), true, 10);
        printInt(gps.failedChecksum(), true, 9);
        Serial.println();
    }







      static void printFloat(float val, bool valid, int len, int prec)
      {
        if (!valid)
        {
          while (len-- > 1)
            Serial.print('*');
          Serial.print(' ');
        }
        else
        {
          Serial.print(val, prec);
          int vi = abs((int)val);
          int flen = prec + (val < 0.0 ? 2 : 1); // . and -
          flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
          for (int i=flen; i<len; ++i)
            Serial.print(' ');
        }
      }

      static void printInt(unsigned long val, bool valid, int len)
      {
        char sz[32] = "*****************";
        if (valid)
          sprintf(sz, "%ld", val);
        sz[len] = 0;
        for (int i=strlen(sz); i<len; ++i)
          sz[i] = ' ';
        if (len > 0) 
          sz[len-1] = ' ';
        Serial.print(sz);
      }

      static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
      {
        if (!d.isValid())
        {
          Serial.print(F("********** "));
        }
        else
        {
          char sz[32];
          sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
          Serial.print(sz);
        }
        
        if (!t.isValid())
        {
          Serial.print(F("******** "));
        }
        else
        {
          char sz[32];
          sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
          Serial.print(sz);
        }

        printInt(d.age(), d.isValid(), 5);
      }

      static void printStr(const char *str, int len)
      {
        int slen = strlen(str);
        for (int i=0; i<len; ++i)
          Serial.print(i<slen ? str[i] : ' ');
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
          (gps.encode(Serial2.read()));
        }                    
        updateData();
        // printData();
      } while (millis() - start < period);   
    }    
    
    /*----------------------------------------------------------------------------------------------------------------------------------------*/ 

}; 