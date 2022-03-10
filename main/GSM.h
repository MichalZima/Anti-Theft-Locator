
#include <SoftwareSerial.h>
#include "GPS.h"

SoftwareSerial fona (FONA_TX, FONA_RX);
GPS mygps;

class GSM {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  private:

    char  latString[20];
    char  lngString[20];
    String sms_status;
    String sender_number;
    String received_date;
    String msg;
    const String PHONE = "+421948462229";
    char pin[5] = {"0000"};
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
      delay(3000); 
      //Serial.println("\ninitializing...");
    
      fona.println("AT"); //Once the handshake test is successful, it will back to OK
      updateSerial();

      delay(2000);
      char pincmd[14] = "AT+CPIN=";
      pincmd[8] = pin[0];
      pincmd[9] = pin[1];
      pincmd[10] = pin[2];
      pincmd[11] = pin[3];
      pincmd[12] = '\0';
      fona.println(pincmd);
      delay(2000);
      updateSerial();
      
      fona.println("AT+CMGF=1"); // Configuring TEXT mode
      updateSerial();
      
      fona.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS responses should be handled
      updateSerial();
      delay(5000);
    
    }          

    /*---------------------------------------------------------------------------------------------------------------------------------*/

    void updateSerial() {
      delay(100);
//       while (Serial.available()) 
//       {
//         fona.write(Serial.read());//Forward what Serial received to Software Serial Port
//       }
      while(fona.available()) 
      {
        parseresponse(fona.readString()); //Forward what Software Serial received to Serial Port
      }
    }

    /*---------------------------------------------------------------------------------------------------------------------------------*/  

    void parseresponse(String buff) {
      //Serial.println(buff);

      unsigned int len, index;
  
      //Remove sent "AT Command" from the response string.
      index = buff.indexOf("\r");
      buff.remove(0, index+2);
      buff.trim();
  
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
    }

    /*---------------------------------------------------------------------------------------------------------------------------------*/

    void doAction() {
      char responseSMS[1024];
      
      if(msg == "id"){
        char cstr[16];
        dtostrf(id, 5, 0, cstr);
        strcpy(responseSMS, "id: ");
        strcat(responseSMS, cstr);                  
        if(replyStatus == true){
          sendSms(responseSMS);
        }
      }
      
      if(msg == "poloha"){
        if (mygps.signalIndex == 3) {
          if (mygps.hasLocation) {       
            if(replyStatus == true){
              dtostrf(mygps.Lat, 11, 9, latString);
              dtostrf(mygps.Lng, 11, 9, lngString);
              strcpy(responseSMS, "http://maps.google.com/maps?q=loc:");
              strcat(responseSMS, latString);
              strcat(responseSMS, ",");
              strcat(responseSMS, lngString);
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
      fona.print("AT+CMGF=1\r");
      delay(1000);
      fona.print("AT+CMGS=\""+sender_number+"\"\r");
      delay(1000);
      fona.print(text);
      delay(100);
      fona.write(0x1A); //ascii code for ctrl-26 //fona.println((char)26); //ascii code for ctrl-26
      delay(1000);
    }

};
