
void fcrc(void){

T_CRC= F_XOR(Chr_Inizio, Messaggio[0]);

#ifdef debug
Serial.println(T_CRC, HEX);
#endif

for (int i=1; i<15; i++){

T_CRC= F_XOR(T_CRC, Messaggio[i]);

/*
#ifdef debug
Serial.println(T_CRC, HEX);
#endif
*/
}

T_CRC= F_XOR(T_CRC,Chr_CRC );

#ifdef debug
Serial.print("CRC: ");
Serial.print(CRC, HEX);
Serial.print(" - CRC calcolato: ");
Serial.println(T_CRC, HEX);
#endif
}

//----- Invia dati --------------------------------------------------
void finvia_dati(void){
  MQTT_connette(); 

  if(stato!=dormire){
    mqttClient.loop(); //loop per maintenere connesso al server.

fcrc(); //Calcola CRC

oled_clean();
if ((unsigned char)T_CRC == (unsigned char)CRC){
    
    String testo="field1=" + String(Messaggio[0]) +
                 "&field2=" + String(Messaggio[1]) +
                 "&field3=" + String(Messaggio[2]) +
                 "&field4=" + String(Messaggio[3]) +
                 "&field5=" + String(Messaggio[6]) +
                 "&field6=" + String(Messaggio[7]) +
                 "&field7=" + String(Messaggio[8]) +
                 "&field8=" + String(Messaggio[9]); 

        // Serial.print("Messaggio MQTT: ");
         //Serial.println(testo);

oled();
                
      
      bool statusmqtt=mqttPublish( channelID, testo);
    
    if(statusmqtt==1){
        Serial.println("Messaggio MQTT inviato");
        stato=disconnectWifi;
        digitalWrite(D1, LOW); //set 0
    }
    else{
          Serial.println("Messaggio MQTT NON inviato");
                  stato=disconnectWifi;
    }

  }
else{
  oled_clean();
Serial.println("Dati non consistenti");
    u8x8.setCursor(0, 5);
    u8x8.print("Bad data!");
stato=dormire;
}

//reset variabili
i=0;
memset(Messaggio, '\0', sizeof(Messaggio)*sizeof(byte));
memset(buff, '\0', sizeof(buff)*sizeof(byte));

  }
}



//---Connessione al broker------------------------------------------
void MQTT_connette() {

  connectWifi();  //Connessione al WiFi


  if(stato!=dormire){

  mqttClient.setServer( server, mqttPort ); //Configure the MQTT client
  //Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the
  //message buffer will result in your callback not being invoked.
  mqttClient.setBufferSize( 2048 );
  //Serial.println("CONNESSIONE WiFi OK");
  delay(200);

  
  while ( !mqttClient.connected() ){
    if ( mqttClient.connect( clientID, mqttUserName, mqttPass ) ) {
#ifdef debug
      Serial.println();
      Serial.print( "MQTT a: " );
      Serial.print( server );
      Serial.print (" porta: ");
      Serial.print( mqttPort );
      Serial.println( " OK." );
#endif        
    } 
    
    else {
#ifdef debug 
      Serial.println();     
      Serial.print( "MQTT connessione faillita, rc = " );
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print( mqttClient.state() );
      Serial.println( " Riprova fra qualche secondo" );
      delay(1000);
		WiFi.disconnect(); 
      Serial.println("Connessione wifi disconessa");
      stato=dormire;
		break;
#endif      
    }
  }
 }
}





//---Publish messaggi a ThingSpeak channel  --------------------------------------------------
bool mqttPublish(long pubChannelID, String message) {
  String topicString ="channels/" + String( pubChannelID ) + "/publish";
  Serial.print("Topic mqtt:"); Serial.println(topicString);
  Serial.print("Messaggio mqtt:"); Serial.println(message);
  return mqttClient.publish( topicString.c_str(), message.c_str() );
}

//---fmqtt  --------------------------------------------------
void fmqtt(){}
