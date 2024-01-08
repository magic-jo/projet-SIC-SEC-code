// source : https://docs.arduino.cc/learn/communication/wire#:~:text=%7D-,I2C%20BMP280,-Qwiic%20BMP280%20module

#include <Wire.h>
#include <Adafruit_BMP280.h>

//Create an instance of the BMP280 sensor
Adafruit_BMP280 bmp; 

void setup() {
  Serial.begin(9600); 
  
  // Start the sensor, and verify that it was found
  if (!bmp.begin()) {
    Serial.println("Sensor not found");
    while (1){} 
  }

}

void loop() {
  // Read the values
  float temperature = bmp.readTemperature();

  // Print to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.println();
  delay(2000);
}