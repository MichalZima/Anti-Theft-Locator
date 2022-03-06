
class LED {

  private:

    const int greenLedPin = 4;
    const int redLedPin = 2;
    int ledState = LOW;  
    unsigned long previousMillis = 0;
    

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  public:
 
    /*----------------------------------------------------------------------------------------------------------------------------------------*/   
  
    void Init() {
      pinMode(greenLedPin, OUTPUT);
      pinMode(redLedPin, OUTPUT);    
      digitalWrite(greenLedPin, LOW);
      digitalWrite(greenLedPin, LOW);
    }  

    /*----------------------------------------------------------------------------------------------------------------------------------------*/     

    void Blik(const long INTERVAL, char COLOR[]) {
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= INTERVAL) {
        previousMillis = currentMillis;


        if (ledState == LOW) {
          ledState = HIGH;
        } else {
          ledState = LOW;
        }

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
          digitalWrite(redLedPin, ledState);            
        }
      }
    }

    /*----------------------------------------------------------------------------------------------------------------------------------------*/     

    void Blik(const long INTERVAL, char COLOR[], int LOOPS) {
      unsigned long currentMillis = millis();

      for (int i = 0; i < LOOPS; i++) {
        if (currentMillis - previousMillis >= INTERVAL) {
          previousMillis = currentMillis;


          if (ledState == LOW) {
            ledState = HIGH;
          } else {
            ledState = LOW;
          }

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
            digitalWrite(redLedPin, ledState);            
          }            
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
    
};


