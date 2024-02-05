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

//DEBUG
int LED = 3;

//------------------------------------------------------//

//INITIALISATION
void setup()
{
  //GENERAL
  //Lance la comm port série. Inutile pour le code final.
  Serial.begin(9600);

  //DEBUG
  //Initialise une led pour debugger
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  //BME
  //Variable pour tester l'initialisation du BME
  uint8_t rslt = 1;
  while(rslt != 0) {
    rslt = bme.begin();
    if(rslt != 0) {
      Serial.println("Init BME NON");
      //Clignotte vite = raté
      blinkWrong();
      //Force le passage
      rslt = 0;
    }
  }
  //Clignote lentement = réussi
  blinkRight();
  
  //SD
  //Essaye d'initialiser le SPI avec la carte SD au port CS
  if (!SD.begin(CS)) {
    Serial.print("Init SD NON");
    blinkWrong();
  }
  else {
    blinkRight();
  }

  //MOTION
  //Initialise le pin de réception 
  pinMode(pin_motion, INPUT);
}

//------------------------------------------------------//

//Variables pour gérer l'exécution périodique des mesures
//BME
//Dernière update du bme
float bme_last_upd = 0;
//Temps entre deux mesures du BME
float bme_ecart_mes = 10000;

//MIC
//Dernière update du mic
float mic_last_upd = 0;
//Temps entre deux mesures du mic
float mic_ecart_mes = 5000;
//Début de mesure du son
float mic_debut = 0;

//MOTION
//Dernière mesure de mouvements
float motion_last_upd = 0; 
//On attend au moins 5 secondes entres deux mesures de mouvements
float motion_ecart_min = 5000;

//Variables de stockages
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
//Min et max du voltage pour le micro sur la période de mesure
float min = analogRead(A0);
float max = analogRead(A0);
//Valeur brute lue à chaque instant
float brute = 0;
//Voltage pic à pic
float vcc = 0;
//Variable du volume en dB SPL
float dB = 0.;
//Variable du dBV
float dBV = 0.;


//------------------------------------------------------//

//BOUCLE

void loop()
{
  //Récupération des donnés des capteurs

  //BME
  //Si l'écart entre la dernière mesure et le temps actuel est plus grand que l'écart maximal autorisé entre deux mesures, on refait la mesure et on stock le temps de la dernière mesure
  if (millis() - bme_last_upd > bme_ecart_mes) {
    //Met à jour la dernière lecture
    bme_last_upd = millis();

    //Enregistre les données
    bme.startConvert();
    delay(1000);
    bme.update();
    //Stock les données (température, humidité, gas resistance)
    temp = bme.readTemperature() / 100;
    humid = bme.readHumidity() / 1000;
    gas = bme.readGasResistance();

    //Ecrit la donnée
    //Ouvre fichier
    myFile = SD.open("data.txt", FILE_WRITE);
    if (myFile) {
      //Lignes de valeurs
      myFile.print("Température, humidité, gaz :");
      myFile.print(temp);
      myFile.print(",");
      myFile.print(humid);
      myFile.print(",");
      myFile.print(gas);
      //Temps
      myFile.print(" --- ");
      myFile.println(millis()/1000.);

      //Ferme le fichier
      myFile.close();
    }
  }

  //MOTION
  //Regarde si il y a eu du mouvement
  if(isPeopleDetected() && millis() - motion_last_upd > motion_ecart_min) {
    //On stock la date de la mesure
    motion_last_upd = millis();
    //Ecrit la donnée
    //Ouvre fichier
    myFile = SD.open("data.txt", FILE_WRITE);
    if (myFile) {
      //Lignes de valeurs
      myFile.print("Mouvement :");
      //Temps
      myFile.println(millis()/1000.);

      //Ferme le fichier
      myFile.close();
    }

    blinkQuick();
  }

  //MIC
  //Si l'écart entre la dernière mesure et le temps actuel est plus grand que l'écart maximal autorisé entre deux mesures, on refait la mesure et on stock le temps de la dernière mesure
  if (millis() - mic_last_upd > mic_ecart_mes) {
    //On note la date du dernier enregistrement
    mic_last_upd = millis();
    //On réinitialise max, min et dB
    max = analogRead(A0);
    min = analogRead(A0);
    dB = 0;

    
    //On sauvegarde le temps de début
    float mic_debut = millis();
    //Tant qu'on a pas regardé pendant 50ms
    while(millis() - mic_debut < 50){
      //On lit la valeur actuelle
      brute = analogRead(A0);
      //Si elle est plus grande que le max, c'est le nouveau max
      if (brute > max){
        max = brute;
      }
      //Si elle est plus petite que le min, c'est le nouveau min
      if (brute < min){
        min = brute;
      }
    }

    //On calcul le vcc
    vcc = max - min;
    //On convertit le vcc en RMS
    vcc = vcc/2.828;
    //On convertit le RMS en dBV
    dBV = 20*log(vcc);
    //On calcul le dB SPL
    dB = dBV;

    //Ecrit la donnée
    //Ouvre fichier
    myFile = SD.open("data.txt", FILE_WRITE);
    if (myFile) {
      //Lignes de valeurs
      myFile.print("Son :");
      myFile.print(dB);
      //Temps
      myFile.print(" --- ");
      myFile.println(millis()/1000.);

      //Ferme le fichier
      myFile.close();
    }
  }
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
      return false;
  }
}

//Annexe pour debug
//Fonctions pour debugs
void blinkRight() {
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}

void blinkWrong() {
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
}

void blinkQuick() {
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
}

