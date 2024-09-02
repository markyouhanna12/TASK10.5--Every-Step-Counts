#include <Wire.h>

// MPU6050 I2C address
const int MPU6050_ADDR = 0x68;

// MPU6050 register addresses
const int PWR_MGMT_1 = 0x6B;
const int GYRO_XOUT_H = 0x43;
const int ACCEL_XOUT_H = 0x3B;

// Variables to hold gyro and accelerometer data
int16_t accelX, accelY, accelZ, gyroX, gyroY, gyroZ;
float yaw = 0.0;
unsigned long lastTime;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // Wake up MPU6050
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission();

  lastTime = millis();
}

void loop() {
  readMPU6050();
  
  // Calculate the yaw using gyroscope data
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0; // Convert to seconds
  lastTime = currentTime;

  float gyroZRate = gyroZ / 131.0; // Convert raw data to degrees per second
  yaw += gyroZRate * dt; // Integrate angular velocity to get yaw

  Serial.print("Yaw: ");
  Serial.println(yaw);
  delay(100);
}

void readMPU6050() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 14, true);
  
  // Read accelerometer data
  accelX = (Wire.read() << 8) | Wire.read();
  accelY = (Wire.read() << 8) | Wire.read();
  accelZ = (Wire.read() << 8) | Wire.read();
  
  // Read temperature data (ignore for now)
  Wire.read();
  Wire.read();
  
  // Read gyroscope data
  gyroX = (Wire.read() << 8) | Wire.read();
  gyroY = (Wire.read() << 8) | Wire.read();
  gyroZ = (Wire.read() << 8) | Wire.read();
}
