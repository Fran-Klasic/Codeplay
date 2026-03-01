#include <Arduino.h>
#include <FastLED.h>

#include "joystick.h"
#include "imu.h"
#include "feedback.h"
#include "led.h"
#include "sleep.h"

#define PINGMS 20
#define DEFAULT_TONE 1000

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);
  pinMode(VIBROMOTOR, OUTPUT);

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  pinMode(BUTTON5, INPUT_PULLUP);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // Initialize inactivity timer and give power-up feedback
  SleepCheck(true);
  userFeedback(true, DEFAULT_TONE);

  initGyro();
}

void loop() {
  // Check for inactivity-based sleep
  SleepCheck(false);

  // Stop feedback after duration expires
  userFeedback(false, 0);

  // Handle incoming game events (sound + vibration)
  checkGameEvents();

  // LED animation
  leds[0] = GetCustomColor(millis());
  FastLED.show();

  // Read inputs and IMU
  updateJoystick();
  updateIMU();

  // Reset sleep timer on user interaction
  if (joystick.buttons || joystick.joy_x || joystick.joy_y) {
    SleepCheck(true);
  }

  // Send joystick packet to game
  Serial.write((uint8_t *)&joystick, sizeof(joystick));

  delay(PINGMS);
}