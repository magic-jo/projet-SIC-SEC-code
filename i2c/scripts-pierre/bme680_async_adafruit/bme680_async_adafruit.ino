//Libraries pour le capteur et la communication I2C
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

//Constante de référence
#define SEALEVELPRESSURE_HPA (1013.25)

//Variable identifiant le capteur pour la communication I2C
Adafruit_BME680 bme; // I2C

void setup() {
  //Taux de communication
  Serial.begin(9600);
  //Attend que la communication avec l'ordi soit établie au travers du port série pour commencer le programme
  //Ainsi, on ne perd pas d'infos 
  while (!Serial);
  Serial.println(F("BME680 async test"));

  //On essaye d'initialiser la communication avec bme.begin(). Si cela échoue, on arrête le programme
  if (!bme.begin(0x76)) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }

  // Paramètres de filtre et de oversampling du capteur - à changer si besoin
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop() {
  //Commence les mesures
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Failed to begin reading :("));
    return;
  }

  //On garde en mémoire le temps qu'on a mis à faire les mesures
  Serial.print(F("Reading started at "));
  Serial.print(millis());
  Serial.print(F(" and will finish at "));
  Serial.println(endTime);

  Serial.println(F("You can do other work during BME680 measurement."));
  delay(50); // This represents parallel work.
  // There's no need to delay() until millis() >= endTime: bme.endReading()
  // takes care of that. It's okay for parallel work to take longer than
  // BME680's measurement time.

  //Récupère la mesure si on a réussi à mesurer
  if (!bme.endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return;
  }
  Serial.print(F("Reading completed at "));
  Serial.println(millis());

  Serial.print(F("Temperature = "));
  Serial.print(bme.temperature);
  Serial.println(F(" *C"));

  Serial.print(F("Pressure = "));
  Serial.print(bme.pressure / 100.0);
  Serial.println(F(" hPa"));

  Serial.print(F("Humidity = "));
  Serial.print(bme.humidity);
  Serial.println(F(" %"));

  Serial.print(F("Gas = "));
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(F(" KOhms"));

  Serial.print(F("Approx. Altitude = "));
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(F(" m"));

  Serial.println();
  delay(2000);
}