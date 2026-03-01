#pragma once
#include <Arduino.h>

// Timeout duration (in milliseconds) after which the device
// enters deep sleep if no user activity is detected
#define TIME_UNTIL_SLEEP 30000

// Checks inactivity time and manages automatic transition to deep sleep.
// - firstCall = true -> resets the inactivity timer
// - firstCall = false -> evaluates whether sleep conditions are met
void SleepCheck(bool firstCall);