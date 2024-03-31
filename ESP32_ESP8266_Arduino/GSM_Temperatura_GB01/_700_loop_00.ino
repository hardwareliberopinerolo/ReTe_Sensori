/*---------Questa sessione è eseguita ciclicamente ogni secondo---------------------------------------*/
void loop() {


//Sensore temperatura -------------------------------




 if(millis()-tiempoAnterior>=periodo){  //Evalúa si ha transcurrido el periodo programado
    legge_temperatura();
    tiempoAnterior=millis();  //Almacena el tiempo actual como referencia
    }




  
  if (seconds != myTimer.readTimer()) { //controllo dei valori ogni secondo
    seconds = myTimer.readTimer();
    state ^= 1; //Funzione booleana xor. Inverte il valore della variablie state (byte)
    digitalWrite(greenLED, state);


/*
    //In questa sezione viene incrementato un contatore in base al passaggio di corrente.
      //Se il sensore di hall rileva corrente viene incrementata la variabile durataAttiva altrimenti viene incrementata la variabile durataSpenta

    sensorValue = digitalRead(sensor); //lettura dell'input e incremento variabili
    if (sensorValue == 0) {
      durataAttiva++;
    }
    else {
      durataSpenta++;
    }
*/

    /*Incrementa il contatore  per invio dati*/
    INVIOsecond++;

    /*Aggiunti io per test*/
    Serial.print(F("INVIO secondi :"));
    Serial.println(INVIOsecond);

  }
  //--------------------------
  /*Quando il tempo di invio INVIOsecond raggiunge il valore corrispondente a tempoInvioDati
    (es.2 ore espresso in secondi ) viene inviato il pacchetto dati a Thingspeak eseguendo la funzione 'accensione_GPRS' (vedi dopo)*/

  if (INVIOsecond  >= tempoInvioDati) { //tentativo invio dei dati raccolti se è passato il tempo prestabilito
    digitalWrite(greenLED, LOW); //Spegne il led verde  durante l'invio
    tempoAttSend = durataAttiva / 60; //carico il tempo di attività in una variabile per l'invio dei dati e resetto i tempi
    tempoMis = (durataAttiva + durataSpenta) / 60; //Carico il tempo di totale di misurazione in una variabile per l'invio dei dati

    //Sensore temperatura -------------------------------
    //legge_temperatura();



    // Trasmissione Dati -------------------------------
    Serial.println(F("-----Tentativo accensione GPRS-----"));
    accensione_GPRS();
    Serial.println(F("-----Fine tentativo accensione GPRS-----"));

    /*----Invio dati.Aggiunto io 'if gprs_on' : esegue questa parte solo se l'accensione GPRS è andata a buon fine------*/
    if (gprs_on) {
      Serial.println(F("----Tentativo invio dati...-----"));
      dtostrf(tempoAttSend, 0, 0, atm_time_string); //Comando che trasforma una variabile float in stringa. In pratica, salva il tempo nella stringa 'atm_time_string'
      Serial.print(F("atm_time_string : "));
      Serial.println(atm_time_string);
      dtostrf(tempoMis, 0, 0, atm_time_string2); //Salva anche tempo misurazione (attiva+spenta)
      Serial.print(F("atm_time_string2 : "));
      Serial.println(atm_time_string2);
      //Temperatura1
      dtostrf(temperatura1, 0, 1, atm_time_string3); //Salva temperatura 1
      Serial.print(F("atm_time_string3 : "));
      Serial.println(atm_time_string3);
      //umidita1
      dtostrf(umidita1, 0, 0, atm_time_string4); //Salvare umidita 1
      Serial.print(F("atm_time_string4 : "));
      Serial.println(atm_time_string4);
      //Temperatura2
      dtostrf(temperatura2, 0, 1, atm_time_string5); //Salva temperatura 2
      Serial.print(F("atm_time_string3 : "));
      Serial.println(atm_time_string5);
      //umidita2
      dtostrf(umidita2, 0, 0, atm_time_string6); //Salva umidita 2
      Serial.print(F("atm_time_string4 : "));
      Serial.println(atm_time_string6);
      //Analogico pin 6
      dtostrf(analog1, 0, 0, atm_time_string7); //Salva analogico pin 6
      Serial.print(F("atm_time_string7 : "));
      Serial.println(atm_time_string7);
      //Analogico pin 7
      dtostrf(analog2, 0, 0, atm_time_string8); //Salva analogico pin7
      Serial.print(F("atm_time_string8 : "));
      Serial.println(atm_time_string8);

      sprintf(http_cmd, "%s=%s&field4=%s", url_string, atm_time_string3, atm_time_string5); //Comando per concatenare in una stringa (http_cmd) più stringhe che prendono il posto dei %s
      Serial.println(http_cmd); //http_cmd è un char array che contiene l' API URL + il dato da inviare (durata attiva della pompa)
      delay(1000);

      /*---Creazione HTTP request tramite la funzione HTTP_GET_start. In pratica controlla se il server thingspeak è attivo-------*/
      i = 0;
      while (!tcp_on && i < 10) {  //Fa 10 tentativi.

        if (!SIM800.HTTP_GET_start(http_cmd, &statuscode, (uint16_t *)&length)) { //Statuscode is an integer that stores the server response and length is the length of the response
          Serial.println(F("---Server NOT OK---"));  //Il server non ha risposto, i dati non sono stati inviati.
          Serial.println(F("Trying again"));
          tcp_on = false;
          digitalWrite(redLED, HIGH); //Accende il led rosso. Significa : server non ha risposto
          i++; //Fa 10 tentativi

        } else {
          Serial.println(F("---Server OK. Response : "));  //Il server  ha risposto, mostro i dati della risposta
          tcp_on = true;
          while (length > 0) { //Non capisco cosa e 'lenght'. In ogni caso rappresenta la lunghezza dei dati registrati da qualche parte (nella sim ?)
            while (SIM800.available()) {
              char c = SIM800.read();
              response += c; //response=response + c
              length--; //-- decrementa lenght
            }
          }
          Serial.println(response); //Mostra i risultati di tutti gli invii della sessione di accensione. Si resetta ad ogni accensione
          if (statuscode == 200) {
            Serial.println(F("---Dati inviati!---"));
            digitalWrite(greenLED, LOW);
            digitalWrite(redLED, LOW);
            durataAttiva = 0; //Azzera solo se ha inviato idati
            durataSpenta = 0;
            NFailure = 0 ; //Azzera il n. di connessioni fallite.
          }
        }
      }
      if (!tcp_on) {  //Se alla fine dei 10 tentativi il server non ha risposto...
        Serial.println(F("---Dati NON inviati! Nuovo tentativo dopo 'tempoinviodati'---"));
        NFailure++; //Conta le connessioni fallite.
      }
    } else {  //Se non si è avviato il gprs..
      Serial.println(F("---Dati non inviati. Nuovo tentativo dopo 'tempoinviodati' "));
      NFailure++; ///Conta le connessioni fallite.
    }

    tcp_on = false;
    gprs_on = false;
    connection_on = false;
    INVIOsecond = 0; //Resetta il contatore per invio dati sia che abbia inviato sia che non abbia inviato. Ritenterà dopo l'intervallo stabilito.

    SIM800ss.print("AT+CFUN=0\r\n"); //Spostato  qui per farlo eseguire sempre dopo il tentativo di invio
    Serial.print(F("NFailure : ")); //Controllo connessioni fallite
    Serial.println(NFailure);

    //Riavvia tutto se per 3 volte non è riuscito a inviare dati. Succede infatti che la SIM si 'blocchi' se non riesce ad inviare...
    if (NFailure > 2) {
      delay (5000);
      Riavvia();
    }
  }
}
