
#include "VALUES.h"

//VALUES myvalues;


class LED {

  private:

    int ledState = HIGH; 
    bool functionDone = true;
    uint32_t previousMillis = 0;
    uint8_t i = 0; 
    
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  public:
 
    /*----------------------------------------------------------------------------------------------------------------------------------------*/   
  
    void Init() {
      pinMode(greenLedPin, OUTPUT);
      pinMode(redLedPin, OUTPUT);          
    }  

    /*----------------------------------------------------------------------------------------------------------------------------------------*/

    void Test() {
      digitalWrite(greenLedPin, HIGH);
      delay(1000);      
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, HIGH);
      delay(1000);
      digitalWrite(redLedPin, LOW);     
      delay(1000);

      Blik(250, "red", 3);
      delay(1000);
      Blik(250, "orange", 3);
      delay(1000);
      Blik(250, "green", 3);
      delay(1000);   

      Light("green", 1000);
      delay(1000);
      Light("orange", 1000);
      delay(1000);
      Light("red", 1000);
      delay(1000);
    }

    /*----------------------------------------------------------------------------------------------------------------------------------------*/     

    void Blik(const long INTERVAL, char COLOR[], int LOOPS) {
      if (functionDone) {
        previousMillis = millis();
        i = 0;
        functionDone = false;
      }
      
      if (i < LOOPS*2) {

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
          ledState = !ledState;
          previousMillis = millis();
          i++;          
        } 
      }
      else {
        functionDone = true;
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
      if (functionDone) {
        previousMillis = millis();
        functionDone = false;
      }
           
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
             
      if (millis() - previousMillis < INTERVAL);
      else {
        functionDone = true;
        digitalWrite(greenLedPin, LOW);
        digitalWrite(redLedPin, LOW);
      }
    }

};


