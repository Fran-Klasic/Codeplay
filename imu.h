#pragma once
#include <SparkFunLSM6DS3.h>
#include <MadgwickAHRS.h>

// Scaling factors used to convert IMU data into game-friendly ranges
#define TILT_SCALE 3.0f
#define ACC_Z_SCALE 100.0f

// Global IMU sensor and sensor fusion filter instances
extern LSM6DS3 myIMU;
extern Madgwick filter;

// Initializes the IMU sensor and the Madgwick sensor fusion filter
void initGyro();

// Updates IMU data and converts orientation into joystick tilt values
void updateIMU();