//Lecture / écriture dans la carte microSD

#include <SD.h>

const int CS = 4;
const int LED = 3;

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

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  //ECRITURE

  //Ouvre fichier
  myFile = SD.open("test_pcb.txt", FILE_WRITE);

  //Si pas d'erreur, on écrit dedans
  if (myFile) {
    Serial.println("ça marche !!!");
    //Ferme le fichier
    myFile.close();
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
  }
  //Si erreur d'ouverture
  else {
    // if the file didn't open, print an error:
    Serial.println("Erreur ouverture fichier");
    digitalWrite(LED, HIGH);
    delay(5000);
    digitalWrite(LED, LOW);
  }
  
}

void loop() {
  // nothing happens after setup
}
