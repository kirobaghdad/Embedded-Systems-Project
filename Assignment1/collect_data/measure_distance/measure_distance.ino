#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

float accelX, velocityX, positionX; // Variables for X-axis motion
float lastTime, currentTime;        // Time tracking
float dt;                           // Time difference

// Thresholds (adjust these based on your sensor and application)
const float ACCEL_THRESHOLD_POS = 0.2;  // m/s^2 - Ignore accel below this
const float ACCEL_THRESHOLD_NEG = 0.1;  // m/s^2 - Ignore accel below this
const float VEL_THRESHOLD = 0.03;   // m/s - Reset velocity below this

void setup() {
  Serial.begin(9600);
  
  // Initialize I2C and MPU6050
  Wire.begin();
  mpu.initialize();

  // Check if MPU6050 is connected
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
    while (1); // Halt if no connection
  }

  mpu.CalibrateAccel(10); // Calibration Time: generate offsets and calibrate our MPU6050
  mpu.CalibrateGyro(10);


  // Initialize variables
  accelX = 0.0;
  velocityX = 0.0;
  positionX = 0.0;
  lastTime = millis() / 1000.0; // Convert to seconds
}

void loop() {
  // Get current time
  currentTime = millis() / 1000.0; // Convert to seconds
  dt = currentTime - lastTime;     // Time difference in seconds
  lastTime = currentTime;

  // Read raw accelerometer data (in m/s^2)
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Convert raw accel data to m/s^2 (assuming default sensitivity of ±2g)
  accelX = ax / 16384.0 * 9.81; // 16384 LSB/g, 9.81 m/s^2 for gravity

  // Apply acceleration threshold (ignore small values as noise)
  if (accelX > 0 && (accelX) < ACCEL_THRESHOLD_POS) {
    accelX = 0.0; // Treat as no movement
  }
  else if (accelX < 0 && -(accelX) > ACCEL_THRESHOLD_NEG)
    accelX = 0.0;


  // Integrate acceleration to get velocity (v = v0 + a * dt)
  if(accelX)
    velocityX += accelX * dt;
  else
    velocityX = 0;

  // Apply velocity threshold (reset to 0 if too small, assuming stationary)
  if (abs(velocityX) < VEL_THRESHOLD) {
    velocityX = 0.0; // Reset velocity to prevent drift
  }

  // Integrate velocity to get position (s = s0 + v * dt)
  positionX += velocityX * dt;

  // Print results
  Serial.print("Accel_X:");
  Serial.print(accelX);
  Serial.print(",");
  Serial.print("Velocity_X:");
  Serial.print(velocityX);
  Serial.print(",");
  Serial.print("PositionX:");
  Serial.println(positionX);

  // Small delay to avoid overwhelming Serial Monitor
  delay(100);
}
// #include <Wire.h>

// // MPU-6050 I2C address
// const int MPU_ADDR = 0x68;

// // Calibration variables
// int16_t AcX, AcY, AcZ;
// float gForceX, gForceY, gForceZ;
// float accelX_offset = 0, accelY_offset = 0, accelZ_offset = 0;

// // Position estimation variables
// float velocityX = 0, velocityY = 0, velocityZ = 0;
// float positionX = 0, positionY = 0, positionZ = 0;
// unsigned long lastTime;

// void setup() {
//   Serial.begin(9600);
//   Wire.begin();
  
//   // Initialize MPU-6050
//   Wire.beginTransmission(MPU_ADDR);
//   Wire.write(0x6B);  // PWR_MGMT_1 register
//   Wire.write(0);     // Wake up MPU-6050
//   Wire.endTransmission(true);
  
//   // Calibrate the sensor
//   calibrateMPU6050();
//   lastTime = millis();
// }

// void calibrateMPU6050() {
//   const int numSamples = 1000;
//   float sumX = 0, sumY = 0, sumZ = 0;
  
//   Serial.println("Calibrating MPU-6050... Keep sensor still");
  
//   for (int i = 0; i < numSamples; i++) {
//     readAccelerometer();
//     sumX += AcX;
//     sumY += AcY;
//     sumZ += AcZ;
//     delay(2);
//   }
  
//   // Calculate average offset (assuming sensor is level during calibration)
//   accelX_offset = sumX / numSamples;
//   accelY_offset = sumY / numSamples;
//   accelZ_offset = (sumZ / numSamples) - 16384.0; // Subtract 1g (16384 = 1g for ±2g range)
  
//   Serial.println("Calibration complete!");
//   Serial.print("X offset: "); Serial.println(accelX_offset);
//   Serial.print("Y offset: "); Serial.println(accelY_offset);
//   Serial.print("Z offset: "); Serial.println(accelZ_offset);
// }

// void readAccelerometer() {
//   Wire.beginTransmission(MPU_ADDR);
//   Wire.write(0x3B);  // Starting register for accelerometer data
//   Wire.endTransmission(false);
//   Wire.requestFrom(MPU_ADDR, 6, true);
  
//   AcX = Wire.read() << 8 | Wire.read();  // X-axis
//   AcY = Wire.read() << 8 | Wire.read();  // Y-axis
//   AcZ = Wire.read() << 8 | Wire.read();  // Z-axis
// }

// void loop() {
//   // Read raw data
//   readAccelerometer();
  
//   // Apply calibration and convert to g-forces (±2g range, 16384 LSB/g)
//   gForceX = (AcX - accelX_offset) / 16384.0;
//   gForceY = (AcY - accelY_offset) / 16384.0;
//   gForceZ = (AcZ - accelZ_offset) / 16384.0;
  
//   // Calculate time difference
//   unsigned long currentTime = millis();
//   float dt = (currentTime - lastTime) / 1000.0; // Convert to seconds
//   lastTime = currentTime;

//   Serial.print("forceX "); Serial.print(gForceX);
//   Serial.print("forceY "); Serial.print(gForceY);
//   Serial.print("forceZ "); Serial.print(gForceZ);
//   Serial.println("");


//   // Integrate acceleration to get velocity (v = v0 + a*t)
//   if((gForceX) > 0.1)
//     velocityX += gForceX * 9.81 * dt;  // Convert g to m/s²
//   if((gForceY) > 0.1)
//     velocityY += gForceY * 9.81 * dt;
//   if((gForceZ) > 0.1)
//     velocityZ += gForceZ * 9.81 * dt;
  
//   // Integrate velocity to get position (s = s0 + v*t)
//   if(abs(velocityX) > 0.1)
//   positionX += velocityX * dt;
//   if(abs(velocityY) > 0.1)
//   positionY += velocityY * dt;
//   if(abs(velocityZ) > 0.1)
//   positionZ += velocityZ * dt;
  
//   // Print results
//   Serial.print("Position X: "); Serial.print(positionX);
//   Serial.print(" m | Y: "); Serial.print(positionY);
//   Serial.print(" m | Z: "); Serial.print(positionZ);
//   Serial.println(" m");
  
//   delay(100);
// }