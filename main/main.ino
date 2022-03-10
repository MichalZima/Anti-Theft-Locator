
#include "GSM.h"

GSM mysim;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() { 
  // Serial.begin(115200);

  mygps.Startup();
  mysim.Startup();

  myled.Init();
  
  mygps.Init(); 
  mysim.Init();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() { 
  mysim.updateSerial();

  mygps.smartDelay(1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



