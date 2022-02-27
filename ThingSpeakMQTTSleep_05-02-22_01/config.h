
const char* ssid = "boyernet2";
const char* pass = "P10ssasc0\\";

/*
const char* ssid = "villaggioglobale";
const char* pass = "retewireless@193";
*/

/*
const char* ssid = "EOLO - FRITZ!Box 7430 ES";
const char* pass = "75388510789394626103";
*/



const char* server = "mqtt3.thingspeak.com";
const int mqttPort = 1883;

/*
https://thingspeak.com/channels/1578204
*/

/*
const int channelID = xxxxxxx; //Temperatura A
const char mqttUserName[] = "OT0aJgAaAxMwKRUfFBA6KCU"; //Temperatura A 
const char clientID[] = "OT0aJgAaAxMwKRUfFBA6KCU";     //Temperatura A
const char mqttPass[] = "+xIP+9CefGUBkcCbiNd3HWqR";     //Temperatura A
*/

const int channelID = 1612409; 


const char mqttUserName[] = "GxobHgMbOD0BMCYoIwIgLwk";
const char clientID[] = "GxobHgMbOD0BMCYoIwIgLwk";
const char mqttPass[] = "TJxVOTGkqaUMhufuhyxEEzKU";



int indSleepEE=40; //Indirizzo EEPROM minuti sleep

//I/O aggiornare i PIN in base alla scheda utilizzata
byte PIN_mod =        4; //Modifica parametri
byte PIN_alim_term = 12; //Alim termometri
byte PIN_term =      13; //Termometri.
