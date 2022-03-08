
#include <SoftwareSerial.h>
#include "GPS.h"

SoftwareSerial fona (FONA_TX, FONA_RX);
GPS mygps;

class GSM {

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  private:

    /*-----------------------------------------------------------------------------------------------------------------------*/
  
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

    /*-----------------------------------------------------------------------------------------------------------------------*/  
          
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  public: 

    /*-----------------------------------------------------------------------------------------------------------------------*/

    void Startup() {
      pinMode(GSMEnablePin, OUTPUT);
      digitalWrite(GSMEnablePin, HIGH);  
    }
    
    /*-----------------------------------------------------------------------------------------------------------------------*/

    void Init() {     
      fona.begin(4800);
      mygps.smartDelay(3000); 
      //Serial.println("\ninitializing...");
    
      fona.println("AT"); //Once the handshake test is successful, it will back to OK
      updateSerial();

      mygps.smartDelay(2000);
      char pincmd[14] = "AT+CPIN=";
      pincmd[8] = pin[0];
      pincmd[9] = pin[1];
      pincmd[10] = pin[2];
      pincmd[11] = pin[3];
      pincmd[12] = '\0';
      fona.println(pincmd);
      mygps.smartDelay(2000);
      updateSerial();
      
      fona.println("AT+CMGF=1"); // Configuring TEXT mode
      updateSerial();
      
      fona.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS responses should be handled
      updateSerial();
    
    }          

    /*---------------------------------------------------------------------------------------------------------------------------------*/    

    void parseresponse(String buff) {
      //Serial.println("print3.0  ");
      //Serial.println(buff);

      unsigned int len, index;
  
      //Remove sent "AT Command" from the response string.
      index = buff.indexOf("\r");
      buff.remove(0, index+2);
      buff.trim();
  
      //Serial.print("print3.1  ");
      //Serial.println(buff); //Response string
  
      if(buff != "OK"){
        index = buff.indexOf(":");
        String cmd = buff.substring(0, index);
        //Serial.print("print3.2  ");
        //Serial.println(cmd);
        cmd.trim();
        //Serial.print("print3.3  ");
        //Serial.println(cmd);
        
        buff.remove(0, index+2);
        //Serial.print("print3.4  ");
        //Serial.println(buff);
        //Serial.println(cmd);        
        
        if(cmd == "+CMTI"){
          //get newly arrived memory location and store it in temp
          //temp = 4
          //Serial.println("cmd = +CMGR");          
          index = buff.indexOf(",");
          String temp = buff.substring(index+1, buff.length()); 
          temp = "AT+CMGR=" + temp + "\r"; 
          //Serial.print("print3.5  ");
          //Serial.println(temp);
          //AT+CMGR=4 i.e. get response stored at memory location 4
          fona.println(temp); 
        }
        else if(cmd == "+CMGR"){
          //Serial.println("cmd = +CMGR");
          extractSms(buff);
          //Serial.println("extractSms Success.");
          if(sender_number == PHONE){
            //Serial.println("doAction");
            doAction();
            fona.println("AT+CMGD=1,4"); 
            updateSerial();
          }
        }

        else if (cmd == "+CMT") {
          //Serial.println("cmd = +CMT");
          extractSms(buff);
          //Serial.println("extractSms Success.");
          if(sender_number == PHONE){
            //Serial.println("doAction");
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
      else if (buff == "OK") {
        myled.Light("green", 1000);
      } 

      else if (buff == "ERROR") {
        myled.Light("red", 1000);
      }

      else {
        myled.Light("orange", 1000);      
      }
    }



    void extractSms(String buff){
      unsigned int index;
      //Serial.println(buff);

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

      //Serial.println("----------------------------------");
      //Serial.println(sms_status);
      //Serial.println(sender_number);
      //Serial.println(received_date);
      //Serial.println(msg);
      //Serial.println("----------------------------------");
    }

    /*---------------------------------------------------------------------------------------------------------------------------------*/

    void doAction() {
      //case sensitive
      //Serial.println("doAction function");
      char responseSMS[1024];
      
      if(msg == "id"){
        char cstr[16];
        dtostrf(id, 5, 0, cstr);
        strcpy(responseSMS, "id: ");
        strcat(responseSMS, cstr);                  
        //Serial.println(responseSMS);
        if(replyStatus == true){
          sendSms(responseSMS);
        }
      }
      
      if(msg == "poloha"){
        if(replyStatus == true){
          dtostrf(mygps.lat, 11, 9, latString);
          dtostrf(mygps.lng, 11, 9, lngString);
          strcpy(responseSMS, "http://maps.google.com/maps?q=loc:");
          strcat(responseSMS, latString);
          strcat(responseSMS, ",");
          strcat(responseSMS, lngString);
          sendSms(responseSMS);
        }
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
      //Serial.println("SMS Sent Successfully.");
    }

    /*---------------------------------------------------------------------------------------------------------------------------------*/

    void updateSerial() {
      delay(100);
      while (Serial.available()) 
      {
        //Serial.print("\n\tprint 2\n");
        fona.write(Serial.read());//Forward what Serial received to Software Serial Port
      }
      while(fona.available()) 
      {
        //Serial.print("\n\tprint 1\n");
        parseresponse(fona.readString()); //Forward what Software Serial received to Serial Port
      }
    }
    
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

};