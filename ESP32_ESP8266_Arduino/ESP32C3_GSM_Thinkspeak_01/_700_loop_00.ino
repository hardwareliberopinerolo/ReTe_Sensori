/*---------Questa sessione è eseguita ciclicamente ogni secondo---------------------------------------*/
void loop() {
 
    
/* 
    
//Incrementa il contatore  per invio dati
  INVIOsecond++;
   //Aggiunti io per test
    Serial.print(F("INVIO secondi :"));
    Serial.println(INVIOsecond);


  
  // Quando il tempo di invio INVIOsecond raggiunge il valore corrispondente a tempoInvioDati     (es.2 ore espresso in secondi ) viene inviato il pacchetto dati a Thingspeak eseguendo la funzione 'accensione_GPRS' (vedi dopo)

  if (INVIOsecond  >= tempoInvioDati) { //tentativo invio dei dati raccolti se è passato il tempo prestabilito
Invio_Dati();
  }
*/


if (millis() - old_tempo > IntervalloInvioDati){
  
 Serial.println("Invio dati....");
  Invio_Dati();
  old_tempo=millis();
  
  }

  
}
