#include <SoftwareSerial.h>
#include "GPS.h"

#define FONA_RX 12 //MISO PIN
#define FONA_TX 13 //MOSI PIN
#define FONA_RST 14

SoftwareSerial fona (FONA_TX, FONA_RX);

//HardwareSerial *fonaSerial = &Serial;

GPS mygps;


class GSM {

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
          
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  public:
  
    char sendto[21] = "+421911250477"; 
    char message[1024];  
    char command[255];  
    int tempForSwitch;   
    uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
    uint8_t type;
    bool messageSent = false;

    /*-----------------------------------------------------------------------------------------------------------------------*/

    void Init() {
      pinMode(15, OUTPUT);
      digitalWrite(15, HIGH);            
      delay(3000);      
      
      fona.begin(4800);
      Serial.println("\ninitializing...");
    
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
      
      fona.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
      updateSerial();
    
    }          
        
    /*---------------------------------------------------------------------------------------------------------------------------------*/     
    
    bool sendMessage() {
      //code for sending message;
      dtostrf(mygps.lat, 11, 9, latString);
      dtostrf(mygps.lng, 11, 9, lngString);
      strcpy(message, "http://maps.google.com/maps?q=loc:");
      strcat(message, latString);
      strcat(message, ",");
      strcat(message, lngString);
            
      // if (fona.sendSMS(sendto, message)) {
      //   myled.Blik(100, "green", 5);
      //   return true;
      // }
      // else {
      //   myled.Blik(100, "red", 5);
      //   return false;
      // }  
    }

    /*---------------------------------------------------------------------------------------------------------------------------------*/    

    void parseMessage(String buff) {
      Serial.print("print3.0  ");
      Serial.println(buff);

      unsigned int len, index;
      //////////////////////////////////////////////////
      //Remove sent "AT Command" from the response string.
      index = buff.indexOf("\r");
      buff.remove(0, index+2);
      buff.trim();
      //////////////////////////////////////////////////
      Serial.print("print3.1  ");
      Serial.println(buff); //Response string
      //////////////////////////////////////////////////
      if(buff != "OK"){
        index = buff.indexOf(":");
        String cmd = buff.substring(0, index);
        Serial.print("print3.2  ");
        Serial.println(cmd);
        cmd.trim();
        Serial.print("print3.3  ");
        Serial.println(cmd);
        
        buff.remove(0, index+2);
        Serial.print("print3.4  ");
        Serial.println(buff);
        Serial.println(cmd);        
        
        if(cmd == "+CMTI"){
          //get newly arrived memory location and store it in temp
          //temp = 4
          Serial.println("cmd = +CMGR");          
          index = buff.indexOf(",");
          String temp = buff.substring(index+1, buff.length()); 
          temp = "AT+CMGR=" + temp + "\r"; 
          Serial.print("print3.5  ");
          Serial.println(temp);
          //AT+CMGR=4 i.e. get message stored at memory location 4
          fona.println(temp); 
        }
        else if(cmd == "+CMGR"){
          Serial.println("cmd = +CMGR");
          extractSms(buff);
          Serial.println("extractSms Success.");
          if(sender_number == PHONE){
            Serial.println("doAction");
            doAction();
            //deleteSms();
          }
        }

        else if (cmd == "+CMT") {
          Serial.println("cmd = +CMT");
          extractSms(buff);
          Serial.println("extractSms Success.");
          
            Serial.println("doAction");
            doAction();
            fona.println("AT+CMGD=1,4"); 
            updateSerial();
          
        }
      //////////////////////////////////////////////////
      }
      else{
      //The result of AT Command is "OK"
      } 

    }


    void extractSms(String buff){
      unsigned int index;
      Serial.println(buff);

      index =buff.indexOf(",");
      sms_status = buff.substring(1, index-1); 
      buff.remove(0, index+2);
      
      sender_number = buff.substring(0, 13);
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
      Serial.println("doAction function");
      if(msg == "id"){
        String printString = "id: " + String(id);                  
        Serial.println(printString);
        if(replyStatus == true){
          sendSms(printString);
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
      fona.print("AT+CMGS=\""+PHONE+"\"\r");
      delay(1000);
      fona.print(text);
      delay(100);
      fona.write(0x1A); //ascii code for ctrl-26 //fona.println((char)26); //ascii code for ctrl-26
      delay(1000);
      Serial.println("SMS Sent Successfully.");
    }

    /*---------------------------------------------------------------------------------------------------------------------------------*/

    void updateSerial() {
      delay(100);
      while (Serial.available()) 
      {
        Serial.print("\n\tprint 2\n");
        fona.write(Serial.read());//Forward what Serial received to Software Serial Port
      }
      while(fona.available()) 
      {
        Serial.print("\n\tprint 1\n");
        parseMessage(fona.readString()); //Forward what Software Serial received to Serial Port
      }
    }

};