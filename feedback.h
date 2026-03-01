#pragma once
#include <Arduino.h>

// Hardware pins for user feedback components
#define BUZZER 42
#define VIBROMOTOR 21

// Default feedback parameters
#define BUZZER_FREQ_HZ 1000
#define FEEDBACK_DURATION_MS 100

// Controls buzzer and vibration feedback.
// - firstCall = true -> starts feedback with the given tone frequency
// - firstCall = false -> stops feedback after the predefined duration
void userFeedback(bool firstCall, int toneFrequency);

// Reads game event bytes from Serial and triggers appropriate feedback
void checkGameEvents();