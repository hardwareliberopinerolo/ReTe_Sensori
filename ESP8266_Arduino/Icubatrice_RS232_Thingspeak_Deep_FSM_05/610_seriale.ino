


/*

if (millis() - tempo_serial > tempo_serial_max){
      Serial.println("Nessun messaggio da seriale");
      delay(1000);
          dormire();
      }
*/

/*
if(messaggio_status==1){
        //delta=millis()-tempo_precedente;
        //Serial.println(delta);
        
   if((millis()-tempo_precedente>=tempo_attesa)){
  Serial.println("messaggio incompleto");
  dormire();
  messaggio_status=0;
  i=0;
  //crk_status=0;
  }
}
*/

//---flegge_seriale-------------------------------------------------

void flegge_seriale(){

 //Serial.println("leggo da seriale");
  if (SWSerial.available()) {
   n =SWSerial.read(); 
   Serial.println(n, HEX);
	stato=analizza_dato;
	}
 else {
  stato=legge_seriale;
  }  

}

//------connectWifi()-------------------------------------------------

void fanalizza_dato(){
	if ((char)n == '#' && messaggio_status == 1) {
       crk_status=1;
        //messaggio_status=0;	
        stato=legge_seriale;

	}

 
	else if ((char)n == '@') {
        Serial.println("Inizio messaggio");
        i = 0; 
        messaggio_status=1;
        tempo_precedente=millis();
        //Serial.println(tempo_precedente);
stato=legge_seriale;
        
	}
	else if ((char)n == 'D') {
        Serial.println("Inizio messaggio");
        //i = 0; 
        messaggio_status=0;
        //tempo_precedente=millis();
        Serial.println("Messaggio Display");
        //dormire();
stato=legge_seriale;
        
    }
	else{
      if(messaggio_status && crk_status){ 
       Serial.print("CRK ricevuto: ");
       Serial.println(n, HEX);
       crk_status=0;
       messaggio_status=0;
       stato=elaboramessaggio;
       }
       
      else if(messaggio_status){
        buff[i] = n; 
        i++; 
          stato=legge_seriale;}
      else{
        stato=legge_seriale;
        }

	} //end else

}



void felaboramessaggio(){

      memcpy(&Messaggio, buff, i);
      Serial.print("T1:"); Serial.print(String(Messaggio[6]));
      Serial.print(" - T2:"); Serial.print(String(Messaggio[7]));
      Serial.print(" - H:"); Serial.print(String(Messaggio[8]));
      Serial.print(" - Ta:"); Serial.print(String(Messaggio[9]));
      Serial.print("\n");
      
      Serial.print("ricevuti: ");  
      Serial.print(i+1);  
      Serial.println(" bytes - Fine messaggio"); 


  stato=invia_dati;

}
