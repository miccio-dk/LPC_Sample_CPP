#include <Wire.h>

// default address 
const int addr = 0x68;
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;

void setup() {
	Wire.begin();

	// sets PWR_MGMT_1 register to 0 (no-sleep mode)
	Wire.beginTransmission(addr);
	Wire.write(0x6B);
	Wire.write(0);
	Wire.endTransmission(true);

	Serial.begin(9600);
	Serial.println("X\tY\tZ\t\tX\tY\tZ");
}

void loop() {
	// reads from register 0x3B (ACCEL_XOUT_H)
	Wire.beginTransmission(addr);
	Wire.write(0x3B);
	Wire.endTransmission(false);
	Wire.requestFrom(addr,14);


	AcX = (Wire.read() << 8 | Wire.read()) / 1671;  // 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
	AcY = (Wire.read() << 8 | Wire.read()) / 1671;  // 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
	AcZ = (Wire.read() << 8 | Wire.read()) / 1671;  // 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)

	// skip temperature registers
	Wire.read();
	Wire.read();

	GyX = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
	GyY = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
	GyZ = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

	Serial.print(AcX);
	Serial.print("\t");
	Serial.print(AcY);
	Serial.print("\t");
	Serial.print(AcZ);
	Serial.print("\t\t");
	Serial.print(GyX);
	Serial.print("\t");
	Serial.print(GyY);
	Serial.print("\t");
	Serial.println(GyZ);

	delay(50);
}