#include <SD.h>
#include <Arduino_LPS22HB.h>
#include "Arduino_BMI270_BMM150.h"
#include <SPI.h>

//#include <Arduino_LSM9DS1.h>


File myFile;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!BARO.begin()) {
    Serial.println("Pressure Sensor Broken");
    while(1);
  }

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  
  Serial.print("Initializing SD card...");
  if(!SD.begin(10)){
    Serial.println("initialization failed");
    while(1);
  }
  Serial.println("initialization done.");

  myFile=SD.open("hiletsgo.txt", FILE_WRITE);
  if(myFile){
    Serial.print("Writing to the test.txt successfully");
    myFile.println("hello world");
    myFile.close();
    Serial.println("done");
  }
  else{
    Serial.println("error opening test.txt");
  }


}

unsigned long previousTime = 0;
unsigned long currentTime = 0;
float previousAltitude = 0;
float currentAltitude = 0;
float velocity = 0;
float altitudeDiff = 0;
unsigned long previousprintTime = 0;

void loop() {
  currentTime = millis();
  float x, y, z;
  //dataFile = SD.open("sensor.txt", FILE_WRITE);
  float current_press = BARO.readPressure();
  // Serial.println(current_press);
  float approx_alt1 = (44330 * (1 - pow(current_press / 101.325, 1 / 5.255))) + 38;
  // Serial.println(approx_alt1);
  // if (dataFile) {
  //Serial.println("approx_alt:");
  //Serial.println(approx_alt1);
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    //Serial.println(F(timestamp));
    // dataFile.close();
  } 
  currentAltitude = approx_alt1;
  if (currentTime - previousTime >= 1000) {
    altitudeDiff = currentAltitude - previousAltitude;
    velocity = altitudeDiff / (currentTime - previousTime);
    Serial.println(velocity);
    previousAltitude = currentAltitude;
    previousTime = currentTime;
  }
  if (currentTime - previousprintTime >= 1000) {
    Serial.print("Velocity per second = ");
    Serial.println(velocity);
    Serial.println("acceleration x-axis");
    Serial.println(x);
    Serial.println("acceleration y-axis");
    Serial.println(y);
    Serial.println("acceleration z-axis");
    Serial.println(z);
    previousprintTime = currentTime; // Reset the print timer
  }

  myFile=SD.open("hiletsgo.txt", FILE_WRITE);
  if(myFile){
    myFile.println("Time:");
    myFile.println(currentTime);
    myFile.println("Pressure:");
    myFile.println(current_press);
    myFile.println("approx_alt1");
    myFile.println(approx_alt1);
    myFile.print("Velocity per second = ");
    myFile.println(velocity);
    myFile.println("acceleration x-axis");
    myFile.println(x);
    myFile.println("acceleration y-axis");
    myFile.println(y);
    myFile.println("acceleration z-axis");
    myFile.println(z);
    myFile.close();
  }
  else{
    Serial.println("error opening test.txt");
  }

}


