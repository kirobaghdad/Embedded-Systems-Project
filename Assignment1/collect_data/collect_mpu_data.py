# Configure serial port - change 'COM3' to your Arduino's port
# On Linux/Mac, it might be something like '/dev/ttyUSB0'

import serial
import csv
import time
serial_port = 'COM10'
baud_rate = 115200

# File to save the data
output_file = 'mpu6050_data.csv'



def collect_data(duration_seconds=60):
    # Open serial connection
    try:
        ser = serial.Serial(serial_port, baud_rate, timeout=1)
        print(f"Connected to {serial_port}")
        
        # Give some time for the connection to establish
        time.sleep(2)
        
        # Open CSV file for writing
        with open(output_file, 'w', newline='') as csvfile:
            csvwriter = csv.writer(csvfile)
            
            # Write header
            header = ['timestamp', 'yaw','pitch','roll','ax','ay','az']
            csvwriter.writerow(header)
            

            # Clear any initial buffer
            ser.flushInput()
            
            line = ser.readline().decode('utf-8').strip()

            while(not line.startswith("Start")):
                line = ser.readline().decode('utf-8').strip()

        
            # print(f"Wait for 15 Seconds for the MPU to settle before collecting data")
            # time.sleep(15)
            
            print(f"Collecting data for {duration_seconds} seconds...")
            
            ser.flushInput()
            
            # Record start time
            start_time = time.time()
            
            while (time.time() - start_time) < duration_seconds:


                try:
                    # Read line from serial
                    line = ser.readline().decode('utf-8').strip()
                    
                    print(line)
                    # Skip empty lines or header
                    if line and not line.startswith('yaw'):
                        # Split the CSV data
                        data = line.split(' ')
                        if len(data) == 6:  # Ensure we have all 6 values
                            # Add timestamp
                            timestamp = time.time() - start_time
                            row = [timestamp] + data
                            csvwriter.writerow(row)
                            
                except Exception as e:
                    print(f"Error reading data: {e}")
                    continue
                    
        print(f"Data saved to {output_file}")
        
    except serial.SerialException as e:
        print(f"Serial connection error: {e}")
        
    finally:
        if 'ser' in locals():
            ser.close()
            print("Serial connection closed")

if __name__ == "__main__":
    # Collect data for 60 seconds (you can modify this)
    collect_data(duration_seconds=10)