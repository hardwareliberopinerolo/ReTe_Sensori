// Need this for the lower level access to set them up.
#include <HardwareSerial.h>
//Define two Serial devices mapped to the two internal UARTs
HardwareSerial MySerial0(0);


#include "WiFi.h"
#include <PubSubClient.h>
//#include <WiFiClientSecure.h>


#include <U8x8lib.h> // https://github.com/olikraus/u8g2
// OLEDs without Reset of the Display
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); // Oled 0.96

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); //  Oled 1.3


#define debug //Decommentare per debug

byte Messaggio[20];
byte buff[20];
int i = 0;
bool crk_status=0;
bool messaggio_status=0;
byte CRC=0x00;
const byte Chr_Inizio=0x40;
const byte Chr_CRC=0x23;
byte T_CRC=0x00;

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

const char* ssid = "xxx";
const char* pass = "xx";


const char* server = "mqtt3.thingspeak.com";
const int mqttPort = 1883;


const int channelID = xxx; //Eligio_Ch1

const char mqttUserName[] = "xxxx";
const char clientID[] = "xxxx";
const char mqttPass[] = "xxx";

/*
const int channelID = 1612410; //Incubatrice

const char mqttUserName[] = "NQ0MNhUlMBILOjMnFAIaOjA";
const char clientID[] = "NQ0MNhUlMBILOjMnFAIaOjA";
const char mqttPass[] = "fkP1kEYSIyJoMMPjxLN2YGzK";
*/


WiFiClient client;
PubSubClient mqttClient( client );


enum stati {dormire,legge_seriale,invia_dati, mqtt,disconnectWifi};
enum stati stato = legge_seriale;
enum stati old_stato = legge_seriale;

typedef void (*GeneralFunction) (); // From https://www.gammon.com.au/callbacks 
GeneralFunction funzionestato [] =  { // array of function pointers
fdormire,
flegge_seriale,
//fanalizza_dato,
//felaboramessaggio,
finvia_dati,
fmqtt,
fdisconnectWifi
};
