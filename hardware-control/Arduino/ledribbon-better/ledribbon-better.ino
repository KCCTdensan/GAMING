#include <FastLED.h>

//---valSetup(PLS CHECK val)---

//Array definition (-cos wave divided into 90 discrete amplitudes from 0 to 255)
const byte wave[90] = {0 , 1 , 2 , 3 , 5 , 8 , 11 , 15 , 20 , 25 , 30 , 36 , 43 , 49 , 56 , 64 , 72 , 80 , 88 , 97 , 105 , 114 , 123 , 132 , 141 , 150 , 158 , 167 , 175 , 183 , 191 , 199 , 206 , 212 , 219 , 225 , 230 , 235 , 240 , 244 , 247 , 250 , 252 , 253 , 254 , 255 , 254 , 253 , 252 , 250 , 247 , 244 , 240 , 235 , 230 , 225 , 219 , 212 , 206 , 199 , 191 , 183 , 175 , 167 , 158 , 150 , 141 , 132 , 123 , 114 , 105 , 97 , 88 , 80 , 72 , 64 , 56 , 49 , 43 , 36 , 30 , 25 , 20 , 15 , 11 , 8 , 5 , 3 , 2 , 1};

//numberOfLEDs(In the future, It will be automized.)
const int numOfL = 144;

//ctrlData OutputPin(DO NOT TOUCH)
const int controlPin = 5;

//Definition of angular velocity
byte anv = 7;

//argument for the delay function of the control because of its speedup.
byte delaySeconds = 50;

//Declaration of current corner position (to be modified)
int nowAngle = 0;

//debugging mode val
//#define DBG

//---valSetupEnd---

CRGB leds[numOfL];

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  FastLED.addLeds<WS2812B, controlPin, GRB>(leds, numOfL);
}
void(* resetFunc)(void) = 0;
void(*brightFunc)(void) = rainbow_onecycle;

void loop() {
  system_startCycle();
}

void system_command() {
  switch (Serial.read()) {
    case 'a':
      sysdeb_disp("a: edit Angular velocity");
      anv = system_compArg();
      break;
    case 'd':
      sysdeb_disp("d: edit 1Cycle delaylMilliSeconds");
      delaySeconds = system_compArg();
      break;
    case 'c':
      sysdeb_disp("c: Changes to the specified light pattern.");
      system_change(system_compArg());
      break;
    case 'D':
      sysdeb_disp("D: Displays all variables");
      system_disp();
      break;
    case 'S':
      system_cutoff();
      while (Serial.available() == 0) {
        Serial.read();
      }
      sysdeb_disp("S: The system operation is now temporarily suspended. To resume, enter S again. In this state, no commands other than S will be accepted.");
      while (Serial.read() != 'S') {}
      sysdeb_disp("S: Resume system operation.\n");
      break;
    case 'R':
      sysdeb_disp("R: System reset.\n");
      Serial.flush();
      resetFunc();
      break;
    default:
      sysdeb_disp("Command not found.\n");
      break;
  }
  system_cleanBuf();

  return;
}

int system_compArg() {
  Serial.read();//Blank space removal
  int i = 0, o = 0;

  while (Serial.peek() != 10 && Serial.peek() != -1) {
    i = Serial.read() - '0';
    o = i + o * 10;;
  }

  sysdeb_disp("Set val: ");
  sysdeb_dispval(o);
  sysdeb_disp("");
  return o;
}

void system_change(byte type) {
  switch (type) {
    case 0:
      brightFunc = rainbow_onecycle;
      sysdeb_disp("c: The light pattern is set to GAMING.");
      break;
    case 1:
      brightFunc = white_onecycle;
      sysdeb_disp("c: The light pattern is set to white.");
      break;
    case 2:
      brightFunc = daylight_onecycle;
      sysdeb_disp("c: The light pattern is set to daylight white.");
      break;
    case 3:
      brightFunc = subdued_onecycle;
      sysdeb_disp("c: The light pattern is set to subdued white.");
      break;
    case 4:
      brightFunc = sepia_onecycle;
      sysdeb_disp("c: The light pattern is set to sepia.");
      break;
    case 5:
      brightFunc = redwave_onecycle;
      sysdeb_disp("c: The light pattern is set to REDWAVE.");
      break;
    default:
      sysdeb_disp("c: Error ! Non-existent specification code.");
  }
  sysdeb_disp("");

  return;
}

void system_disp() {
#ifdef DEB
  sysdeb_disp("Angular velocity val: ");
  sysdeb_dispval(anv);
  sysdeb_disp("1Cycle delayMilliSeconds val: ");
  sysdeb_dispval(delaySeconds);
  sysdeb_disp("All vals are displayed.\n");
#else
  Serial.print(anv);
  Serial.print(" ");
  Serial.print(delaySeconds);
  Serial.println();
#endif
}

void system_cleanBuf() {
  while (Serial.peek() != -1) {
    Serial.read();
  }

  return;
}

void system_cutoff() {
  for (int i = 0; i < numOfL; i++) {
    leds[i].r = 0;
    leds[i].g = 0;
    leds[i].b = 0;
  }
  FastLED.show();
}

void system_shiftColors() {
  for (int i = numOfL - 1; i > 0; i--) {
    leds[i].r = leds[i - 1].r;
    leds[i].g = leds[i - 1].g;
    leds[i].b = leds[i - 1].b;
  }
}

void system_startCycle() {
  if (Serial.available() > 0)system_command();
  brightFunc();
}

void sysdeb_disp(String str) {
#ifdef DEB
  Serial.println(str);
#endif
}

void sysdeb_dispval(int val) {
#ifdef DEB
  Serial.print(val);
#endif
}

void rainbow_onecycle() {
  system_shiftColors();
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

void white_onecycle() {
  system_shiftColors();
  leds[0].r = 255;
  leds[0].g = 255;
  leds[0].b = 255;

  FastLED.show();
  return;
}

void daylight_onecycle() {
  system_shiftColors();
  leds[0].r = 255;
  leds[0].g = 180;
  leds[0].b = 100;

  FastLED.show();
  return;
}

void subdued_onecycle() {
  system_shiftColors();
  leds[0].r = 255;
  leds[0].g = 100;
  leds[0].b = 25;

  FastLED.show();
  return;
}

void sepia_onecycle() {
  system_shiftColors();
  leds[0].r = 255;
  leds[0].g = 50;
  leds[0].b = 5;

  FastLED.show();
  return;
}

void redwave_onecycle() {
  system_shiftColors();
  if (nowAngle > 89)nowAngle %= 90;
  leds[0].r = wave[nowAngle];
  leds[0].g = wave[0];
  leds[0].b = wave[0];
  nowAngle += anv;
  delay(delaySeconds);

  FastLED.show();
  return;
}
