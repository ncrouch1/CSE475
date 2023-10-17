#include <Arduino_LSM6DSOX.h>

float Ax, Ay, Az;
float Gx, Gy, Gz;

void setup() {
  Serial.begin(115200);

  while(!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
  Serial.println();

  Serial.print("Gyroscope sample rate = ");  
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println("Hz");
  Serial.println();

}

void loop() {

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(Ax, Ay, Az);

    Serial.print(millis());
    Serial.print(',');
    Serial.print(Ax);
    Serial.print(',');
    Serial.print(Ay);
    Serial.print(',');
    Serial.print(Az);
    Serial.print(',');
  } else {
    Serial.println("Failed to read data from Accelerometer");
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(Gx, Gy, Gz);
    Serial.print(Gx);
    Serial.print(',');
    Serial.print(Gy);
    Serial.print(',');
    Serial.print(Gz);
    Serial.println();
  } else {
    Serial.println("Failed to read data from Gyroscope");
  }

delay(50);

}