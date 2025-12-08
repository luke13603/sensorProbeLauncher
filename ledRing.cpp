#include <Adafruit_NeoPixel.h>

#define LED_PIN    3
#define LED_COUNT  12

Adafruit_NeoPixel ring(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setupRing() {
  ring.begin();
  ring.setBrightness(255); 
  ring.show();
}

void loadingFill(uint8_t wait) {
  // Fill LEDs one-by-one
  for (int i = 0; i < LED_COUNT; i++) {
    ring.setPixelColor(i, ring.Color(255, 0, 0));  // red
    ring.show();
    delay(wait);
  }

  // All lit → clear instantly
  ring.clear();
  ring.show();
  delay(wait);
}

void loopRing() {loadingFill(80);}

void setRingGreen() {
  for (int i = 0; i < LED_COUNT; i++) {
    ring.setPixelColor(i, ring.Color(0, 255, 0)); // green
  }
  ring.show();
}

void setRingBlue() {
  for (int i = 0; i < LED_COUNT; i++) {
    ring.setPixelColor(i, ring.Color(0, 0, 255)); // BLUE
  }
  ring.show();  // update instantly
}


void blueStrobe(uint16_t onTime, uint16_t offTime) {
  // Turn all LEDs BLUE
  for (int i = 0; i < LED_COUNT; i++) {
    ring.setPixelColor(i, ring.Color(0, 0, 255));
  }
  ring.show();
  delay(onTime);

  // Turn all LEDs OFF
  ring.clear();
  ring.show();
  delay(offTime);
}

void policeLights(uint16_t flashTime, uint16_t pauseTime) {
  // --- BLUE DOUBLE FLASH ---
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < LED_COUNT; i++) {
      ring.setPixelColor(i, ring.Color(0, 0, 255)); // BLUE
    }
    ring.show();
    delay(flashTime);

    ring.clear();
    ring.show();
    delay(flashTime);
  }

  delay(pauseTime); // Pause between colors

  // --- RED DOUBLE FLASH ---
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < LED_COUNT; i++) {
      ring.setPixelColor(i, ring.Color(255, 0, 0)); // RED
    }
    ring.show();
    delay(flashTime);

    ring.clear();
    ring.show();
    delay(flashTime);
  }

  delay(pauseTime); // Pause before looping
}

void rainbow(uint8_t wait) {
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < LED_COUNT; i++) {
      int pixelHue = firstPixelHue + (i * 65536L / LED_COUNT);
      ring.setPixelColor(i, ring.gamma32(ring.ColorHSV(pixelHue)));
    }
    ring.show();
    delay(wait);
  }
}

void resetRing() { ring.clear(); ring.show();}
