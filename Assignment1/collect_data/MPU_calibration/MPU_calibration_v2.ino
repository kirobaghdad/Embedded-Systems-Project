#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

// Six Position Weighted Least Square Method

// Arrays to store gyroscope readings for 6 positions
float gyro_readings[6][3]; // [position][axis] for X, Y, Z
float biases[3];           // b_x, b_y, b_z
float scale_factors[3];    // S_xx, S_yy, S_zz

// Number of samples to average per position
const int NUM_SAMPLES = 1000;

// Function to collect gyroscope data for a given position
void collectData(int position)
{
    Serial.print("Collecting data for position ");
    Serial.println(position + 1);
    Serial.println("Place the MPU-6050 in the specified orientation and wait...");

    float sum[3] = {0, 0, 0}; // Sum of X, Y, Z readings

    for (int i = 0; i < NUM_SAMPLES; i++)
    {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        sum[0] += g.gyro.x; // X-axis
        sum[1] += g.gyro.y; // Y-axis
        sum[2] += g.gyro.z; // Z-axis

        delay(10); // Small delay between readings
    }

    // Average the readings
    gyro_readings[position][0] = sum[0] / NUM_SAMPLES;
    gyro_readings[position][1] = sum[1] / NUM_SAMPLES;
    gyro_readings[position][2] = sum[2] / NUM_SAMPLES;

    Serial.print("Position ");
    Serial.print(position + 1);
    Serial.print(": X = ");
    Serial.print(gyro_readings[position][0], 4);
    Serial.print(", Y = ");
    Serial.print(gyro_readings[position][1], 4);
    Serial.print(", Z = ");
    Serial.println(gyro_readings[position][2], 4);
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(10); // Wait for Serial Monitor

    // Initialize MPU-6050
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
            delay(10);
    }
    Serial.println("MPU6050 Found!");

    // Set MPU-6050 settings
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    // Step 1: Collect data in 6 positions
    Serial.println("Starting calibration. Follow the instructions for each position.");
    Serial.println("Position 1: +X up (X-axis pointing up)");
    Serial.println("Press any key to start...");
    while (Serial.available() == 0)
        ;          // Wait for user input
    Serial.read(); // Clear the input
    collectData(0);

    Serial.println("Position 2: -X up (X-axis pointing down)");
    Serial.println("Press any key to start...");
    while (Serial.available() == 0)
        ;
    Serial.read();
    collectData(1);

    Serial.println("Position 3: +Y up (Y-axis pointing up)");
    Serial.println("Press any key to start...");
    while (Serial.available() == 0)
        ;
    Serial.read();
    collectData(2);

    Serial.println("Position 4: -Y up (Y-axis pointing down)");
    Serial.println("Press any key to start...");
    while (Serial.available() == 0)
        ;
    Serial.read();
    collectData(3);

    Serial.println("Position 5: +Z up (Z-axis pointing up)");
    Serial.println("Press any key to start...");
    while (Serial.available() == 0)
        ;
    Serial.read();
    collectData(4);

    Serial.println("Position 6: -Z up (Z-axis pointing down)");
    Serial.println("Press any key to start...");
    while (Serial.available() == 0)
        ;
    Serial.read();
    collectData(5);

    // Step 2: Compute calibration parameters (biases and scale factors)
    // From equation (9), we compute the averages for each axis pair
    float A_x = (gyro_readings[0][0] + gyro_readings[1][0]) / 2; // X-axis (+X, -X)
    float A_y = (gyro_readings[2][1] + gyro_readings[3][1]) / 2; // Y-axis (+Y, -Y)
    float A_z = (gyro_readings[4][2] + gyro_readings[5][2]) / 2; // Z-axis (+Z, -Z)

    // Biases (b_x, b_y, b_z)
    biases[0] = A_x;
    biases[1] = A_y;
    biases[2] = A_z;

    // Scale factors (S_xx, S_yy, S_zz)
    // We need to know the true angular velocity (ω_true). For static positions, ω_true = 0.
    // However, the method assumes you know ω_true for non-static cases. Here, we assume static (ω_true = 0).
    // From equation (10), S = (measured - bias) / ω_true, but since ω_true = 0, we simplify:
    // Instead, we compute relative scale factors by comparing the differences
    float S_xx = (gyro_readings[0][0] - gyro_readings[1][0]) / 2; // Difference in X-axis readings
    float S_yy = (gyro_readings[2][1] - gyro_readings[3][1]) / 2; // Difference in Y-axis readings
    float S_zz = (gyro_readings[4][2] - gyro_readings[5][2]) / 2; // Difference in Z-axis readings

    // Normalize scale factors (assuming the ideal scale factor is 1)
    scale_factors[0] = 1.0 / S_xx;
    scale_factors[1] = 1.0 / S_yy;
    scale_factors[2] = 1.0 / S_zz;

    Serial.println("Calibration complete!");
    Serial.print("Biases (b_x, b_y, b_z): ");
    Serial.print(biases[0], 4);
    Serial.print(", ");
    Serial.print(biases[1], 4);
    Serial.print(", ");
    Serial.println(biases[2], 4);

    Serial.print("Scale Factors (S_xx, S_yy, S_zz): ");
    Serial.print(scale_factors[0], 4);
    Serial.print(", ");
    Serial.print(scale_factors[1], 4);
    Serial.print(", ");
    Serial.println(scale_factors[2], 4);
}

void loop()
{
    // Step 3: Apply calibration to real-time readings
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Corrected gyroscope readings
    float gyro_x = (g.gyro.x - biases[0]) * scale_factors[0];
    float gyro_y = (g.gyro.y - biases[1]) * scale_factors[1];
    float gyro_z = (g.gyro.z - biases[2]) * scale_factors[2];

    // Print corrected readings
    Serial.print("Corrected Gyro (rad/s): X = ");
    Serial.print(gyro_x, 4);
    Serial.print(", Y = ");
    Serial.print(gyro_y, 4);
    Serial.print(", Z = ");
    Serial.println(gyro_z, 4);

    delay(100);
}