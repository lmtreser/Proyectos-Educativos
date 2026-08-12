// NeoPixel simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 8

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  pixels.begin();
  pixels.clear();
  pixels.setBrightness(255);
}

void loop() {

  uint32_t color = pixels.Color(255, 255, 255);
  pixels.fill(color, 0);
  //pixels.setPixelColor(2, 255, 255, 255);
  pixels.show();
}
