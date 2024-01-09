void setup() {
  Serial.begin(9600);
}

void loop() {
  /*
  // read the value from the sensor with voltage
  Serial.print(250);
  Serial.print(", ");
  Serial.print(350);
  Serial.print(", ");
  Serial.println(analogRead(A0));

  delay(10);
  */

  //Read the value in dB
  //Valeur brute
  int brute = analogRead(A0);
  //Valeur en tension
  float voltage;
  voltage = (brute*5)/1024.;
  //Valeur en dB
  float dba = 20.*log10( (voltage/14.) / 0.00002);

  //Affichage sur moniteur graphique
  Serial.print(0);
  Serial.print(", ");
  Serial.print(84);
  Serial.print(", ");
  Serial.println(dba);

  delay(1);

}