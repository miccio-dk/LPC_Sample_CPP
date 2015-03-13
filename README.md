# LPC Sample code (C++)
Read from MPU6050 Gyro+Accelerometer and MA3-P12 Magnetic shaft encoder using an LPC11C24 board.
Separate classes for each device. Encoder read continuously through SysTick interrupts.

## Folder description
- **`LPC_Sample_CPP/`** LPCXpresso code
- **`MA3P12/`** Arduino code simulating the sensor (PWM freq = 250Hz, res = 1us)
- **`mpu6050_test/`** Arduino code for testing the gyro-accelerometer 
