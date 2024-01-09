//Lecture / écriture dans la carte microSD

#include <SD.h>

const int CS = 10;

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  //Essaye de initialiser le SPI avec la carte SD au port CS
  if (!SD.begin(CS)) {
    Serial.print("Init échouée");
    while (true);
  }
  Serial.println("Init réussie");

  //ECRITURE

  //Ouvre fichier
  myFile = SD.open("test.txt", FILE_WRITE);

  //Si pas d'erreur, on écrit dedans
  if (myFile) {
    Serial.println("Ouverture fichier");
    myFile.println("Mot 1. Mot 2.");
    //Ferme le fichier
    myFile.close();
  }
  //Si erreur d'ouverture
  else {
    // if the file didn't open, print an error:
    Serial.println("Erreur ouverture fichier");
  }
  
  //LECTURE

  //Ouvre fichier
  myFile = SD.open("test.txt");

  //Si pas d'erreur
  if (myFile) {
    Serial.println("test.txt:");

    //lit tant qu'il y a des données
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    //Ferme fichier
    myFile.close();
  }
  //Ouverture échouée
  else {
    Serial.println("Erreur ouverture");
  }
}

void loop() {
  // nothing happens after setup
}
