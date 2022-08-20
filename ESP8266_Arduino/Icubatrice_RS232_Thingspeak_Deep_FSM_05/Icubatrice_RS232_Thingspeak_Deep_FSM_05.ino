#include <SoftwareSerial.h> // https://github.com/plerup/espsoftwareserial/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <WiFiClientSecure.h>


#define debug //Decommentare per debug

byte Messaggio[16];
byte buff[16];
int i = 0;
bool crk_status=0;
bool messaggio_status=0;

#define MYPORT_TX 14
#define MYPORT_RX 12

SoftwareSerial SWSerial;

byte n;
unsigned long tempo_precedente=0;
unsigned long tempo_serial=0;
//unsigned long delta=0;
const unsigned long tempo_attesa=500;//msec
const unsigned long IDLE_TIME=60000; // 1 min
const unsigned long tempo_serial_max=300000; // 5 min

const int minuti_sleep = 1;

/*
Messaggio da iviare in Esadecimale @.....#
 
40 00 00 00 00 00 00 14 15 1A 00 00 00 00 00 00 23 FF
*/


/*
Messaggio display in Esadecimale D.....#
 
44 00 00 00 00 00 00 14 15 1A 00 00 00 00 00 00 23 FF
*/

/*
const char* ssid = "xxx";
const char* pass = "xxx";
*/

const char* ssid = "sss";
const char* pass = "ppp";


const char* server = "mqtt3.thingspeak.com";
const int mqttPort = 1883;

/*
https://thingspeak.com/channels/1612410
*/

const int channelID = 1612410; //Incubatrice

const char mqttUserName[] = "xxx";
const char clientID[] = "xxx";
const char mqttPass[] = "yyy";



WiFiClient client;
PubSubClient mqttClient( client );


enum stati {dormire,legge_seriale, analizza_dato, elaboramessaggio, invia_dati, mqtt,disconnectWifi};
enum stati stato = legge_seriale;
enum stati old_stato = legge_seriale;

typedef void (*GeneralFunction) (); // From https://www.gammon.com.au/callbacks 
GeneralFunction funzionestato [] =  { // array of function pointers
fdormire,
flegge_seriale,
fanalizza_dato,
felaboramessaggio,
finvia_dati,
fmqtt,
fdisconnectWifi
};
