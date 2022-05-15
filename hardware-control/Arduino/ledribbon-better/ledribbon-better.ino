#include <FastLED.h>

//---valSetup(PLS CHECK val)---

//Array definition (-cos wave divided into 90 discrete amplitudes from 0 to 255)
const byte wave[]={0 , 1 , 2 , 3 , 5 , 8 , 11 , 15 , 20 , 25 , 30 , 36 , 43 , 49 , 56 , 64 , 72 , 80 , 88 , 97 , 105 , 114 , 123 , 132 , 141 , 150 , 158 , 167 , 175 , 183 , 191 , 199 , 206 , 212 , 219 , 225 , 230 , 235 , 240 , 244 , 247 , 250 , 252 , 253 , 254 , 255 , 254 , 253 , 252 , 250 , 247 , 244 , 240 , 235 , 230 , 225 , 219 , 212 , 206 , 199 , 191 , 183 , 175 , 167 , 158 , 150 , 141 , 132 , 123 , 114 , 105 , 97 , 88 , 80 , 72 , 64 , 56 , 49 , 43 , 36 , 30 , 25 , 20 , 15 , 11 , 8 , 5 , 3 , 2 , 1};

//numberOfLEDs(In the future, It will be automized.)
const int numOfL = 32;

//ctrlData OutputPin(DO NOT TOUCH)
const int controlPin = 5;

//Definition of angular velocity
const int anv=7;

//Declaration of current corner position (to be modified)
int nowAngle=0;

//---valSetupEnd---

CRGB leds[numOfL];

void setup() {
  pinMode(13, OUTPUT);
  FastLED.addLeds<WS2812B, controlPin, GRB>(leds, numOfL);
  rainbow_start();
}


void loop() {
  rainbow_onecycle();
}

void rainbow_start(){
  for(int nowL=0;nowL<numOfL;nowL++){
    if(nowAngle<45){
      leds[nowL].r = wave[nowAngle%45 +45];
      leds[nowL].g = wave[nowAngle%45];
      leds[nowL].b = wave[0];
    }else if(nowAngle<90){
      leds[nowL].r = wave[0];
      leds[nowL].g = wave[nowAngle%45 +45];
      leds[nowL].b = wave[nowAngle%45];
    }else{
      leds[nowL].r = wave[nowAngle%45];
      leds[nowL].g = wave[0];
      leds[nowL].b = wave[nowAngle%45 +45];
    }
    nowAngle+=anv;
    if(nowAngle>134)nowAngle%=135;
  }
  FastLED.show();
  return;
}

void rainbow_onecycle(){
  for(int i=numOfL-1;i>0;i--){
    leds[i].r=leds[i-1].r;
    leds[i].g=leds[i-1].g;
    leds[i].b=leds[i-1].b;
  }
  if(nowAngle<45){
    leds[0].r = wave[nowAngle%45 +45];
    leds[0].g = wave[nowAngle%45];
    leds[0].b = wave[0];
  }else if(nowAngle<90){
    leds[0].r = wave[0];
    leds[0].g = wave[nowAngle%45 +45];
    leds[0].b = wave[nowAngle%45];
  }else{
    leds[0].r = wave[nowAngle%45];
    leds[0].g = wave[0];
    leds[0].b = wave[nowAngle%45 +45];
  }
  nowAngle+=anv;
  if(nowAngle>134)nowAngle%=135;
  
  FastLED.show();
  return;
}
