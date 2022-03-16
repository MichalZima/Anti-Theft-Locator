
#include <SoftwareSerial.h>
#include "GPS.h"

SoftwareSerial fona (FONA_TX, FONA_RX);
GPS mygps;

class GSM {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  private:

    char*  latString;
    char*  lngString;
    String sms_status;
    String sender_number;
    String received_date;
    String msg;
    const String PHONE = "+421948462229";
    const char* pin = {"0000"};
    const int id = 123;
    bool replyStatus = true;
    char responseSMS[1024];   
          
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  public: 

    /*-----------------------------------------------------------------------------------------------------------------------*/

    void Startup() {
      pinMode(GSMEnablePin, OUTPUT);
      digitalWrite(GSMEnablePin, HIGH);  
    }
    
    /*-----------------------------------------------------------------------------------------------------------------------*/

    void Init() {     
      fona.begin(4800);
      Serial.println("\ninitializing...");
    
      fona.println("AT"); //Once the handshake test is successful, it will back to OK
      Serial.println("AT");
      updateSerial();

      delay(2000);
      char pincmd[14] = "AT+CPIN=";
      pincmd[8] = pin[0];
      pincmd[9] = pin[1];
      pincmd[10] = pin[2];
      pincmd[11] = pin[3];
      pincmd[12] = '\0';
      fona.println(pincmd);
      Serial.println(pincmd);
      delay(2000);
      updateSerial();
      
      fona.println("AT+CMGF=1"); // Configuring TEXT mode
      Serial.println("AT+CMGF=1");
      updateSerial();
      
      fona.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS responses should be handled
      Serial.println("AT+CNMI=1,2,0,0,0");
      updateSerial();
    }          

    /*---------------------------------------------------------------------------------------------------------------------------------*/

    void updateSerial() {
      delay(100);
      while (Serial.available()) {
        fona.write(Serial.read());//Forward what Serial received to Software Serial Port
      }

      while(fona.available()) {
        // digitalWrite(GPSEnablePin, LOW);
        parseresponse(fona.readString()); //Forward what Software Serial received to Serial Port
      }
    }

    /*---------------------------------------------------------------------------------------------------------------------------------*/  

    void parseresponse(String buff) {  // buff = AT response from gsm module
      Serial.println(buff);

      unsigned int len, index;
  
      //Remove sent "AT Command" from the response string.
      index = buff.indexOf("\r");
      buff.remove(0, index+2);
      buff.trim();
      Serial.println(buff);
  
      if(buff != "OK"){
        index = buff.indexOf(":");
        String cmd = buff.substring(0, index);
        cmd.trim();
        
        buff.remove(0, index+2);        
        
        if(cmd == "+CMTI"){
          //get newly arrived memory location and store it in temp
          //temp = 4
          index = buff.indexOf(",");
          String temp = buff.substring(index+1, buff.length()); 
          temp = "AT+CMGR=" + temp + "\r"; 
          //AT+CMGR=4 i.e. get response stored at memory location 4
          fona.println(temp); 
        }
        else if(cmd == "+CMGR"){
          extractSms(buff);
          if(sender_number == PHONE){
            doAction();
            fona.println("AT+CMGD=1,4"); 
            updateSerial();
          }
        }
        else if (cmd == "+CMT") {
          extractSms(buff);
          if(sender_number == PHONE){
            doAction();
            fona.println("AT+CMGD=1,4"); 
            updateSerial();
          }
          else{                      
            fona.println("AT+CMGD=1,4"); 
            updateSerial();          
          }
        }
      }
//      else if (buff == "OK") {
//        while (!myled.functionDone) myled.Light("green", 1000);
//      } 
//      else if (buff == "ERROR") {
//        while (!myled.functionDone) myled.Light("red", 1000);
//      }
    }

    /*---------------------------------------------------------------------------------------------------------------------------------*/

    void extractSms(String buff){
      unsigned int index;

      index =buff.indexOf(",");
      // sms_status = buff.substring(1, index-1); 
      // buff.remove(0, index+2);
      
      sender_number = buff.substring(1, index-1);
      buff.remove(0,19);
    
      received_date = buff.substring(0, 20);
      buff.remove(0,buff.indexOf("\r"));
      buff.trim();
      
      index =buff.indexOf("\n\r");
      buff = buff.substring(0, index);
      buff.trim();
      msg = buff;
      buff = "";
      msg.toLowerCase();

      Serial.println("extracting");
      Serial.println(sender_number);
      Serial.println(received_date);
      Serial.println(msg);
    }

    /*---------------------------------------------------------------------------------------------------------------------------------*/

    void doAction() {
      char responseSMS[1024];
      Serial.println("doing action");
      
      if(msg == "id"){
        char cstr[16];
        dtostrf(id, 5, 0, cstr);
        strcpy(responseSMS, "id: ");
        strcat(responseSMS, cstr);                  
        if(replyStatus == true){
          Serial.println("sending sms:");
          sendSms(responseSMS);
        }
      }
      
      if(msg == "poloha"){
        Serial.println("msg == poloha");
        if (mygps.signalIndex > 1) {
          Serial.print("signaIndex: ");
          Serial.println(mygps.signalIndex);
          if (mygps.hasLocation) {       
            if(replyStatus == true){
              Serial.println("everythig true");
              dtostrf(mygps.Lat, 10, 7, latString);
              dtostrf(mygps.Lng, 10, 7, lngString);
              Serial.println("dtostrf done");
              strcpy(responseSMS, "http://maps.google.com/maps?q=loc:");
              strcat(responseSMS, latString);
              strcat(responseSMS, ",");
              strcat(responseSMS, lngString);
              strcat(responseSMS, "\0");
              Serial.println("strcat done");
              Serial.println("response SMS:");
              Serial.println(responseSMS);
              sendSms(responseSMS);
            }
          }   
          else sendSms("no location");
        }
        else sendSms("no signal");
      }
      
      sms_status = "";
      sender_number="";
      received_date="";
      msg="";  
    }

    /*---------------------------------------------------------------------------------------------------------------------------------*/

    void sendSms(String text) {
      Serial.println("sendSms() run");
      fona.print("AT+CMGF=1\r");
      Serial.print("AT+CMGF=1\r");
      delay(1000);
      fona.print("AT+CMGS=\""+sender_number+"\"\r");
      Serial.print("AT+CMGS=\""+sender_number+"\"\r");
      delay(1000);
      fona.print(text);
      Serial.print(text);
      delay(100);
      fona.write(0x1A); //ascii code for ctrl-26 //fona.println((char)26); //ascii code for ctrl-26
      Serial.write(0x1A);
      delay(500);
    }

};