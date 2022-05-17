#include <FastLED.h>

//---valSetup(PLS CHECK val)---

//Array definition (-cos wave divided into 90 discrete amplitudes from 0 to 255)
const byte wave[] = {0 , 1 , 2 , 3 , 5 , 8 , 11 , 15 , 20 , 25 , 30 , 36 , 43 , 49 , 56 , 64 , 72 , 80 , 88 , 97 , 105 , 114 , 123 , 132 , 141 , 150 , 158 , 167 , 175 , 183 , 191 , 199 , 206 , 212 , 219 , 225 , 230 , 235 , 240 , 244 , 247 , 250 , 252 , 253 , 254 , 255 , 254 , 253 , 252 , 250 , 247 , 244 , 240 , 235 , 230 , 225 , 219 , 212 , 206 , 199 , 191 , 183 , 175 , 167 , 158 , 150 , 141 , 132 , 123 , 114 , 105 , 97 , 88 , 80 , 72 , 64 , 56 , 49 , 43 , 36 , 30 , 25 , 20 , 15 , 11 , 8 , 5 , 3 , 2 , 1};

//numberOfLEDs(In the future, It will be automized.)
const int numOfL = 32;

//ctrlData OutputPin(DO NOT TOUCH)
const int controlPin = 5;

//Definition of angular velocity
byte anv = 7;

//argument for the delay function of the control because of its speedup.
byte delaySeconds = 50;

//Declaration of current corner position (to be modified)
int nowAngle = 0;

//---valSetupEnd---

CRGB leds[numOfL];

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  FastLED.addLeds<WS2812B, controlPin, GRB>(leds, numOfL);
  rainbow_start();
}
void(* resetFunc)(void) = 0;

void loop() {
  rainbow_cycle();
}

void command() {
  switch (Serial.read()) {
    case 't':
      Serial.println("t: edit Angular velocity");
      anv = compArg();
      break;
    case 'd':
      Serial.println("d: edit 1Cycle delaylMilliSeconds");
      delaySeconds = compArg();
      break;
    case 'D':
      Serial.println("D: Displays all variables");
      system_disp();
      break;
    case 'R':
      Serial.println("R: Reset board.\n");
      delay(2);
      resetFunc();
      break;
    default:
      Serial.println("Command not found.\n");
      break;
  }
  return;
}

int compArg() {
  Serial.read();//Blank space removal
  int i = 0, o = 0;

  while (Serial.peek() != 10 && Serial.peek() != -1) {
    i = Serial.read() - '0';
    o = i + o * 10;;
  }
  Serial.read();//null code removal

  Serial.print("Set val: ");
  Serial.println(o);
  Serial.println();
  return o;
}

void system_disp(){
  Serial.print("Angular velocity val: ");
  Serial.println(anv);
  Serial.print("1Cycle delayMilliSeconds val: ");
  Serial.println(delaySeconds);
  Serial.println("All vals are displayed.\n");
}

void rainbow_cycle() {
  if (Serial.available() > 0)command();
  rainbow_onecycle();
}

void rainbow_start() {
  for (int nowL = 0; nowL < numOfL; nowL++) {
    if (nowAngle < 45) {
      leds[nowL].r = wave[nowAngle % 45 + 45];
      leds[nowL].g = wave[nowAngle % 45];
      leds[nowL].b = wave[0];
    } else if (nowAngle < 90) {
      leds[nowL].r = wave[0];
      leds[nowL].g = wave[nowAngle % 45 + 45];
      leds[nowL].b = wave[nowAngle % 45];
    } else {
      leds[nowL].r = wave[nowAngle % 45];
      leds[nowL].g = wave[0];
      leds[nowL].b = wave[nowAngle % 45 + 45];
    }
    nowAngle += anv;
    if (nowAngle > 134)nowAngle %= 135;
  }
  FastLED.show();
  return;
}

void rainbow_onecycle() {
  for (int i = numOfL - 1; i > 0; i--) {
    leds[i].r = leds[i - 1].r;
    leds[i].g = leds[i - 1].g;
    leds[i].b = leds[i - 1].b;
  }
  if (nowAngle < 45) {
    leds[0].r = wave[nowAngle % 45 + 45];
    leds[0].g = wave[nowAngle % 45];
    leds[0].b = wave[0];
  } else if (nowAngle < 90) {
    leds[0].r = wave[0];
    leds[0].g = wave[nowAngle % 45 + 45];
    leds[0].b = wave[nowAngle % 45];
  } else {
    leds[0].r = wave[nowAngle % 45];
    leds[0].g = wave[0];
    leds[0].b = wave[nowAngle % 45 + 45];
  }
  nowAngle += anv;
  if (nowAngle > 134)nowAngle %= 135;
  delay(delaySeconds);

  FastLED.show();
  return;
}
