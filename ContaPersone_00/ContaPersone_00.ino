/*
 * Nome canale: Controllo tempeature
 * 
 * Modifica parametri: togliere il ponticello tra (D0 IO16) e RST 
 */ 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


/*
extern "C" {
#include "user_interface.h" // this is for the RTC memory read/write functions
}
*/

#include "config.h"


#define debug //Decommentare per debug


WiFiClient client;
PubSubClient mqttClient( client );

const int RTC_block=64;



//---Publish messaggi a ThingSpeak channel.
void mqttPublish(long pubChannelID, String message) {
  
  String topicString ="channels/" + String( pubChannelID ) + "/publish";

 Serial.println(topicString);
 Serial.println( message);

 mqttClient.publish( topicString.c_str(), message.c_str() ) ;
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

#ifdef debug    
    Serial.println();
    Serial.println("Letture e invio");
#endif    
 
 connectWifi();                  //Connessione al WiFi
 inviaMQTT();

 #ifdef debug    
 Serial.println("Vado a dormire...");
#endif 
delay(500);  
 ESP.deepSleep(0); //Sleep



}
//----------------------------------------------------
void loop() {
}

//-------------------------------------------------------
void inviaMQTT(void){

 
  if (!mqttClient.connected()) { //Riconnette MQTT se sconnesso
     MQTT_connette(); 
  }
  
  //mqttClient.loop(); //loop per maintenere connesso al server.


int memoria;
uint64 Conteggio;

system_rtc_mem_read(RTC_block, &memoria, sizeof(memoria));

#ifdef debug  
Serial.print("Memoria: ");
Serial.println(memoria);
#endif  

if (memoria==0){
system_rtc_mem_read((RTC_block+sizeof(memoria)), &Conteggio, sizeof(Conteggio));
/*Serial.print("Conteggio: ");
Serial.println(Conteggio);*/
Conteggio =Conteggio +10;
/*Serial.print("Conteggio: ");
Serial.println( Conteggio);*/
system_rtc_mem_write((RTC_block+sizeof(memoria)), &Conteggio, sizeof(Conteggio));


#ifdef debug  
  Serial.print("Invio a - channelID: ");
  Serial.println(channelID);
  Serial.print("Conteggio: ");  
  Serial.println(Conteggio);


#endif  
  
 //mqttClient.loop(); //loop per maintenere connesso al server. 
 mqttPublish( channelID, "field1=" + String(Conteggio) );
  //mqttPublish( channelID, "field1=26" );

 /*
  mqttPublish( channelID, "field1=" + String(Conteggio) +
                         "&field2=" + String(10)+
                         "&field3=" + String(25) +
                         "&field4=" + String(65));
*/

}

else{ //Azzero memoria RTC al primo avvio
#ifdef debug  
  Serial.println("Azzero RTC mem");
#endif 
  
  memoria=0;
  Conteggio=0;
 system_rtc_mem_write(RTC_block, &memoria, sizeof(memoria));
 system_rtc_mem_write((RTC_block+sizeof(memoria)), &Conteggio, sizeof(Conteggio)) ;
  }


}


//-------------------------------------------------------
void connectWifi(){
  WiFi.begin( ssid, pass );
  while ( WiFi.waitForConnectResult() != WL_CONNECTED ) {
    #ifdef debug  
  Serial.print(".");
#endif
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
