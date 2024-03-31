#include <Adafruit_FONA.h> //Modificare nella libreria l’APN inserendo quello corretto in base al gestore della SIM
//ATTENZIONE Modificare nella libreria Adafruit_FONA.cpp l'apn (riga : apn = F("TM") )inserendo il giusto corrispondende in base al gestore della SIM
//Per Things Mobile : TM
//Per Hologram : hologram


//ESP32C3 HW Serial 0
#define FONA_RX 8
#define FONA_TX 10

#define FONA_RST 2



#define FONA_APN             "hologram"  
#define FONA_USERNAME        ""  
#define FONA_PASSWORD        ""


// Need this for the lower level access to set them up.
#include <HardwareSerial.h>

//Define two Serial devices mapped to the two internal UARTs
HardwareSerial MySerial0(0);
//HardwareSerial MySerial1(1);



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
char url_string[] = "http://api.thingspeak.com/update?api_key=xxxx&field3"; //URL del sito - non usare https ma http!!!

//int tempoInvioDati = 1800; //Variabile per modificare il tempo di invio dei dati (in secondi)(es. 3600 = invio dei dati ogni H)

int tempoInvioDati = 30*60; //Variabile per modificare il tempo di invio dei dati (in secondi)(es. 3600 = invio dei dati ogni H)

const unsigned long IntervalloInvioDati =1*60000; // minuti invio dati
unsigned long tempo=0;
unsigned long old_tempo=0;


const unsigned long periodo = 1*60000;
unsigned long tiempoAnterior = 0;
