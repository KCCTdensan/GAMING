#include <FastLED.h>

//---valSetup(PLS CHECK val)---

//ledNumber(In the future, It will be automized.)
const int numberOfLEDs = 32;

//ctrlDatPin(DO NOT TOUCH)
const int controlPin = 5;

//attachInterruptPinVal
const int atPinI = 0, atPinII = 1; //atPinI=pin2,atPinII=pin3

//rainbowThetaVal
const unsigned int angDelta = 5, ledDelta = 2;

//modeVal
int blinkMode = 0, modeNum = 2;

//thetaVal(info: This val is "FLOAT TYPE".)
float theta = 0, sutheta = 0;

//waveform array
const int seprate = 270;
unsigned char wave[seprate][3];

//---valSetupEnd---

CRGB leds[numberOfLEDs];

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, controlPin, GRB>(leds, numberOfLEDs);
  genwav(seprate);
}


void loop() {
  rainbow();
}

void rainbow() {
  int ang = 0, bfang;
  while (1) {
    digitalWrite(13, 1);
    for (int thisLED = 0; thisLED < numberOfLEDs; thisLED++) {
      bfang = ang + (thisLED * ledDelta);
      if (bfang >= seprate)bfang %= seprate;
      /*
        bfang%=ang+(ledDelta*thisLED);
        Serial.println(ang);
        Serial.println(bfang);
        Serial.println((int)wave[ang][0]);
        Serial.println((int)wave[ang][1]);
        Serial.println((int)wave[ang][2]);
        Serial.println("------------------------");
      */
      leds[thisLED].r = wave[bfang][0];
      leds[thisLED].g = wave[bfang][1];
      leds[thisLED].b = wave[bfang][2];
      FastLED.show();
    }
    digitalWrite(13, 0);
    ang += angDelta;
    if (ang >= seprate) {
      ang = 0;
    }
  }
}

void genwav(int n) {
  float deltasep = 540 / n, angper;
  for (int i = 0; i < n; i++) {
    angper =(float) i / n;
    Serial.println(angper);
    if (angper < 0.16) {
      wave[i][0] = (char)128 + 127 * sin(2 * PI * (((float)deltasep * i) / 360.0));
      wave[i][1] = 0;
      wave[i][2] = (char)128 - 127 * sin(2 * PI * (((float)deltasep * i) / 360.0));
      Serial.println("OH");
    } else if ((angper >= 0.16) && (angper < 0.5)) {
      wave[i][0] = (char)128 + 127 * sin(2 * PI * (((float)deltasep * i) / 360.0));
      wave[i][1] = (char)128 - 127 * sin(2 * PI * (((float)deltasep * i) / 360.0));
      wave[i][2] = 0;
      Serial.println("SAY");
    } else if ((angper >= 0.5) && (angper < 0.94)) {
      wave[i][0] = 0;
      wave[i][1] = (char)128 - 127 * sin(2 * PI * (((float)deltasep * i) / 360.0));
      wave[i][2] = (char)128 + 127 * sin(2 * PI * (((float)deltasep * i) / 360.0));
      Serial.println("CAN");
    } else {
      wave[i][0] = (char)128 - 127 * sin(2 * PI * (((float)deltasep * i) / 360.0));
      wave[i][1] = 0;
      wave[i][2] = (char)128 + 127 * sin(2 * PI * (((float)deltasep * i) / 360.0));
      Serial.println("YOU");
    }
  }
  Serial.println("SEE");
}
