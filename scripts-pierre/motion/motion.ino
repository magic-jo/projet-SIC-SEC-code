//PIN
//Numéro du pin de capteur mouvement
#define pin_motion 4

void setup()
{
  //Initialise le pin de réception 
  pinMode(pin_motion, INPUT);

  //Initialise la LED intégré pour tests
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}


void loop()
{
  if(isPeopleDetected()) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }

}

int isPeopleDetected()
{
    int motion = digitalRead(pin_motion);
    if(motion == HIGH)//if the sensor value is HIGH?
    {
        return true;//yes,return ture
    }
    else
    {
        return false;//no,return false
    }
}