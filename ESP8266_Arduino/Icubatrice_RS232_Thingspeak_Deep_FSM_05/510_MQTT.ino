//----- Invia dati --------------------------------------------------
void finvia_dati(void){
  MQTT_connette(); 

  if(stato!=dormire){
    mqttClient.loop(); //loop per maintenere connesso al server.
    String testo="field5=" + String(Messaggio[6]) +
                 "&field6=" + String(Messaggio[7]) +
                 "&field7=" + String(Messaggio[8]) +
                 "&field8=" + String(Messaggio[9]); 

        // Serial.print("Messaggio MQTT: ");
         //Serial.println(testo);
                     
      
      bool statusmqtt=mqttPublish( channelID, testo);
    
    if(statusmqtt==1){
        Serial.println("Messaggio MQTT inviato");
        stato=disconnectWifi;
    }
    else{
          Serial.println("Messaggio MQTT NON inviato");
          stato=dormire;
    }
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
      stato=dormire;
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
