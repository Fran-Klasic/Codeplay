#include "imu.h"
#include "joystick.h"

// IMU sensor instance (LSM6DS3) using I2C communication at address 0x6B
LSM6DS3 myIMU(I2C_MODE, 0x6B);

// Madgwick filter instance for sensor fusion
Madgwick filter;

// Initializes the IMU sensor and the Madgwick filter.
// The filter update rate is set to 100 Hz.
void initGyro() {
  myIMU.begin();
  filter.begin(100);
}

// Reads accelerometer and gyroscope data, applies sensor fusion,
// and updates joystick tilt values in a normalized range.
void updateIMU() {
  // Read accelerometer data (in g)
  float ax = myIMU.readFloatAccelX();
  float ay = myIMU.readFloatAccelY();
  float az = myIMU.readFloatAccelZ();

  // Read gyroscope data (in degrees per second)
  float gx = myIMU.readFloatGyroX();
  float gy = myIMU.readFloatGyroY();
  float gz = myIMU.readFloatGyroZ();

  // Update Madgwick filter using IMU data
  filter.updateIMU(gx, gy, gz, ax, ay, az);

  // Get orientation angles from the filter
  float roll = filter.getRoll();
  float pitch = filter.getPitch();

  // Convert orientation angles to game-friendly tilt values [-100, 100]
  // X axis is inverted to match in-game orientation
  joystick.tilt_x = constrain((int)(pitch * TILT_SCALE), -100, 100) * -1;
  joystick.tilt_y = constrain((int)(roll * TILT_SCALE), -100, 100);

  // Use Z-axis acceleration to detect vertical movement or impact
  joystick.tilt_z = constrain((int)(az * ACC_Z_SCALE), -100, 100);
}