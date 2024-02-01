//LIBRAIRIES

//BME680 (Température - Humidité - IAQ ?)
//Librairie du constructeur
#include "DFRobot_BME680_I2C.h"
//Librairie standard Arduino pour connection I2C
#include "Wire.h"

//LECTEUR CARTE SD
//Librairie pour la communication avec le lecteur
#include <SD.h>

//------------------------------------------------------//

//Variables globales

//BME
//Objet I2C du capteur
DFRobot_BME680_I2C bme(0x76);  //0x76 I2C address !! (not 0x77)

//SD
//Pin associé au Chip Select du protocole SPI
const int CS = 4;
//Variable de type fichier pour gérer l'écriture dans la carte SD
File myFile;

//MOTION
//Numéro du pin de capteur mouvement
#define pin_motion 0

//MIC
//Pin sur lequel on lit le volume sonore
#define mic A0

//------------------------------------------------------//

//INITIALISATION
void setup()
{
  //BME
  //Variable pour tester l'initialisation du BME
  uint8_t rslt = 1;
  while(rslt != 0) {
    rslt = bme.begin();
    if(rslt != 0) {
      Serial.println("Init BME NON");
      delay(2000);
    }
  }
  Serial.println("Init BME OUI");

  //SD
  //Essaye d'initialiser le SPI avec la carte SD au port CS
  if (!SD.begin(CS)) {
    Serial.print("Init SD NON");
    while (true);
  }
  Serial.println("Init SD OUI");

  //MOTION
  //Initialise le pin de réception 
  pinMode(pin_motion, INPUT);
}

//------------------------------------------------------//

//Variables pour gérer l'exécution périodique des mesures et les stocker

//Variable pour stocker le temps de départ
int dep = millis();
//Variable pour stocker le temps courant depuis le départ
int tps = millis() - dep;
//Dernière update du bme
int bme_last_upd = 0;

//BME
//Température
float temp = 0;
//Humidité
float humid = 0;
//Gas resis
float gas = 0;

//MOTION
//Variable qui dit si quelqu'un est là
bool motion = false;

//MIC
//Variable de la tension brute du micro
int brute = 0;


//------------------------------------------------------//

//BOUCLE

void loop()
{

  /* POUR LE CODE FINAL
  //Récupération des donnés des capteurs

  //Récupération des données du BME
  //Récupère le temps courant
  tps = millis() - dep ;
  //Si cela fait 60 secondes
  if (tps - bme_last_upd > 60000) {
    //Met à jour la dernière lecture
    bme_last_upd = tps;
    //Enregistre les données
    bme.startConvert();
    delay(1000);
    bme.update();
    //Stock les données (température, humidité, gas resistance)
    temp = bme.readTemperature() / 100;
    humid = bme.readHumidity() / 1000;
    gas = bme.readGasResistance();
  }

  //Enregistre quand le capteur de mouvement détecte quelqu'un
  if(isPeopleDetected()) {
      motion = true;
    }

  //Enregistre les volumes sonore
  int brute = analogRead(A0);
  // -- calcul pour le transformer en db A FAIRE --
  */

  //POUR TEST

  //Enregistre les données
  //BME
  bme.startConvert();
  delay(1000);
  bme.update();
  //Stock les données (température, humidité, gas resistance)
  temp = bme.readTemperature() / 100;
  humid = bme.readHumidity() / 1000;
  gas = bme.readGasResistance();
  //MOTION
  if(isPeopleDetected()) {
      motion = true;
    }
  //MIC
  int brute = analogRead(A0);
  
  //------------------------------------------------------//

  /* POUR CODE FINAL
  //Ecrit les données - moyenne ? 
  //Ouvre fichier, en écrasant son contenu précédent (enlever | O_TRUNC pour écrire à la suite)
  myFile = SD.open("data.csv", FILE_WRITE | O_TRUNC);
  //Si pas d'erreur, on écrit dedans au format CSV
  if (myFile) {
    //Ligne d'en-tête des colones
    myFile.println("Temperature, humidité, ...");
    //Ligne de valeur
    myFile.println("15, 0.6, ...");
    //Ferme le fichier
    myFile.close();
  }
  */
  
  //POUR TEST
  

}

//Fonction annexe pour tester la détection de mouvement
int isPeopleDetected()
{
  //On créé une variable booléenne enregistrant l'existence de mouvement
  int motion = digitalRead(pin_motion);
  //Si il y a du mouvement, motion est HIGH -> on return true
  if(motion == HIGH)
  {
      return true;
  }
  else
  {
      return false;/
  }
}





