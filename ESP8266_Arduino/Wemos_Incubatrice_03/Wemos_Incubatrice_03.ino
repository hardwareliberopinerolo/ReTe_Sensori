/*
 Incobatrice DHT22
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#include <EEPROM.h>
//#include <Adafruit_Sensor.h> 
#include <DHT.h>             //DHT_sensor_library-1.4.3

#include "config.h"
#include "paginaHTML.h"

#define debug //Decommentare per debug
//#define ApIp  //ACCESS POINT oppure IP

//#define seteeprom //Decommentare per set della eprom


String programma="Wemos_Incubatrice_00";

float vBatt=0;        //Tensione batteria.
long rssi=0;          //RSSI.
float minuti_sleep = 0.5; 

const unsigned long IDLE_TIME=60000;

String XML;           


ADC_MODE(ADC_VCC);

DHT dht(PIN_dht, DHTTYPE);   //Sensore DHT

WiFiClient client;
PubSubClient mqttClient( client );
ESP8266WebServer serverAP(80);

//---Publish messaggi a ThingSpeak channel--------------------------
void mqttPublish(long pubChannelID, String message) {
  String topicString ="channels/" + String( pubChannelID ) + "/publish";
#ifdef debug
  Serial.print("topic: ");
  Serial.println(topicString);
  Serial.print("messaggio: ");
  Serial.println(message);
#endif
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
            Serial.println( "MQQT fallito - Va in deep sleep" );
      delay(1000);
#endif  

      ESP.deepSleep(minuti_sleep*60000000); //Sleep    
    }
  }
}
//------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("Avvio ESP");
  EEPROM.begin(512);  //Initialize EEPROM






bool status_PIN_mod=0;

//per usare PIN_mod per attivare AP e pagina internet
/*
pinMode(PIN_mod,INPUT);
delay(50);
bool status_PIN_mod=digitalRead(PIN_mod);
*/

  if(status_PIN_mod==0){ //Ciclo.
#ifdef debug    
    Serial.println();
    Serial.println("Letture e invio");
#endif    
    connectWifi();        //Connessione al WiFi
    //vBatt = ESP.getVcc()/1000.00;
    vBatt = system_get_vdd33()/1000.00;
    rssi = WiFi.RSSI();

#ifdef debug
    Serial.println();
    Serial.print("vBatt: ");
    Serial.println(vBatt);
    
    Serial.print("RSSI(): ");
    Serial.println(rssi);
#endif

//attiva pin alimentazione DHT22
  pinMode(PIN_alim_sensori, OUTPUT);
  digitalWrite(PIN_alim_sensori,HIGH);
delay(2000);
  dht.begin();
  delay(1000);
  letturaSensori();     //Lettura e invio dati

#ifdef seteeprom
//-----Da eseguire alla prima accensione del ESP per registrare i minuti di sleep nella Eeprom    
      EEPROM.write(indSleepEE,minuti_sleep);
      delay(30);
      EEPROM.commit();
      delay(20);
#endif
    
    minuti_sleep = EEPROM.read(indSleepEE);

    
    #ifdef debug
      Serial.print("intervallo letture: ");
      Serial.println(minuti_sleep);
    #endif
    
    delay(1000); 
    digitalWrite(PIN_alim_sensori,LOW); 

 Serial.println("Sleep");
    ESP.deepSleep(minuti_sleep*60000000); //Sleep
  }else{  //Parametri.
    Serial.println();
    modificaParametri();
    Serial.println("Pronto modifica parametri"); 
    Serial.print("Indirizzo IP: ");
    Serial.println("Letture e invio");
  }
}
//------------------------------------------------
void loop() {
  serverAP.handleClient();
  delay(10);
}
//-----------------------------------------------------
void modificaParametri(void){
#ifdef ApIp
  WiFi.mode(WIFI_AP);  
  WiFi.softAP("Essicatore"); //Nome access point.
#else                        //Accedere con 192.168.4.1
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 Serial.println();
 Serial.print("WiFi connesso - IP:");
 Serial.println(WiFi.localIP());
 Serial.println();
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
  mqttClient.loop(); //loop per mantenere connesso al server.


  DHT_hum =dht.readHumidity();
  delay(50);
  DHT_temp=dht.readTemperature();

#ifdef debug  
  Serial.print("Invio a channelID: ");
  Serial.println(channelID);
  Serial.println("Umidità: "+String(DHT_hum));  
  Serial.println("Temperatura: "+String(DHT_temp)); 
#endif  



  mqttPublish( channelID,"&field1=" + String(DHT_temp)+
                         "&field2=" + String(DHT_hum)+
                         "&field3=" + String(rssi) +
                         "&field4=" + String(vBatt));


/*
  mqttPublish( channelID,"&field5=" + String(rssi) +
                         "&field6=" + String(vBatt));
 */                     
               
  delay(100);                         
}

//-------------------------------------------------------
void connectWifi(){
  WiFi.setAutoConnect(true);
  WiFi.persistent(true);   
  WiFi.begin( ssid, pass );
  Serial.println("Connessione WiFi");
  unsigned long tempo=millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    if (millis() - tempo > IDLE_TIME){
      Serial.println("Connessione Fallita - Attivo Deep Sleep");
      delay(1000);
          ESP.deepSleep(minuti_sleep*60000000); //Slee
      }
    


    
  }
 Serial.println();
 Serial.print("WiFi connesso - IP:");
 Serial.println(WiFi.localIP());
 Serial.println();

  mqttClient.setServer( server, mqttPort ); //Configure the MQTT client
  //Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the
  //message buffer will result in your callback not being invoked.
  mqttClient.setBufferSize( 2048 );

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
//---Riceve i parametri impostati da paginaHTML------------
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
