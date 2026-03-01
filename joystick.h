#pragma once
#include <Arduino.h>

// Joystick analog input pins
#define JOY_PIN_X 11
#define JOY_PIN_Y 12

// ADC configuration and value mapping
// Raw ADC values are converted to a normalized range [-100, 100]
#define JOY_RAW_MIN 0
#define JOY_RAW_MAX 4095
#define JOY_OUTPUT_MIN -100
#define JOY_OUTPUT_MAX 100
#define JOY_DEADZONE 50

// Button input pins (active LOW)
#define BUTTON1 41
#define BUTTON2 39
#define BUTTON3 40
#define BUTTON4 14
#define BUTTON5 10

// Data packet sent to the game over Serial communication.
// Contains button states, joystick axes, and IMU tilt values.
typedef struct
{
  int16_t buttons;
  int16_t joy_x;
  int16_t joy_y;
  int16_t tilt_x;
  int16_t tilt_y;
  int16_t tilt_z;
} joystick_packet_t;

// Global joystick packet instance
extern joystick_packet_t joystick;

// Reads and packs button states into a bitfield
int buttonClicks();

// Reads and normalizes the joystick X-axis value
int getJoystickX();

// Reads and normalizes the joystick Y-axis value
int getJoystickY();

// Updates the joystick packet with the latest input values
void updateJoystick();