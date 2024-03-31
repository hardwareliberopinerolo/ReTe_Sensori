
/*---------------------------------------------------------------------------------------------------------------------------------
  //In questa sezione viene gestito il modulo SIM 800L. E' una routine chiamata solo quando occorre inviare i dati (vedi Void loop)*/
void accensione_GPRS()
{
  
//Setta APN
SIM800.setGPRSNetworkSettings(F(FONA_APN), F(FONA_USERNAME), F(FONA_PASSWORD));
//SIM800.setGPRSNetworkSettings(F("hologram"));


delay (5000);

/*-----Registrazione su network-----*/
  SIM800ss.print("AT+CFUN=1\r\n");

  Serial.println(F("Waiting to be registered to network..."));
  i = 0;

  while (!connection_on && i < 60) {  //Fa 60 tentativi.
    digitalWrite(redLED, LOW);  //Ad ogni ciclo spegne il led rosso
    net_status = SIM800.getNetworkStatus(); //Legge lo stato del network
    Serial.print(F("i : "));
    Serial.print(i);
    Serial.print(F(" net_status : "));
    Serial.println(net_status);

    if (net_status == 1 || net_status == 5 ) {     //Connesso. Abilitazione con net_status == 5
      /*  net_status=1 significa 'connesso al network'. Net_status=5 significa 'connesso in roaming'.
        Con la SIM Thingsmobile a volte il net_status non va mai a  1,  resta solo 5. Per questo abbiamo messo nel while net_status != 5. */
      /*Nota : se non risulta connesso, va in standby fisso, occorre un reset manuale per farlo ripartire...*/
      Serial.println(F("Registered to home network!"));
      connection_on = true; //Se si è connesso aggiorna questa variabile
      digitalWrite(greenLED, HIGH); //Accende il led verde se si connette
      digitalWrite(redLED, LOW); //Ripristina il led

    } else { //Se non si è connesso ...
      Serial.println(F("Not connected to network!"));
      connection_on = false;
      digitalWrite(redLED, HIGH);  //Led rosso se non si è connesso
      delay(2000); //Tempo di attesa 2 sec prima di riprovare. Con delay 2000  verifica per  max 3 min se si è connesso (90 tentativi x 2 secondi)
      i++;
    }

  }

  /*-----accensione gprs-----*/
  if (connection_on) {  //Faccio eseguire accensione gprs solo se prima si è connesso al network.
    Serial.println(F("Turning on GPRS... "));
    delay(2000);
    i = 0;

    while (!gprs_on && i < 10) { //Tentativi per avvio gprs.
      digitalWrite(redLED, LOW);  //Ad ogni ciclo spegne il led rosso

      if (!SIM800.enableGPRS(true)) {
        Serial.println("Failed to turn on GPRS");
        Serial.println("Trying again...");
        gprs_on = false;
        digitalWrite(redLED, HIGH);
        delay(2000);
        i++;

      } else {
        Serial.println("GPRS now turned on");
        delay(2000);
        gprs_on = true;
        digitalWrite(greenLED, HIGH); //Accende il led verde se si GPRS OK
        digitalWrite(redLED, LOW);
      }
    }
  }
}
