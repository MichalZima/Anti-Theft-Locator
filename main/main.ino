#include "GSM.h"
#include "values.h"

GSM mysim;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() { 
  Serial.begin(115200);
  myled.Init();
  //mygps.Init(); 
  mysim.Init();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  mysim.updateSerial();

  // mygps.smartDelay(1);
  // if (mygps.signalIndex == 3) {
  //   if (mygps.hasLocation) {
  //     mysim.sendMessage();
  //     delay(120000);         
  //   }    
  // }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


