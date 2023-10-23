
#define DEBUG true  //set to true for debug output, false for no debug output
#define DEBUG_SERIAL if(DEBUG)Serial

#define CHARACTERISTIC_SIZE 40 // size of the characteristic value in bytes


#include <ArduinoBLE.h>
#include <Arduino_LSM6DSOX.h>

float Ax, Ay, Az;
float Gx, Gy, Gz;

BLEService FASTService("19ec3fd3-cf5f-4f24-9f1a-fb806db2591a"); // BLE LED Service, 1840 is the Generic Health Sensor service uuid.

BLEByteCharacteristic FallCharacteristic("5f9bd2ed-9c52-4b45-9434-951e2d7a9b21", BLERead | BLENotify | BLEWrite);

void setup() {
  DEBUG_SERIAL.begin(115200);
  #if DEBUG == true
    while(!Serial);
  #endif  //DEBUG == true
  if (!IMU.begin()) {
    DEBUG_SERIAL.println("Failed to initialize IMU!");
    while (1);
  }
  DEBUG_SERIAL.print("Accelerometer sample rate = ");
  DEBUG_SERIAL.print(IMU.accelerationSampleRate());
  DEBUG_SERIAL.println("Hz");
  DEBUG_SERIAL.println();

  DEBUG_SERIAL.print("Gyroscope sample rate = ");  
  DEBUG_SERIAL.print(IMU.gyroscopeSampleRate());
  DEBUG_SERIAL.println("Hz");
  DEBUG_SERIAL.println();

  // begin initialization
  if (!BLE.begin()) {
    DEBUG_SERIAL.println("starting Bluetooth® Low Energy failed!");
  }
  // set advertised local name and service UUID:
  BLE.setLocalName("FAST Device");
  BLE.setAdvertisedService(FASTService);
  // add the characteristic to the service
  FASTService.addCharacteristic(FallCharacteristic);
  // add service
  BLE.addService(FASTService);
  // start advertising
  BLE.advertise();
  DEBUG_SERIAL.println("BLE FAST Peripheral, waiting for connections....");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();
  // if a central is connected to peripheral:
  if (central) {
    DEBUG_SERIAL.print("Connected to central: ");
    // print the central's MAC address:
    DEBUG_SERIAL.println(central.address());
    // while the central is still connected to peripheral:
    while (central.connected()) {
      uint8_t packet[CHARACTERISTIC_SIZE];     // initialize packet data array

      // int buttonState = digitalRead(buttonPin);
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(Ax, Ay, Az);
        unsigned long time_stamp = millis();
        ((unsigned long *) packet) [0] = time_stamp;
        ((float *) packet) [1] = Ax;
        ((float *) packet) [2] = Ay;
        ((float *) packet) [3] = Az;

      } else {
        DEBUG_SERIAL.println("Failed to read data from Accelerometer");
      }

      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(Gx, Gy, Gz);
        ((float *) packet) [4] = Gx;
        ((float *) packet) [5] = Gy;
        ((float *) packet) [6] = Gz;
      } else {
        DEBUG_SERIAL.println("Failed to read data from Gyroscope");
      }

      FallCharacteristic.writeValue(*packet); // write value to characteristic

      DEBUG_SERIAL.println(*((unsigned long *)packet));
      DEBUG_SERIAL.println(*((float *)packet + 1));
      DEBUG_SERIAL.println(*((float *)packet + 2));
      DEBUG_SERIAL.println(*((float *)packet + 3));
      DEBUG_SERIAL.println(*((float *)packet + 4));
      DEBUG_SERIAL.println(*((float *)packet + 5));
      DEBUG_SERIAL.println(*((float *)packet + 6));
      DEBUG_SERIAL.println();

      delay(50);
      
    }

    DEBUG_SERIAL.print(F("Disconnected from central: "));
    DEBUG_SERIAL.println(central.address());
  }
}
