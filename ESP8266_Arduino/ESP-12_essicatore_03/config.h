const char* ssid = "sss";
const char* pass = "ppp";

const char* server = "mqtt3.thingspeak.com";
const int mqttPort = 1883;

const int channelID = 1612409; //Essicatore

const char mqttUserName[] = "xxx";
const char clientID[] = "xxx";
const char mqttPass[] = "yyy";

int indSleepEE=40; //Indirizzo EEPROM minuti sleep

//I/O aggiornare i PIN in base alla scheda utilizzata
#define DHTTYPE DHT22  //DHT 22  (AM2301) 
byte PIN_mod         = 5; //Ciclo/modifica parametri
byte PIN_ds          =12; //Termometro DS18B20
byte PIN_dht         =13; //Termometro/umidità DHT
byte PIN_alim_sensori=14; //Alimentazione sensori
byte PIN_ventole     = 4; //Rileva alimentazione ventole 
byte PIN_sw2         = 0; //Pulsante

float DHT_hum;    //DHT22 umidità.
float DHT_temp;   //DHT22 temperatura
float ds_temp_A;  //DS18B20 temperatura termometro A
float ds_temp_B;  //   "         "           "     B
byte ventole=0;


const unsigned long IDLE_TIME=60000;
