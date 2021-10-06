//ESPboy Encoder
//for www.espboy.com project
//by RomanS

#include <Ticker.h>

#include "lib/ESPboyInit.h"
#include "lib/ESPboyInit.cpp"


//encoder1 pins
#define ENC_A 15
#define ENC_B 14
#define ENC_BUT 11

//encoder2 pins
#define ENC_AA 13
#define ENC_BB 12
#define ENC_BUTT 10

#define ENCODER_READ_FREQ_HZ 200
#define REFRESH_RATE 10

volatile int16_t param1 = 0, param2 = 0;

ESPboyInit myESPboy;

Ticker timer1;
void checkEncoders();

void checkEncoders(){
  static uint32_t readingMCP = 0;
  static uint32_t previousReadingMCP = 0;
  
  static uint8_t previousState1 = 0;
  static uint8_t previousState2 = 0;
  
  static uint8_t currentState;

  readingMCP = myESPboy.mcp.readGPIOAB();

  if(previousReadingMCP == readingMCP)
    return;

  previousReadingMCP = readingMCP;
  
  if (!bitRead(readingMCP, ENC_BUT) || !bitRead(readingMCP, ENC_BUTT)) {
     myESPboy.playTone(200,200); 
     return;
   }

   currentState = 0;

   if (bitRead(readingMCP, ENC_A)) bitSet(currentState, 0);
   if (bitRead(readingMCP, ENC_B)) bitSet(currentState, 1);

   if (currentState == 0b11){ 
    if (previousState1 == 0b10) param1++;
    else 
      if (previousState1 == 0b01) param1--; 
   }
 
   previousState1 = currentState;


   currentState = 0;

   if (bitRead(readingMCP, ENC_AA)) bitSet(currentState, 0);
   if (bitRead(readingMCP, ENC_BB)) bitSet(currentState, 1);

   if (currentState == 0b11){ 
    if (previousState2 == 0b10) param2++;
    else if (previousState2 == 0b01) param2--;
   }
 
  previousState2 = currentState;
}




void setup() {
  Serial.begin(115200);
  
  myESPboy.begin("Encoder"); //Init ESPboy
  
  for (uint8_t i=10; i<16; i++){
    myESPboy.mcp.pinMode(i, INPUT);
    myESPboy.mcp.pullUp(i, HIGH);
  }
    
  timer1.attach(1.0/ENCODER_READ_FREQ_HZ, checkEncoders);
  myESPboy.tft.setTextSize(4);
  myESPboy.tft.setTextColor(TFT_GREEN);
}



void loop(){
  static int16_t prevParam1=10, prevParam2;
  static uint32_t tmr=0;

  delay(1);
  if(millis() - tmr > 1000 / REFRESH_RATE){
    tmr = millis();
    if (prevParam1 != param1 || prevParam2 != param2){
       myESPboy.playTone(20,20); 
       prevParam1 = param1;
       prevParam2 = param2;
       myESPboy.tft.fillScreen(TFT_BLACK);
       myESPboy.tft.setCursor(0,40);
       myESPboy.tft.println(param1);
       myESPboy.tft.println(param2);
    } 
  }
}
