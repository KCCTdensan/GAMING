#include <FastLED.h>

//---valSetup(PLS CHECK val)---

//numberOfLEDs(In the future, It will be automized.)
const int numberOfLEDs = 32;

//ctrlData OutputPin(DO NOT TOUCH)
const int controlPin = 5;

//

//---valSetupEnd---

CRGB leds[numberOfLEDs];

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, controlPin, GRB>(leds, numberOfLEDs);
  writewav();
  rainbow_start();
}


void loop() {
  rainbow_onecycle();
}

void rainbow_start(){
  
}

void writewav(){
  if(EEPROM.read(45)==255)return;
  byte wave[]={0 , 1 , 2 , 3 , 5 , 8 , 11 , 15 , 20 , 25 , 30 , 36 , 43 , 49 , 56 , 64 , 72 , 80 , 88 , 97 , 105 , 114 , 123 , 132 , 141 , 150 , 158 , 167 , 175 , 183 , 191 , 199 , 206 , 212 , 219 , 225 , 230 , 235 , 240 , 244 , 247 , 250 , 252 , 253 , 254 , 255 , 254 , 253 , 252 , 250 , 247 , 244 , 240 , 235 , 230 , 225 , 219 , 212 , 206 , 199 , 191 , 183 , 175 , 167 , 158 , 150 , 141 , 132 , 123 , 114 , 105 , 97 , 88 , 80 , 72 , 64 , 56 , 49 , 43 , 36 , 30 , 25 , 20 , 15 , 11 , 8 , 5 , 3 , 2 , 1};
  for(int i=0;i<90;i++){
    EEPROM.write(i,wave[i]);
  }
  for(int i=80;i<EEPROM.length();i++){
    EEPROM.write(i,0);
  }
  return;
}
