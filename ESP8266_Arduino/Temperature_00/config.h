
const char* ssid = "sss";
const char* pass = "ppp";


const char* server = "mqtt3.thingspeak.com";
const int mqttPort = 1883;

/*
https://thingspeak.com/channels/1612409
*/


const int channelID = 1612409; 


const char mqttUserName[] = "xxx";
const char clientID[] = "xxx";
const char mqttPass[] = "yyy";



int indSleepEE=40; //Indirizzo EEPROM minuti sleep

//I/O aggiornare i PIN in base alla scheda utilizzata
byte PIN_mod =        4; //Modifica parametri
byte PIN_alim_term = 12; //Alim termometri
byte PIN_term =      13; //Termometri.
