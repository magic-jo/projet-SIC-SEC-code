void setup() {
  Serial.begin(9600);
}

void loop() {
  //Calcul de la valeur en dB SPL
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

  //On sauvegarde le temps de début
  float debut = millis();
  //Temps qu'on a pas regardé pendant 50ms
  while(millis() - debut < 50){
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

  //On calcul le VCC
  vcc = max - min;
  //On convertit le vcc en RMS
  vcc = vcc/2.82;
  //On convertit le RMS en dBV
  dBV = 20*log(vcc);
  //On calcul le dB SPL
  dB = dBV;

  //Affichage
  // read the value from the sensor with voltage
  Serial.print(0);
  Serial.print(", ");
  Serial.print(100);
  Serial.print(", ");
  Serial.println(dB);
  

}