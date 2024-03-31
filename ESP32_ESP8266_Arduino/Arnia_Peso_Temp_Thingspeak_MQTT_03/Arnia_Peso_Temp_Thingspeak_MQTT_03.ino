/*
 * Nome canale: Arnia Gio
 * Estensimetri e Temperatura
 * Modifica parametri: togliere il ponticello tra (D0 IO16) e RST 
 */ 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include "config.h"


// CONFIGURE HX711
#include "HX711.h"//The library used for arduino  https://github.com/bogde/HX711
#define HX711_SCK_PIN  D1
#define HX711_DOUT_PIN D2
//scale(DOUT,SCK)
HX711 scale;//(HX711_DOUT_PIN, HX711_SCK_PIN); // parameter "gain" omitted; default value 128

//Configurazione DS18B20
#define PIN_DS D4 //Termometro DS18B20
OneWire ow(PIN_DS);  //Oggetto Termometri.
DallasTemperature termometro(&ow); //



#define debug //Decommentare per debug

char * programma = "ThingSpeak MQTT Arnia Peso prova 00 ";
float vBatt=0;        //Tensione batteria.
long rssi=0;          //RSSI.
const float minuti_sleep = 1; //Minuti Sleep 
//float peso_raw=0;

           
ADC_MODE(ADC_VCC);

WiFiClient client;
PubSubClient mqttClient( client );


//--- Publish messaggi a ThingSpeak channel ------------------------------------------
void mqttPublish(long pubChannelID, String message) {
  String topicString ="channels/" + String( pubChannelID ) + "/publish";
  #ifdef debug
  Serial.println(topicString );
  #endif
  
  bool statusmqtt=mqttClient.publish( topicString.c_str(), message.c_str() );
#ifdef debug
  Serial.print("Stato mqtt: ");
  Serial.println(statusmqtt);
  Serial.println("-----");
  #endif
}

//--- Connessione al broker ------------------------------------------
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
  Serial.begin( 9600 );

  delay(10);

  #ifdef debug
    Serial.println("Avvio Wifi");
  #endif
    
   
    connectWifi();                  //Connessione al WiFi

    vBatt = system_get_vdd33()/1000.00;
    rssi = WiFi.RSSI();

#ifdef debug
    Serial.println();
    Serial.print("vBatt: ");
    Serial.println(vBatt);
    
    Serial.print("RSSI(): ");
    Serial.println(rssi);

    Serial.print("Minuti sleep: ");
    Serial.println(minuti_sleep);
#endif

  // SETUP HX711
  scale.begin(HX711_DOUT_PIN, HX711_SCK_PIN);

    letturaSensori();               //Lettura temperature

    ESP.deepSleep(minuti_sleep*60000000); //Sleep


}
//----------------------------------------------------
void loop() {

}

//-------------------------------------------------------
void letturaSensori(void){
  if (!mqttClient.connected()) { //Riconnette MQTT se sconnesso
     MQTT_connette(); 
  }
  mqttClient.loop(); //loop per maintenere connesso al server.

 long peso_raw=misura_peso_raw();
 float temp_estensimetro=misura_temp(0);

#ifdef debug  
  int fc;
  Serial.print("Invio a - channelID: ");
  Serial.println(channelID);
  Serial.println("Peso Raw: "+String(peso_raw));  
  Serial.println("Temperatura Cella: "+String(temp_estensimetro));
#endif

String messaggio="field5=" + String(peso_raw) + 
                         "&field6=" + String(temp_estensimetro) +
                         "&field7=" + String(rssi) +
                         "&field8=" + String(vBatt);

#ifdef debug 
 Serial.println(messaggio);
 Serial.println("----");
#endif
 
  mqttPublish( channelID, messaggio);
}


//--- Lettura Estensimetri ---------------------------------
long misura_peso_raw(){

  // GET weight
  scale.power_up();
  long weight_raw=0;

if (scale.is_ready()) {
    weight_raw = scale.read_average(20);
     
    
#ifdef debug 
  Serial.print(" Raw weight: ");
  Serial.println(weight_raw, 3);
#endif
  scale.power_down();  
  
  } 
else {
  #ifdef debug 
    Serial.println("HX711 not found.");
   #endif
   
	weight_raw = -1000;
  }
    return weight_raw;
}

//---- Misura Temperatura Estensimetri ----------

float misura_temp(byte sonda){
  termometro.requestTemperatures();
  float t = termometro.getTempCByIndex(sonda);
  if (t < -20 ) t=-20; //Per evitare -127 se il termometro non è connesso 

#ifdef debug 
  Serial.print("Temperatura: ");
  Serial.println(t,3);
#endif
 
  return t;
}

//---- Connect Wifi ---------------------------------------------------
void connectWifi(){
  WiFi.begin( ssid, pass );
  while ( WiFi.waitForConnectResult() != WL_CONNECTED ) {
    delay(1000);
  }
  mqttClient.setServer( server, mqttPort ); //Configure the MQTT client
  //Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the
  //message buffer will result in your callback not being invoked.
  mqttClient.setBufferSize( 2048 );
  #ifdef debug 
  Serial.println("CONNESSIONE WiFi OK");
  #endif
}
