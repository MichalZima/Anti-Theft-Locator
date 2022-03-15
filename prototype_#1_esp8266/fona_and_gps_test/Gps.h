#include <TinyGPS++.h>
#include <SoftwareSerial.h>

      static const int RXPin = 4, TXPin = 5;
      static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

class Locate {
  public:
    long Time;
    
    void data() {


      unsigned long ms = 5000;

      ss.begin(GPSBaud);
      
      unsigned long start = millis();
      do 
      {
        while (ss.available())
          gps.encode(ss.read());
      } while (millis() - start < ms);
      Time = gps.time.value();
    }
  
}; 
