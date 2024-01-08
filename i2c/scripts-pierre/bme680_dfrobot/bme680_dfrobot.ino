#include "DFRobot_BME680_I2C.h"
#include "Wire.h"

/*use an accurate altitude to calibrate sea level air pressure*/
#define CALIBRATE_PRESSURE 

DFRobot_BME680_I2C bme(0x76);  //0x76 I2C address !! (not 0x77)

float seaLevel; 
void setup()
{
  uint8_t rslt = 1;
  Serial.begin(9600);
  while(!Serial);
  delay(1000);
  Serial.println();
  while(rslt != 0) {
    rslt = bme.begin();
    if(rslt != 0) {
      Serial.println("bme begin failure");
      delay(2000);
    }
  }
  Serial.println("bme begin successful");
  #ifdef CALIBRATE_PRESSURE
  bme.startConvert();
  delay(1000);
  bme.update();
  /*You can use an accurate altitude to calibrate sea level air pressure. 
   *And then use this calibrated sea level pressure as a reference to obtain the calibrated altitude.
   *In this case,525.0m is chendu accurate altitude.
   */
  seaLevel = bme.readSeaLevel(21.0); //21m est l'altitude de Lille
  Serial.print("seaLevel :");
  Serial.println(seaLevel);
  #endif
}

void loop()
{
  bme.startConvert();
  delay(1000);
  bme.update();
  Serial.println();
  Serial.print("temperature(C) :");
  Serial.println(bme.readTemperature() / 100, 2);
  Serial.print("pressure(Pa) :");
  Serial.println(bme.readPressure());
  Serial.print("humidity(%rh) :");
  Serial.println(bme.readHumidity() / 1000, 2);
  Serial.print("gas resistance(ohm) :");
  Serial.println(bme.readGasResistance());
  Serial.print("altitude(m) :");
  Serial.println(bme.readAltitude());
  #ifdef CALIBRATE_PRESSURE
  Serial.print("calibrated altitude(m) :");
  Serial.println(bme.readCalibratedAltitude(seaLevel));
  #endif
}