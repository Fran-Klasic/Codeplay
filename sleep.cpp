#include "sleep.h"
#include "led.h"
#include "driver/rtc_io.h"
#include "esp_sleep.h"

// Stores the timestamp of the last detected user activity.
// Used to determine when the inactivity timeout has been reached.
static unsigned long firstSleepTime = 0;

// Checks whether the device should enter deep sleep mode.
// - firstCall = true  → resets the inactivity timer (user interaction detected)
// - firstCall = false → checks if the inactivity timeout has expired
void SleepCheck(bool firstCall) {
  // Current system uptime in milliseconds
  unsigned long now = millis();

  // Reset inactivity timer on first call or user activity
  if (firstCall) {
    firstSleepTime = now;
  }
  // If no activity occurred for TIME_UNTIL_SLEEP, enter deep sleep
  else if (now - firstSleepTime >= TIME_UNTIL_SLEEP) {
    // Turn off LED to visually indicate sleep mode
    leds[0] = CRGB::Black;
    FastLED.show();

    // Configure Button 4 (GPIO14) as wake-up source (active LOW)
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_14, 0);
    rtc_gpio_pullup_en(GPIO_NUM_14);

    // Enter deep sleep mode
    esp_deep_sleep_start();
  }
}