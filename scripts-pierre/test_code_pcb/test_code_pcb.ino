//Lecture / écriture dans la carte microSD

#include <SD.h>

const int CS = 4;

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
  myFile = SD.open("test_pcb.csv", FILE_WRITE | O_TRUNC);

  //Si pas d'erreur, on écrit dedans
  if (myFile) {
    Serial.println("ça marche !!!");
    //Ferme le fichier
    myFile.close();
  }
  //Si erreur d'ouverture
  else {
    // if the file didn't open, print an error:
    Serial.println("Erreur ouverture fichier");
  }
  
}

void loop() {
  // nothing happens after setup
}
