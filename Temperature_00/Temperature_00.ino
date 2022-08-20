/*
 * Nome canale: Controllo tempeature
 * 
 * Modifica parametri: togliere il ponticello tra (D0 IO16) e RST 
 */ 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include "config.h"
#include "paginaHTM.h"

//#define debug //Decommentare per debug
//#define ApIp  //ACCESS POINT oppure IP
char * programma = "ThingSpeakMQTTSleep_05-02-22_01";
float vBatt=0;        //Tensione batteria.
long rssi=0;          //RSSI.
int minuti_sleep = 0; 
float temp_Interna=0;
float temp_Esterna=0;
String XML;           
ADC_MODE(ADC_VCC);
WiFiClient client;
PubSubClient mqttClient( client );
OneWire ow(PIN_term);  //Oggetto Termometri.
DallasTemperature termometro(&ow); //
ESP8266WebServer serverAP(80);

//---Publish messaggi a ThingSpeak channel.
void mqttPublish(long pubChannelID, String message) {
  String topicString ="channels/" + String( pubChannelID ) + "/publish";
  mqttClient.publish( topicString.c_str(), message.c_str() );
}

//---Connessione al broker------------------------------------------
void MQTT_connette() {
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
    } else {
#ifdef debug 
      Serial.println();     
      Serial.print( "MQTT connessione faillita, rc = " );
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print( mqttClient.state() );
      Serial.println( " Riprova fra qualche secondo" );
      delay(1000);
#endif      
    }
  }
}

//-----------------------------------------------
void setup() {
  Serial.begin( 115200 );
  EEPROM.begin(512);  //Initialize EEPROM
  delay(10);
  pinMode(PIN_mod,INPUT);
  pinMode(PIN_alim_term,OUTPUT);
  digitalWrite(PIN_alim_term,HIGH);
  if(digitalRead(PIN_mod)==0){
#ifdef debug    
    Serial.println();
    Serial.println("Letture e invio");
#endif    
    connectWifi();                  //Connessione al WiFi
    //vBatt = ESP.getVcc()/1000.00;
    vBatt = system_get_vdd33()/1000.00;
    rssi = WiFi.RSSI();
    letturaSensori();               //Lettura temperature
    minuti_sleep = EEPROM.read(indSleepEE);
    delay(30); 
    digitalWrite(PIN_alim_term,LOW); 
#ifdef debug
    Serial.println();
    Serial.print("vBatt: ");
    Serial.println(vBatt);
    
    Serial.print("RSSI(): ");
    Serial.println(rssi);
    Serial.print("Minuti sleep: ");
    Serial.println(minuti_sleep);
#endif
    ESP.deepSleep(minuti_sleep*60000000); //Sleep
  }else{
    Serial.println();
    modificaParametri();
    Serial.println("Pronto modifica parametri"); 
    Serial.print("Indirizzo IP: ");
    Serial.println(WiFi.localIP());
  }
}
//----------------------------------------------------
void loop() {
  serverAP.handleClient();
  delay(10);
}
//-----------------------------------------------------
void modificaParametri(void){
#ifdef ApIp
  WiFi.mode(WIFI_AP);  
  WiFi.softAP("TempFrigo"); //Nome access point.
#else                        //Accedere con 192.168.4.1
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  WiFi.config(IPAddress(192,168,1,121), IPAddress(192,168,1,1), IPAddress(255,255,255,1));
#endif    
  minuti_sleep = EEPROM.read(indSleepEE);
  delay(20);
  Serial.print("Intervallo impostato: ");
  Serial.println(minuti_sleep);
  serverAP.on("/", Carica_paginaHTML);     //Caricamento iniziale pagina HTML.
  serverAP.on("/programma", [](){          //Visualizza un messaggio.
  serverAP.send(200, "text/plain", programma);});
  serverAP.on("/comandi",Esegue_comandi);  //Riceve i comandi dal pagina HTML.
  serverAP.begin();
}
//-------------------------------------------------------
void letturaSensori(void){
  if (!mqttClient.connected()) { //Riconnette MQTT se sconnesso
     MQTT_connette(); 
  }
  mqttClient.loop(); //loop per maintenere connesso al server.
  temp_Interna = misuraTemp(0); //Termometro interno.
  temp_Esterna = misuraTemp(1); //     "     esterno.
#ifdef debug  
  int fc;
  Serial.print("Invio a - channelID: ");
  Serial.println(channelID);
  Serial.print("T. esterna: "+String(temp_Esterna));  
  Serial.println(" T. interna: "+String(temp_Interna)); 
#endif  
  
  mqttPublish( channelID, "field1=" + String(temp_Interna) +
                         "&field2=" + String(temp_Esterna) +
                         "&field3=" + String(rssi) +
                         "&field4=" + String(vBatt));
}
//---Lettura termometri---------------------------------
float misuraTemp(byte sonda){
  termometro.requestTemperatures();
  float t = termometro.getTempCByIndex(sonda);
  if (t < -45 ) t=0; //Per evitare -127 se il termometro non è connesso 
  return t;
}
//-------------------------------------------------------
void connectWifi(){
  WiFi.begin( ssid, pass );
  while ( WiFi.waitForConnectResult() != WL_CONNECTED ) {
    delay(1000);
  }
  mqttClient.setServer( server, mqttPort ); //Configure the MQTT client
  //Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the
  //message buffer will result in your callback not being invoked.
  mqttClient.setBufferSize( 2048 );
  //Serial.println("CONNESSIONE WiFi OK");
}

//---Carica la pagina dal file paginaHTML.h--------------- 
void Carica_paginaHTML(){
  serverAP.send(200,"text/html",paginaHTML);
}
//---Funzione eseguita in continuazione-------------------
void Esegue_comandi(){
  riceveHTML(); //Eesegue i comandi ricevuti
  componeXML(); //Risposte a paginaHTML.
  serverAP.send(200,"text/xml",XML); //Invia alla pagina HTML.
}
//---Riceve i parametri impstati da paginaHTML------------
void riceveHTML(void){
  for ( uint8_t i = 0; i < serverAP.args(); i++ ) {
    if (serverAP.argName(i)=="intervMod") {
      minuti_sleep = serverAP.arg(i).toInt();
      EEPROM.write(indSleepEE,minuti_sleep);
      delay(30);
      EEPROM.commit();
      delay(20);
      Serial.print("intervallo letture: ");
      Serial.println(minuti_sleep);
    }
  }
}
//---Prepara file XML da rinviare alla pagina HTML.
void componeXML(){
  XML="<?xml version = \"1.0\" ?>";
  XML+="<inputs>";

  XML+="<tempi>";
  XML+=minuti_sleep;
  XML+="</tempi>";
  
  XML+="</inputs>";
}
