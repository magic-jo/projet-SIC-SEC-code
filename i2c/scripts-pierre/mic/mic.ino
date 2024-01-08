void setup() {
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  Serial.print(250);
  Serial.print(", ");
  Serial.print(350);
  Serial.print(", ");
  Serial.println(analogRead(A0));

  delay(10);
}