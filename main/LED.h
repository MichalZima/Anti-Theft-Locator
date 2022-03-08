
#include "VALUES.h"


class LED {

  private:

    int ledState = 1;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  public:
 
    /*----------------------------------------------------------------------------------------------------------------------------------------*/   
  
    void Init() {
      pinMode(greenLedPin, OUTPUT);
      pinMode(redLedPin, OUTPUT);          
    }  

    /*----------------------------------------------------------------------------------------------------------------------------------------*/

    void Test() {
      Serial.println("\n1. segment start");
      digitalWrite(greenLedPin, HIGH);
      Serial.println("  green on");
      delay(1000);      
      digitalWrite(greenLedPin, LOW);
      Serial.println("  green off");      
      Serial.println("  red on");
      digitalWrite(redLedPin, HIGH);
      delay(1000);
      digitalWrite(redLedPin, LOW); 
      Serial.println("  red off");     
      delay(1000);

      Serial.println("1. segment done");

      Blik(250, "red", 3);
      delay(1000);
      Blik(250, "orange", 3);
      delay(1000);
      Blik(250, "green", 3);
      delay(1000);   

      Serial.println("2. segment done");

      Light("green", 1000);
      delay(1000);
      Light("orange", 1000);
      delay(1000);
      Light("red", 1000);
      delay(1000);

      Serial.println("3. segment done");
    }

    /*----------------------------------------------------------------------------------------------------------------------------------------*/     

    void Blik(const long INTERVAL, char COLOR[], int LOOPS) {
      uint32_t previousMillis = millis();
      uint8_t i = 0;
      Serial.println("print 2.1");

      while (i < LOOPS*2) {
        Serial.println("print 2.2");    
        Serial.println("print 2.3");  

        if (COLOR == "green") {
          digitalWrite(greenLedPin, ledState);
          digitalWrite(redLedPin, LOW);            
        }
        if (COLOR == "red") {
          digitalWrite(redLedPin, ledState);
          digitalWrite(greenLedPin, LOW);            
        }
        if (COLOR == "orange") {
          digitalWrite(redLedPin, ledState);
          digitalWrite(greenLedPin, ledState);            
        }
                        
        if (millis() - previousMillis >= INTERVAL/2) {
          Serial.println("print 2.4");
          ledState = !ledState;
          previousMillis = millis();
          i++;          
        } 
      }
    }

    /*----------------------------------------------------------------------------------------------------------------------------------------*/     

    void Light(char COLOR[8]) {
      if (COLOR == "green") {
        digitalWrite(greenLedPin, HIGH);
        digitalWrite(redLedPin, LOW);        
      }
      if (COLOR == "red") {
        digitalWrite(greenLedPin, LOW);
        digitalWrite(redLedPin, HIGH);        
      }
      if (COLOR == "orange") {
        digitalWrite(greenLedPin, HIGH);
        digitalWrite(redLedPin, HIGH);        
      }
      if (COLOR == "none") {
        digitalWrite(greenLedPin, LOW);
        digitalWrite(redLedPin, LOW);        
      }  
    }

    /*----------------------------------------------------------------------------------------------------------------------------------------*/     

    void Light(char COLOR[8], uint32_t INTERVAL) {
      uint32_t previousMillis = millis();
      Serial.println("print 3.1");
           
      if (COLOR == "green") {
        digitalWrite(greenLedPin, HIGH);
        digitalWrite(redLedPin, LOW);        
      }
      if (COLOR == "red") {
        digitalWrite(greenLedPin, LOW);
        digitalWrite(redLedPin, HIGH);        
      }
      if (COLOR == "orange") {
        digitalWrite(greenLedPin, HIGH);
        digitalWrite(redLedPin, HIGH);        
      }       
      while (millis() - previousMillis < INTERVAL);
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, LOW);
    }

};


