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
int bme_last_upd = 0;
//Temps entre deux mesures du BME
int bme_ecart_mes = 60000;

//MIC
//Dernière update du mic
int mic_last_upd = 0;
//Temps entre deux mesures du mic
int mic_ecart_mes = 30000;
//Temps pendant lequel on enregistre les données sonores
int mic_duree_enr = 5000;
//Variable qui dit si on est en train d'enregistrer
int mic_enr = 0;

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
//Variable de la tension brute du micro
int brute = 0;
//Moyenne en dB sur la dernière période de temps
int dB = 0;


//------------------------------------------------------//

//BOUCLE

void loop()
{

  // POUR LE CODE FINAL

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
    myFile = SD.open("data.csv", FILE_WRITE);
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
  if(isPeopleDetected()) {
      //Ecrit la donnée
      //Ouvre fichier
      myFile = SD.open("data.csv", FILE_WRITE);
      if (myFile) {
        //Lignes de valeurs
        myFile.print("Mouvement :");
        //Temps
        myFile.println(millis()/1000.);

        //Ferme le fichier
        myFile.close();
      }

  //MIC
  //Si l'écart entre la dernière mesure et le temps actuel est plus grand que l'écart maximal autorisé entre deux mesures, on refait la mesure et on stock le temps de la dernière mesure
  if (millis() - mic_last_upd > mic_ecart_mes) {
    //On note la date du dernier enregistrement
    mic_last_upd = millis();
    //On se met à enregistrer
    mic_enr = 1;
  }
  //Si on est train d'enregistrer, on stock les valeurs dans le tableau
  if(mic_enr) {
    //Si on enregsitre depuis moins de temps que le temps réglé par enregistrement
    if(millis() - mic_last_upd < mic_dure_enr){
      //Stock les valeurs dans le tableau. Comment gérer l'écart entre deux valeurs que l'on stock ?

    }
    //Sinon, c'est qu'il faut arrêter d'enregistrer et sortir une valeur
    else {
      //On arrête d'enregistrer
      mic_enr = 0;
      //On moyenne les valeurs pour sortir un dB moyen
      //dB = ...

      //Ecrit la donnée
      //Ouvre fichier
      myFile = SD.open("data.csv", FILE_WRITE);
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
  }// -- calcul pour le transformer en db A FAIRE -- 
   
  
  //POUR TEST
  /*
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


  //DEBUG
  blinkRight();
  */

  //------------------------------------------------------//

  // POUR CODE FINAL
  //Ecrit les données
  //Ouvre fichier, en écrasant son contenu précédent (enlever | O_TRUNC pour écrire à la suite)
  myFile = SD.open("data.csv", FILE_WRITE);
  //Si pas d'erreur, on écrit dedans au format CSV
  if (myFile) {
    //En-tête des colonnes déjà écrites en initialisation

    //Lignes de valeurs
    myFile.print(temp);
    myFile.print(",");
    myFile.print(humid);
    myFile.print(",");
    myFile.print(gaz);
    myFile.print(",");
    myFile.print(motion);
    myFile.print(",");
    myFile.println(dB);//Saut de ligne pour le dernier

    //Ferme le fichier
    myFile.close();
  }
  
  //Remet motion à 0 car on a écrit dessus
  motion = 0;

  //POUR TEST
  /*
  //Ouvre fichier, en écrasant son contenu précédent (enlever | O_TRUNC pour écrire à la suite)
  myFile = SD.open("capteur.txt", FILE_WRITE);
  if (myFile) {
    //Écriture de valeurs
    //BME
    myFile.println("Température : ");
    myFile.println(temp);
    
    myFile.println("Gas : ");
    myFile.println(gas);
    myFile.println("Humidité : ");
    myFile.println(humid);
    //MOTION
    myFile.println("Motion : ");
    myFile.println(motion);
    //MIC
    myFile.println("Son : ");
    myFile.println(brute);

    myFile.println("------");
    
    myFile.close();

    blinkWrong();
  }

  */

  //Attend 1 seconde pour voir
  delay(1000);
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

