#include "joystick.h"

// Global joystick data packet shared across the project.
// Holds button states, joystick axes, and IMU tilt values.
joystick_packet_t joystick = { 0 };

// Reads the state of all buttons and packs them into a single bitfield.
// Each button occupies one bit to allow efficient transmission.
int buttonClicks() {
  // Buttons are active LOW, so values are inverted
  int b1 = !digitalRead(BUTTON1);
  int b2 = !digitalRead(BUTTON2);
  int b3 = !digitalRead(BUTTON3);
  int b4 = !digitalRead(BUTTON4);
  int b5 = !digitalRead(BUTTON5);

  // Pack individual button states into a single integer
  int buttons = 0;
  buttons |= b1 << 0;
  buttons |= b2 << 1;
  buttons |= b3 << 2;
  buttons |= b4 << 3;
  buttons |= b5 << 4;

  return buttons;
}

// Reads the X-axis of the joystick and converts it to a normalized range.
// A deadzone is applied to prevent noise when the joystick is near center.
int getJoystickX() {
  // Read raw ADC value from joystick X axis
  int raw = analogRead(JOY_PIN_X);

  // Map raw ADC range to game-friendly range [-100, 100]
  int val = map(raw, JOY_RAW_MIN, JOY_RAW_MAX,
                JOY_OUTPUT_MIN, JOY_OUTPUT_MAX);

  // Apply deadzone to filter small unwanted movements
  if (abs(val) <= JOY_DEADZONE)
    val = 0;

  return val;
}

// Reads the Y-axis of the joystick and converts it to a normalized range.
// A deadzone is applied to improve stability around the center position.
int getJoystickY() {
  // Read raw ADC value from joystick Y axis
  int raw = analogRead(JOY_PIN_Y);

  // Map raw ADC range to game-friendly range [-100, 100]
  int val = map(raw, JOY_RAW_MIN, JOY_RAW_MAX,
                JOY_OUTPUT_MIN, JOY_OUTPUT_MAX);

  // Apply deadzone to filter small unwanted movements
  if (abs(val) <= JOY_DEADZONE)
    val = 0;

  return val;
}

// Updates the joystick data packet with the latest button and axis values.
// This function is called once per loop iteration.
void updateJoystick() {
  joystick.buttons = buttonClicks();
  joystick.joy_x = getJoystickX();
  joystick.joy_y = getJoystickY();
}