#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN         12
#define STAIR            10                          // Ile schodow
#define LEDS_PER_STAIR   20                          // Ile diod na jeden schodek
#define NUM_LEDS         STAIR * LEDS_PER_STAIR
CRGB leds[NUM_LEDS];

// Set up Variables
unsigned long timeOut = 60000;   // timestamp to remember when the PIR was triggered.
int downUp = 0;                  // variable to rememer the direction of travel up or down the stairs
int alarmPinTop = 10;            // PIR at the top of the stairs
int alarmPinBottom = 11;         // PIR at the bottom of the stairs
int alarmValueTop = LOW;         // Variable to hold the PIR status
int alarmValueBottom = LOW;      // Variable to hold the PIR status
int ledPin = 13;                 // LED on the arduino board flashes when PIR activated

// Zdefiniowane kolory
const CRGB::HTMLColorCode colors[90] = {CRGB::Amethyst, CRGB::Aqua, CRGB::Blue, CRGB::BlueViolet, CRGB::Brown, CRGB::Brown, CRGB::Brown, CRGB::Chartreuse, CRGB::Chocolate,
                                        CRGB::Coral, CRGB::CornflowerBlue, CRGB::Crimson, CRGB::Cyan, CRGB::DarkBlue, CRGB::DarkCyan, CRGB::DarkGoldenrod, CRGB::DarkGray,
                                        CRGB::DarkGreen, CRGB::DarkMagenta, CRGB::DarkKhaki, CRGB::DarkOliveGreen, CRGB::DarkOrange, CRGB::DarkOrchid, CRGB::DarkRed,
                                        CRGB::DarkSalmon, CRGB::DarkSlateBlue, CRGB::DarkSlateGray, CRGB::DarkTurquoise, CRGB::DarkViolet, CRGB::DeepPink, CRGB::DeepSkyBlue,
                                        CRGB::DimGray, CRGB::DodgerBlue, CRGB::FireBrick, CRGB::ForestGreen, CRGB::Fuchsia, CRGB::Gold, CRGB::Goldenrod, CRGB::Green, CRGB::YellowGreen,
                                        CRGB::GreenYellow, CRGB::HotPink, CRGB::IndianRed, CRGB::Indigo, CRGB::Khaki, CRGB::LawnGreen, CRGB::LightSeaGreen, CRGB::LightSkyBlue,
                                        CRGB::LightSlateGray, CRGB::Lime, CRGB::LimeGreen, CRGB::Magenta, CRGB::Maroon, CRGB::MediumAquamarine, CRGB::MediumBlue, CRGB::MediumOrchid,
                                        CRGB::MediumPurple, CRGB::MediumSeaGreen, CRGB::MediumSlateBlue, CRGB::MediumSpringGreen, CRGB::MediumTurquoise, CRGB::MediumVioletRed,
                                        CRGB::MidnightBlue, CRGB::MintCream, CRGB::NavajoWhite, CRGB::Navy, CRGB::Olive, CRGB::OliveDrab, CRGB::Orange, CRGB::OrangeRed, CRGB::Orchid,
                                        CRGB::PaleVioletRed, CRGB::PaleGreen, CRGB::Plaid, CRGB::Purple, CRGB::Red, CRGB::RoyalBlue, CRGB::SeaGreen, CRGB::SandyBrown, CRGB::SlateBlue,
                                        CRGB::SkyBlue, CRGB::Snow, CRGB::SpringGreen, CRGB::SteelBlue, CRGB::Teal, CRGB::Tomato, CRGB::Turquoise, CRGB::Violet, CRGB::White, CRGB::Yellow
                                       };


void setup() {
  Serial.begin (9600);                                                                        // only requred for debugging
  pinMode(ledPin, OUTPUT);                                                                    // initilise the onboard pin 13 LED as an indicator
  pinMode(alarmPinTop, INPUT_PULLUP);                                                         // for PIR at top of stairs initialise the input pin and use the internal restistor
  pinMode(alarmPinBottom, INPUT_PULLUP);                                                      // for PIR at bottom of stairs initialise the input pin and use the internal restistor
  delay (2000);                                                                               // it takes the sensor 2 seconds to scan the area around it before it can
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(16);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  alarmValueTop = digitalRead(alarmPinTop);                         // Constantly poll the PIR at the top of the stairs
  alarmValueBottom = digitalRead(alarmPinBottom);                   // Constantly poll the PIR at the bottom of the stairs

  if (alarmValueTop == HIGH && downUp != 2)  {                      // the 2nd term allows timeOut to be contantly reset if one lingers at the top of the stairs before decending but will not allow the bottom PIR to reset timeOut as you decend past it.
    timeOut = millis();                                             // Timestamp when the PIR is triggered.  The LED cycle wil then start.
    downUp = 1;
    topdown(random8(0, 6));                                         // lights up the strip from top down
  }

  if (alarmValueBottom == HIGH && downUp != 1)  {                   // the 2nd term allows timeOut to be contantly reset if one lingers at the bottom of the stairs before decending but will not allow the top PIR to reset timeOut as you decend past it.
    timeOut = millis();                                             // Timestamp when the PIR is triggered.  The LED cycle wil then start.
    downUp = 2;
    topdown(random8(0, 6));                                         // lights up the strip from top down
  }

  if (timeOut + 10000 < millis() && timeOut + 15000 < millis()) {    //switch off LED's in the direction of travel.
    if (downUp == 1) {
      ledoff(random8(0, 4));
    }
    if (downUp == 2)  {
      ledoff(random8(0, 4));
    }
    downUp = 0;
  }
}

void topdown(int mode) {
  switch (mode) {
    case 0:
      colourWipeDownUp(colors[random8(0, 90)], 3, 1);
      break;
    case 1:
      colourWipeDownUp(colors[random8(0, 90)], 3, 0);
      break;
    case 2:
      GreenRed(200);
      break;
    case 3:
      efekt(50, colors[random8(0, 90)]);
      break;
    case 4:
      efekt1(50, colors[random8(0, 90)]);
      break;
    case 5:
      efekt2(50, colors[random8(0, 90)], 1);
      break;
  }
  for (int i = 0; i < 3; i++) {                   // Helpful debug indication flashes led on Arduino board twice
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
}

void ledoff(int mode) {
  Serial.println(mode);
  switch (mode) {
    case 0:
      all_off(500);
      break;
    case 1:
      efekt(50, CRGB::Black);
      break;
    case 2:
      efekt1(50, CRGB::Black);
      break;
    case 3:
      efekt2(50, CRGB::Black, 0);
      break;
  }
  for (int i = 0; i < 3; i++) {                  // Helpful debug indication flashes led on Arduino board twice
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
}

void all_off(int wait) {
  if (downUp != 2)
    for (int j = 0; j < STAIR; j++) {
      int start = NUM_LEDS / STAIR * j;
      for (int i = start; i < start + LEDS_PER_STAIR; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      FastLED.delay(wait);
    }
  if (downUp != 1)
    for (int j = STAIR; j > 0; j--) {
      int start = NUM_LEDS / STAIR * j;
      for (int i = start; i > start - LEDS_PER_STAIR; i--) {
        leds[i - 1] = CRGB::Black;
      }
      FastLED.show();
      FastLED.delay(wait);
    }
}

// Fade light each step strip
void colourWipeDownUp(uint32_t color, int wait, int r) {
  if (downUp != 2)
    for (int j = 0; j < STAIR; j++) {
      if (r == 1) {
        color = colors[random8(0, 90)];
      }
      int start = NUM_LEDS / STAIR * j;
      for (int k = 0; k <= 50; k++) {
        for (int i = start; i < start + LEDS_PER_STAIR; i++) {
          leds[i] =  blend(CRGB::Black, color, k * (255 / 50));
        }
        FastLED.show();
        FastLED.delay(wait);
      }
    }
  if (downUp != 1)
    for (int j = STAIR; j > 0; j--) {
      if (r == 1) {
        color = colors[random8(0, 90)];
      }
      int start = NUM_LEDS / STAIR * j;
      for (int k = 0; k <= 50; k++) {
        for (int i = start; i > start - LEDS_PER_STAIR; i--) {
          leds[i - 1] =  blend(CRGB::Black, color, k * (150 / 50));
        }
        FastLED.show();
        FastLED.delay(wait);
      }
    }
}

void GreenRed (uint16_t wait) {
  if (downUp != 2) {
    for (int j = 0; j < STAIR; j++) {
      int start = NUM_LEDS / STAIR * j;
      for (int i = start; i < start + LEDS_PER_STAIR; i++) {
        leds[i] =  CRGB::Green;
      }
      FastLED.show();
      FastLED.delay(wait);
    }
    for (int j = 0; j < STAIR; j++) {
      int start = NUM_LEDS / (LEDS_PER_STAIR / 2) * j;
      for (int i = start; i < start + (LEDS_PER_STAIR / 2); i++) {
        leds[i + 5] =  CRGB::Red;
      }
      FastLED.show();
      FastLED.delay(80);
    }
  }
  if (downUp != 1) {
    for (int j = STAIR; j > 0; j--) {
      int start = NUM_LEDS / STAIR * j;
      for (int i = start; i > start - LEDS_PER_STAIR; i--) {
        leds[i - 1] =  CRGB::Green;
      }
      FastLED.show();
      FastLED.delay(wait);
    }
    for (int j = STAIR; j > 0; j--) {
      int start = NUM_LEDS / (LEDS_PER_STAIR / 2) * j;
      for (int i = start; i > start - (LEDS_PER_STAIR / 2); i--) {
        leds[i - 5] =  CRGB::Red;
      }
      FastLED.show();
      FastLED.delay(80);
    }
  }
}

void efekt (uint16_t wait, uint32_t color) {
  if (downUp != 2)
    for (int j = 0; j < STAIR / 2; j++) {
      int start =  LEDS_PER_STAIR * j;
      for (int i = 0; i < LEDS_PER_STAIR / 2; i++) {
        leds[(start * 2) + i] = color;
        leds[((start * 2) + LEDS_PER_STAIR) - i - 1] = color;
        FastLED.delay(wait);
        FastLED.show();
      }
      for (int i = LEDS_PER_STAIR / 2; i > 0; i--) {
        leds[LEDS_PER_STAIR + (start * 2) + i - 1] = color;
        leds[(LEDS_PER_STAIR + (start * 2) + LEDS_PER_STAIR) - i] = color;
        FastLED.delay(wait);
        FastLED.show();
      }
    }
  if (downUp != 1)
    for (int j = STAIR / 2; j > 0; j--) {
      int start =  LEDS_PER_STAIR * j;
      for (int i = 0; i < LEDS_PER_STAIR / 2; i++) {
        leds[(start * 2) - i - 1] = color;
        leds[((start * 2) - LEDS_PER_STAIR) + i] = color;
        FastLED.delay(wait);
        FastLED.show();
      }
      for (int i = LEDS_PER_STAIR / 2; i > 0; i--) {
        leds[start * 2 - LEDS_PER_STAIR  -  i] = color;
        leds[start * 2 - LEDS_PER_STAIR * 2 + i - 1] = color;
        FastLED.delay(wait);
        FastLED.show();
      }
    }
}

void efekt1 (uint16_t wait, uint32_t color) {
  //  if (downUp != 2)
  for (uint16_t j = 0; j < STAIR; j++) {
    uint16_t start =  LEDS_PER_STAIR * j;
    for (uint16_t i = 0; i < LEDS_PER_STAIR / 2; i++) {
      leds[start + i] = color;
      leds[(start + LEDS_PER_STAIR) - i - 1] = color;
      FastLED.delay(wait);
      FastLED.show();
    }
  }
  //  if (downUp != 1)
  for (uint16_t j = STAIR; j > 0; j--) {
    uint16_t start =  LEDS_PER_STAIR * j;
    for (uint16_t i = 0; i < LEDS_PER_STAIR / 2; i++) {
      leds[start - i - 1] = color;
      leds[(start - LEDS_PER_STAIR) + i] = color;
      FastLED.delay(wait);
      FastLED.show();
    }
  }
}

void efekt2 (uint16_t wait, uint32_t color, int white) {
  if (downUp != 2)
    for (uint16_t j = 0; j < STAIR; j++) {
      uint16_t start =  LEDS_PER_STAIR * j;
      for (uint16_t i = 0; i < LEDS_PER_STAIR / 2; i++) {
        leds[start + i] = color;
        leds[(start + LEDS_PER_STAIR) - i - 1] = color;
        FastLED.delay(wait);
        FastLED.show();
      }
    }
  if (downUp != 1)
    for (uint16_t j = STAIR; j > 0; j--) {
      uint16_t start =  LEDS_PER_STAIR * j;
      for (uint16_t i = 0; i < LEDS_PER_STAIR / 2; i++) {
        leds[start - i - 1] = color;
        leds[(start - LEDS_PER_STAIR) + i] = color;
        FastLED.delay(wait);
        FastLED.show();
      }
    }
  if (white == 1) {
    for (int k = 0; k <= 255; k++) {
      for (int j = 0; j < NUM_LEDS; j++) {
        leds[j] = blend(color, CRGB::White, k);
      }
      FastLED.delay(5);
    }
    FastLED.show();
  }
}