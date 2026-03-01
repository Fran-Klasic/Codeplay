#pragma once
#include <Arduino.h>

// Hardware pins for user feedback components
#define BUZZER 42
#define VIBROMOTOR 21

// Default feedback parameters
#define BUZZER_FREQ_HZ 1000
#define FEEDBACK_DURATION_MS 100

typedef enum {
  MELODY_NONE = 0,
  MELODY_DEFAULT,
  MELODY_COIN,
  MELODY_HEAL,
  MELODY_KILL,
  MELODY_DAMAGE,
  MELODY_DEATH
} MelodyType;


// Controls buzzer and vibration feedback.
// - firstCall = true -> starts feedback with the given tone frequency
// - firstCall = false -> stops feedback after the predefined duration
void userFeedback(bool start, MelodyType melody);

// Reads game event bytes from Serial and triggers appropriate feedback
void checkGameEvents();
// Updateds effect of led
void updateEffect();