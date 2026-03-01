#pragma once
#include <FastLED.h>

// LED hardware configuration
#define LED_PIN 2
#define NUM_LEDS 1
#define BRIGHTNESS 100
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

// Generates a smoothly changing RGB color based on the given time value
CRGB GetCustomColor(unsigned long time);

// Global LED buffer used by FastLED
extern CRGB leds[NUM_LEDS];