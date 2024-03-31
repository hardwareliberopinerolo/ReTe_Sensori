/*-----Questa parte è eseguita solo una volta all'accensione-----------------------------------*/
void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(FONA_RST, OUTPUT);
  //pinMode(sensor, INPUT);

  while (!Serial); //Attende che la porta seriale sia aperta
  Serial.begin(9600);
  digitalWrite(greenLED, HIGH);
  delay (2000);
  Serial.println(F("Initializzazione SIM800L...."));

  SIM800ss.begin(9600); // if you're using software serial
  if (! SIM800.begin(SIM800ss)) {
    Serial.println(F("SIM800L non trovata.."));
    Serial.println(F("Reset tra 5 minuti")); //Fa un reset se non trova la SIM
    digitalWrite(redLED, HIGH);
    delay (300000);  //300000 = 5 minuti
    digitalWrite(redLED, LOW);
    Serial.println(F("Riavvia"));
    delay (2000);
    Riavvia(); //Riavvia tutto
  }

  Serial.println(F("SIM800L is OK"));
  digitalWrite(redLED, LOW);
  delay(1000);
  SIM800ss.print("AT+CFUN=0\r\n"); //Predispone la SIM per la funzionalità minima (AT*CFUN=0) per consumare meno energia
  delay(8000);

  myTimer.startTimer(); //start del timer all'accensione del dispositiv
  digitalWrite(greenLED, LOW);

  Serial.print(F("Tempo invio dati: "));
  Serial.println(tempoInvioDati);






}
