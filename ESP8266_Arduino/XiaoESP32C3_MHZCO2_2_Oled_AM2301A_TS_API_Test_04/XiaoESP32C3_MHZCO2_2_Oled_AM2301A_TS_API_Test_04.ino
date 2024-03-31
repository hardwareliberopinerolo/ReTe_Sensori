
#include <HardwareSerial.h>
//Define two Serial devices mapped to the two internal UARTs
HardwareSerial MySerial0(0);

#define TX0 D6
#define RX0 D7
/*
//test
#define TX0 D4
#define RX0 D5
*/

HardwareSerial MySerial1(1);

#define TX1 D4
#define RX1 D5
/*
//test
#define TX1 D6
#define RX1 D7
*/
//#include "Arduino.h"
#include "MHZCO2.h" //https://github.com/RobTillaart/MHZCO2


MHZ19B MHZ19B0;
MHZ19B MHZ19B1;

#include <Wire.h>        // Instantiate the Wire library
#include <U8x8lib.h> // https://github.com/olikraus/u8g2
#define DSDA D3
#define DSCL D2
//U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* clock=*/ DSCL, /* data=*/ DSDA, /* reset=*/ U8X8_PIN_NONE); //  Oled 1.3
U8X8_SH1106_128X64_NONAME_SW_I2C u8x8(/* clock=*/ DSCL, /* data=*/ DSDA, /* reset=*/ U8X8_PIN_NONE); //  Oled 1.3

// constants won't change. They're used here to set pin numbers:
#define buttonPin D1  // the number of the pushbutton pin
// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status
int tasto=0;

#include <dhtnew.h> //https://github.com/RobTillaart/DHTNew
#define DHT_Data D10
DHTNEW mySensor(DHT_Data);


#include <WiFi.h>
#include <ThingSpeak.h> // always include thingspeak header file after other header files and custom macros

#define SECRET_SSID "xxx"	// replace MySSID with your WiFi network name
#define SECRET_PASS "xxx"	// replace MyPassword with your WiFi password


#define SECRET_CH_ID xxx	// your channel number
#define SECRET_WRITE_APIKEY "xxx"   // replace XYZ with your channel write API Key


char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;


String myStatus = "";


#define period 120000
unsigned long time_now = 0;

const unsigned long IDLE_TIME=60000; // 1 min

void setup()
{
  Serial.begin(9600);
  Serial.println(__FILE__);
  Serial.print("MHZCO2_LIB_VERSION: ");
  Serial.println(MHZCO2_LIB_VERSION);


    Wire.begin(DSDA,DSCL);
        // Initialize display
  u8x8.begin();
  u8x8.setFont(u8x8_font_8x13B_1x2_f);
 delay(50);
 u8x8.clearLine(2);
 u8x8.setCursor(0, 2);
  u8x8.print("Test MHZ19B...");

// Configure MySerial0 on pins TX=D6 and RX=D7 (-1, -1 means use the default) Xiao ESP32C3
//MySerial0.begin(9600, SERIAL_8N1, -1, -1);


  delay(1000);

Serial.println("Avvio Sensori MHZ19B...");


  MHZ19B0.begin(&MySerial0);
  MHZ19B1.begin(&MySerial1);
    delay(1000);

// Configure MySerial0 on pins TX=D6 and RX=D7 (-1, -1 means use the default) Xiao ESP32C3
//MySerial0.begin(9600, SERIAL_8N1, -1, -1);
MySerial0.begin(9600, SERIAL_8N1, RX0, TX0);
MySerial1.begin(9600, SERIAL_8N1, RX1, TX1);

Serial.println("Ok!");
 delay(1000);

MySerial0.flush();
MySerial1.flush();


//Serial.println("Heating...3min...");
//delay(180000);


  Serial.println("Test sensori MHZ19B...");

  int status0 = MHZ19B0.measure();
  int status1 = MHZ19B1.measure();

Serial.println(status0);
Serial.println(status1);

    mySensor.read();

if (status0 !=0 || status1 !=0){
  Serial.println("Riavvio ESP!!! Errore UART");
   u8x8.clearLine(2);
 u8x8.setCursor(0, 2);
  u8x8.print("Riavvio ESP");
   delay(1000);
  ESP.restart();
  }

 u8x8.clearLine(2);
 u8x8.setCursor(0, 2);
  u8x8.print("MHZ19B OK");

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

delay(1000);
Serial.println("Ok!");

 WiFi.mode(WIFI_STA);

}


void loop()
{

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
   //++tasto;
   Serial.println("Calibrazione");
 u8x8.clearLine(2);
 u8x8.clearLine(3);
 u8x8.setCursor(0, 2);
 u8x8.print("Calibrazione");

 u8x8.clearLine(5);
  u8x8.clearLine(6);
 u8x8.setCursor(0, 5);
 u8x8.print("Calibrazione");

  MHZ19B0.calibrateZero();
  MHZ19B1.calibrateZero();

delay(60000);

}



if((unsigned long)(millis() - time_now) > period){
        time_now = millis();

  
  int status0 = MHZ19B0.measure();
  int status1 = MHZ19B1.measure();

Serial.println(status0);
Serial.println(status1);

    mySensor.read();

if (status0 !=0 || status1 !=0){
  Serial.println("Riavvio ESP!!! Errore UART");
   delay(1000);
  ESP.restart();
  }

else{
  Serial.print("CO2:  ");
  int CO2_0=MHZ19B0.getCO2();
  int CO2_1=MHZ19B1.getCO2();

  float Umidity= mySensor.getHumidity();
  float Temperature = mySensor.getTemperature();


  
  Serial.print(CO2_0);
  Serial.print("-");
  Serial.println(CO2_1);


 u8x8.clearLine(1);
  u8x8.clearLine(2);
 u8x8.setCursor(0, 1);
  u8x8.print(CO2_0);

 u8x8.clearLine(3);
  u8x8.clearLine(4);
 u8x8.setCursor(0, 3);
  u8x8.print(CO2_1);


 u8x8.clearLine(5);
  u8x8.clearLine(6);
 u8x8.setCursor(0, 5);
  u8x8.print(String (Temperature) + " - " +String (Umidity) );
 

    
  Serial.print("MCO2: ");
  Serial.print(MHZ19B0.getMinCO2());
     Serial.print("-");
    Serial.println(MHZ19B1.getMinCO2());
  Serial.print("Temp: ");
  Serial.print(MHZ19B0.getTemperature());
     Serial.print("-");
    Serial.println(MHZ19B1.getTemperature());
  Serial.print("Accu: ");
  Serial.print(MHZ19B0.getAccuracy());
     Serial.print("-");
    Serial.println(MHZ19B1.getAccuracy());

  Serial.print("T: ");
  Serial.print(Temperature);
     Serial.print(" U: ");
    Serial.println(Umidity);


 WiFi.begin( ssid, pass );
  //WiFi.begin();
  Serial.println("Connessione WiFi");
  unsigned long tempo=millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    if (millis() - tempo > IDLE_TIME){
      Serial.println("Connessione Fallita");
      ESP.restart();
      //delay(1000);
          //stato=dormire;
          break;
      }
      }




    
  ThingSpeak.begin(client);  // Initialize ThingSpeak
// set the fields with the values

  ThingSpeak.setField(5, CO2_0);
  ThingSpeak.setField(6, CO2_1);
  ThingSpeak.setField(7, Temperature);
  ThingSpeak.setField(8, Umidity);


  // figure out the status message

    myStatus = String("Prova API");
 
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

client.stop();
WiFi.disconnect();


}
}
}


//  -- END OF FILE --
