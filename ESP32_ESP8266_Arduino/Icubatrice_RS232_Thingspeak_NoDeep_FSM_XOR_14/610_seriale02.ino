


//---flegge_seriale-------------------------------------------------

void flegge_seriale(){

 //leggo da seriale;
  if (SWSerial.available()) {
   n =SWSerial.read(); 
   Serial.println(n, HEX);
   digitalWrite(D1, HIGH); //per set a 1
    if(messaggio_status == 1){
      buff[i] = n; 
      i++; 
    }
   if ((char)n == '@'&& i== 0) {
        Serial.println("Inizio messaggio");
        messaggio_status=1;
        tempo_precedente=millis();
//        stato=legge_seriale;
   }
    //elaboramessaggio
   if (messaggio_status == 1 && i == 18) {
       Serial.print("CRK ricevuto: ");
       CRC = n;
       Serial.println(n, HEX);
       messaggio_status=0;
       
      memcpy(&Messaggio, buff, i);

      Serial.print("PMV:"); Serial.print(String(Messaggio[0]));
      Serial.print("MVU:"); Serial.print(String(Messaggio[1]));
      Serial.print("STR:"); Serial.print(String(Messaggio[2]));
      Serial.print("RSB:"); Serial.print(String(Messaggio[3]));


      Serial.print("T1:"); Serial.print(String(Messaggio[6]));
      Serial.print(" - T2:"); Serial.print(String(Messaggio[7]));
      Serial.print(" - H:"); Serial.print(String(Messaggio[8]));
      Serial.print(" - Ta:"); Serial.print(String(Messaggio[9]));
      Serial.print("\n");
      
      Serial.print("ricevuti: ");  
      Serial.print(i);  
      i = 0;
      Serial.println(" bytes - Fine messaggio"); 
      stato=invia_dati; //invia dati a Thingspeak
    }
    stato=legge_seriale;
	}
  else {
    stato=legge_seriale;
  }  
}

/*
//------connectWifi()-------------------------------------------------

void fanalizza_dato(){
        //i = 0; 
	if (messaggio_status == 1 && i == 19) {
     stato=elaboramessaggio;
	}
}

 	else if ((char)n == '@'&& i== 0) {
        Serial.println("Inizio messaggio");
//        digitalWrite(D1, HIGH); //per set a 1
        messaggio_status=1;
        tempo_precedente=millis();
        //Serial.println(tempo_precedente);
      stato=legge_seriale;
        
	}
    
	else{
      if(messaggio_status){
        buff[i] = n; 
        i++; 
        stato=legge_seriale;}
      else{
        stato=legge_seriale;
      }

	} //end else
}


void felaboramessaggio(){

      Serial.print("ricevuto");
      memcpy(&Messaggio, buff, i);

      Serial.print("PMV:"); Serial.print(String(Messaggio[0]));
      Serial.print("MVU:"); Serial.print(String(Messaggio[1]));
      Serial.print("STR:"); Serial.print(String(Messaggio[2]));
      Serial.print("RSB:"); Serial.print(String(Messaggio[3]));


      Serial.print("T1:"); Serial.print(String(Messaggio[6]));
      Serial.print(" - T2:"); Serial.print(String(Messaggio[7]));
      Serial.print(" - H:"); Serial.print(String(Messaggio[8]));
      Serial.print(" - Ta:"); Serial.print(String(Messaggio[9]));
      Serial.print("\n");
      
      Serial.print("ricevuti: ");  
      Serial.print(i+1);  
      Serial.println(" bytes - Fine messaggio"); 


  stato=invia_dati; //Non invia dati a Thingspeak
  //stato=dormire;
}
*/
