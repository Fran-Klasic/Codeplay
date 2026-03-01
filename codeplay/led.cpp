#include "led.h"
#include <Arduino.h>

// LED buffer used by FastLED
CRGB leds[NUM_LEDS];

// Color change periods for each RGB channel (in milliseconds).
// Different periods are used to avoid repeating color patterns.
#define RED_PERIOD_MS 1000.0f
#define GREEN_PERIOD_MS 2000.0f
#define BLUE_PERIOD_MS 3000.0f

// Generates a smoothly changing RGB color based on the current time.
// Each color channel is driven by a sine wave with a different period,
// resulting in a continuous and non-repeating color transition.
CRGB GetCustomColor(unsigned long time) {
  // Convert time to angular position [0, 2*PI] for each color channel
  float rA = (time % (unsigned long)RED_PERIOD_MS) * TWO_PI / RED_PERIOD_MS;
  float gA = (time % (unsigned long)GREEN_PERIOD_MS) * TWO_PI / GREEN_PERIOD_MS;
  float bA = (time % (unsigned long)BLUE_PERIOD_MS) * TWO_PI / BLUE_PERIOD_MS;

  // Map sine output from [-1, 1] to [0, 255] for LED brightness
  uint8_t r = (sin(rA) * 0.5f + 0.5f) * 255;
  uint8_t g = (sin(gA) * 0.5f + 0.5f) * 255;
  uint8_t b = (sin(bA) * 0.5f + 0.5f) * 255;

  // Return the calculated RGB color
  return CRGB(r, g, b);
}