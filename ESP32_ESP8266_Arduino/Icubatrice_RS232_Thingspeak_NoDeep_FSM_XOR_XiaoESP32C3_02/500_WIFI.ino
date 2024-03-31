//------ connectWifi -------------------------------------------------
void connectWifi(){

  //WiFi.setAutoConnect(true);
    WiFi.setAutoConnect(false);
  WiFi.persistent(true);   
  WiFi.begin( ssid, pass );
  Serial.println("Connessione WiFi");
  unsigned long tempo=millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    if (millis() - tempo > IDLE_TIME){
      Serial.println("Connessione Fallita");
      //delay(1000);
          stato=dormire;
          break;
    }
  }

}


//------ fdisconnectWifi -------------------------------------------------
void fdisconnectWifi(){
  mqttClient.disconnect();
  WiFi.disconnect(); 
  Serial.println("Connessione mqtt e wifi disconesse");
  stato=dormire;
}
