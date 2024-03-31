/*-----Questa parte è eseguita solo una volta all'accensione-----------------------------------*/
void setup() {
  //pinMode(greenLED, OUTPUT);
  //pinMode(redLED, OUTPUT);
  //pinMode(FONA_RST, OUTPUT);
  //pinMode(sensor, INPUT);

  //while (!Serial); //Attende che la porta seriale sia aperta
  Serial.begin(9600);
  //digitalWrite(greenLED, HIGH);
  delay (2000);
  Serial.println(F("Initializzazione SIM800L...."));



    // Configure MySerial0 on pins TX=D6 and RX=D7 (-1, -1 means use the default)
    MySerial0.begin(9600, SERIAL_8N1, FONA_RX,  FONA_TX);
    //MySerial0.print("MySerial0");



if (!MySerial0) { // If the object did not initialize, then its configuration is invalid
  Serial.println("Invalid HWSerial pin configuration, check config"); 
  while (1) { // Don't continue with invalid configuration
    delay (1000);
  }
}



  
  if (! SIM800.begin(MySerial0)) {
    Serial.println(F("SIM800L non trovata.."));
    Serial.println(F("Reset tra 5 minuti")); //Fa un reset se non trova la SIM
    //digitalWrite(redLED, HIGH);
    delay (300000);  //300000 = 5 minuti
    //digitalWrite(redLED, LOW);
    Serial.println(F("Riavvia"));
    delay (2000);
    ESP.restart(); //Riavvia tutto
  }

  Serial.println(F("SIM800L is OK"));
  //digitalWrite(redLED, LOW);
  delay(1000);

  
  MySerial0.print("AT+CFUN=0\r\n"); //Predispone la SIM per la funzionalità minima (AT*CFUN=0) per consumare meno energia
  //MySerial0.print("AT+CFUN=1\r\n"); //Predispone la SIM per la funzionalità minima (AT*CFUN=0) per consumare meno energia
  delay(8000);


  //digitalWrite(greenLED, LOW);

  Serial.print(F("Tempo invio dati: "));
  //Serial.println(tempoInvioDati);
 Serial.println(IntervalloInvioDati);
  //old_tempo=millis();

/*
//Setta APN
 Serial.println("Setta APN...");
SIM800.setGPRSNetworkSettings(F(FONA_APN), F(FONA_USERNAME), F(FONA_PASSWORD));
//SIM800.setGPRSNetworkSettings(F("hologram"));
delay (5000);
*/


    // Trasmissione Dati -------------------------------
    Serial.println(F("-----Tentativo accensione GPRS-----"));
    accensione_GPRS();
    Serial.println(F("-----Fine tentativo accensione GPRS-----"));



}
