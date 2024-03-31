/*Versione 21/12/22
  Non salva i dati su eprom se non trasmessi.
  Salva i dati in minuti
  Salva  temperatura ed umidita di due sensori DHT11 se collegati ai pin 4 e 5 (versione DHt11 con resistenza !)
  sava dati di 2 ingressi analogici su pin 6 e 7


  Legenda colori:
  All'accensione :
    Verde fisso = inizializzazione
    Verde fisso + rosso fisso = errore inizializzazione. Ritenta dopo 5 minuti
  Durante il funzionamento :
    Verde lampeggiante = loop di campionamento sensore
    Lampeggio rosso lento : tentativo invio dati
    Verde fisso = dati inviati
    Rosso fisso = errore di comunicazione con la scheda SIM800, o errore di rete o errore GPRS. Dati non inviati. (l'effetto è led lampeggiante rosso e verde)

Thinkspeak
https://thingspeak.com/channels/1612409

    
*/

#include <secTimer.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_FONA.h> //Modificare nella libreria l’APN inserendo quello corretto in base al gestore della SIM
//ATTENZIONE Modificare nella libreria Adafruit_FONA.cpp l'apn (riga : apn = F("TM") )inserendo il giusto corrispondende in base al gestore della SIM
//Per Things Mobile : TM
//Per Hologram : hologram

//-------Sensore temperatura

// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 4 //D4
#define TEMPERATURE_PRECISION 9
//#define TEMPERATURE_PRECISION 12

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Assign address manually. The addresses below will beed to be changed
// to valid device addresses on your bus. Device address can be retrieved
// by using either oneWire.search(deviceAddress) or individually via
// sensors.getAddress(deviceAddress, index)
DeviceAddress insideThermometer = { 0x28, 0xAA, 0xCD, 0xDB, 0x19, 0x13, 0x02, 0x17 };
DeviceAddress outsideThermometer   = { 0x28, 0xAA, 0x23, 0x4D, 0x1A, 0x13, 0x02, 0x63 };


/*
 28 AA CD DB 19 13 02 17 
 28 AA 23 4D 1A 13 02 63 
*/



//---------------------------------

//Dichiarazione di funzione che punta all'indirizzo zero
void(* Riavvia)(void) = 0; //Gestisce riavvio software se si blocca all'inizio (es. mattino per mancanza sole)

secTimer myTimer;

#define FONA_RX 9
#define FONA_TX 10
#define FONA_RST 2

#define FONA_APN             "hologram"  
#define FONA_USERNAME        ""  
#define FONA_PASSWORD        ""


SoftwareSerial SIM800ss = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA SIM800 = Adafruit_FONA(FONA_RST);

char http_cmd[160]; //allungato a 160 per leggere 2 temperatura ed umidita e 2 analogici
char atm_time_string[20];
char atm_time_string2[20]; //Aggiunto io
char atm_time_string3[20]; //temperarura 1
char atm_time_string4[20]; //umidita 1
char atm_time_string5[20]; //temperarura 2
char atm_time_string6[20]; //umidita 2
char atm_time_string7[20]; //analogico 1
char atm_time_string8[20]; //analogico 2
int net_status;

uint16_t statuscode; //intero 16 bit senza segno = unsigned short
int16_t length; //intero 16 bit positivo
String response = "";
char buffer[512];

boolean connection_on = false; //Variabile per testare la connessione col network
boolean gprs_on = false;
boolean tcp_on = false;

int sensor = 3; //logica del sensore inversa: quando la pompa è attiva il segnale logico sul pin 3 è BASSO!
int greenLED = 11;
int redLED = 12;
int i = 0;

byte state = 1;
byte sensorValue = 0;

unsigned long seconds = 0;
unsigned long durataAttiva = 0; //tempo in secondi di rilevazione corrente pinza amperometrica
unsigned long durataSpenta = 0; //tempo in secondi di inattività della pinza amperometrica
unsigned long tempoAttSend = 0;    //Variabili di backup per invio al sito (cosa servono ?)
unsigned long tempoMis = 0;        //variabile backup tempo accensione per invio al sito
unsigned long INVIOsecond = 0;     //Contatore per invio dati
int NFailure = 0; //Conteggia  i 'mancati invii'

//Temperatura e umidita-----

float t1 =0;
float t2 =0;

float temperatura1 = 0;        //variabile backup temperatura 1 per invio al sito
int umidita1 = 0;
float temperatura2 = 0;        //variabile backup temperatura  2 per invio al sito
int umidita2 = 0;

//sensori anagiloci---------------
int analog1 = 0;
int analog2 = 0;
//

/*----------------------------------------------------------------Valori modificabili----------------------------------------------*/
////ATTENZIONE Modificare eventualmente l'API KEY per inviare ad altro canale Thingspeak
//In basso l’indirizzo parziale del grafico Thingspeak che verrà successivamente completato in base alle informazioni proveniente dal sensore di hall.
//Modificato per scrivere solo nel campo 3 e 4
char url_string[] = "http://api.thingspeak.com/update?api_key=xxx&field3"; //URL del sito - non usare https ma http!!!

//int tempoInvioDati = 1800; //Variabile per modificare il tempo di invio dei dati (in secondi)(es. 3600 = invio dei dati ogni H)

int tempoInvioDati = 30*60; //Variabile per modificare il tempo di invio dei dati (in secondi)(es. 3600 = invio dei dati ogni H)

const unsigned long periodo = 1*60000;
unsigned long tiempoAnterior = 0;
