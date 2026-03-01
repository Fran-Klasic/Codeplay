#include "feedback.h"

// Stores the timestamp of the last feedback activation.
// Used to stop sound and vibration after a fixed duration.
static unsigned long firstCallTime = 0;

// Controls buzzer and vibration motor feedback.
// - firstCall = true  → starts feedback with selected tone
// - firstCall = false → stops feedback after FEEDBACK_DURATION_MS
void userFeedback(bool firstCall, int toneFrequency) {
  unsigned long now = millis();

  if (firstCall) {
    // Start sound and vibration
    tone(BUZZER, toneFrequency);
    digitalWrite(VIBROMOTOR, HIGH);
    firstCallTime = now;
  } else if (now - firstCallTime >= FEEDBACK_DURATION_MS) {
    // Stop sound and vibration
    noTone(BUZZER);
    digitalWrite(VIBROMOTOR, LOW);
  }
}

// Checks for incoming game events and triggers different tones
// depending on the received event type.
void checkGameEvents() {
  if (!Serial.available())
    return;

  uint8_t event = Serial.read();

  switch (event) {
    case 0x01:  // Coin collected
      userFeedback(true, 1500);
      break;

    case 0x02:  // Heal received
      userFeedback(true, 1200);
      break;

    case 0x03:  // Enemy killed
      userFeedback(true, 1800);
      break;

    case 0x04:  // Damage received
      userFeedback(true, 800);
      break;

    case 0x05:  // Player death
      userFeedback(true, 400);
      break;

    default:
      // Ignore unknown events
      break;
  }
}