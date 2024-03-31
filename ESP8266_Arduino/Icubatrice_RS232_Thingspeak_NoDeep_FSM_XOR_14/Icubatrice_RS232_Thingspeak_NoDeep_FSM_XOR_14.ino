#include <SoftwareSerial.h> // https://github.com/plerup/espsoftwareserial/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <WiFiClientSecure.h>


#define debug //Decommentare per debug

byte Messaggio[15];
byte buff[15];
int i = 0;
bool crk_status=0;
bool messaggio_status=0;
byte CRC=0x00;
const byte Chr_Inizio=0x40;
const byte Chr_CRC=0x23;
byte T_CRC=0x00;

#define MYPORT_TX 14 //Wemos D5
#define MYPORT_RX 12 //Wemos D6

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
Messaggio da iviare in Esadecimale @.....#CRC
 @  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 #  CRC
40 00 00 00 00 00 00 14 15 1A 00 00 00 00 00 00 23 78
*/


/*
Messaggio display in Esadecimale D.....#
 
44 00 00 00 00 00 00 14 15 1A 00 00 00 00 00 00 23 FF
*/

/*
const char* ssid = "xxx";
const char* pass = "xxx";
*/



const char* server = "mqtt3.thingspeak.com";
const int mqttPort = 1883;

/*
https://thingspeak.com/channels/1612410
*/

const int channelID = xxx; //Incubatrice

const char mqttUserName[] = "xxx";
const char clientID[] = "xxx";
const char mqttPass[] = "xxx";



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
