#include <Wire.h>        // Instantiate the Wire library
#include <U8x8lib.h> // https://github.com/olikraus/u8g2
#define DSDA D4
#define DSCL D5
//U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* clock=*/ DSCL, /* data=*/ DSDA, /* reset=*/ U8X8_PIN_NONE); //  Oled 1.3
U8X8_SH1106_128X64_NONAME_SW_I2C u8x8(/* clock=*/ DSCL, /* data=*/ DSDA, /* reset=*/ U8X8_PIN_NONE); //  Oled 1.3



// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h> // https://github.com/milesburton/Arduino-Temperature-Control-Library

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS  D10
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
DeviceAddress insideThermometer = { 0x28, 0x24, 0xD2, 0xB5, 0x04, 0x00, 0x00, 0x27  };
DeviceAddress outsideThermometer   = { 0x28, 0xFF, 0x64, 0x1E, 0x01, 0xAC, 0x26, 0x9D };


#include <WiFi.h>
#include <ThingSpeak.h> // always include thingspeak header file after other header files and custom macros

#define SECRET_SSID "xxx"	// replace MySSID with your WiFi network name
#define SECRET_PASS "xxx"	// replace MyPassword with your WiFi password


#define SECRET_CH_ID 1578204	// your channel number
#define SECRET_WRITE_APIKEY "xxxx"   // replace XYZ with your channel write API Key


char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;


String myStatus = "";


//#define period 120000 //2 min
//#define period 15000 //15 sec
#define period 300000  //5 min

unsigned long time_now = 0;

const unsigned long IDLE_TIME=60000; // 1 min

void setup()
{
  time_now = millis(); 
  Serial.begin(9600);
Serial.println("Avvio OLED...");
    Wire.begin(DSDA,DSCL);
        // Initialize display
  u8x8.begin();
  u8x8.setFont(u8x8_font_8x13B_1x2_f);
 delay(50);
 u8x8.clearLine(2);
 u8x8.clearLine(3);
 u8x8.setCursor(0, 2);
 u8x8.print("DS18B20 setup..");
Serial.println("Avvio Sensori DS18B20...");
  // Start up the library
  sensors.begin();

 u8x8.clearLine(2);
 u8x8.clearLine(3);
 u8x8.setCursor(0, 2);
  u8x8.print("DS18B20 OK");
Serial.println("DS18B20 Ok!");

 WiFi.mode(WIFI_STA);

}


void loop()
{


if((unsigned long)(millis() - time_now) > period){


 // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
float t1 = sensors.getTempC(insideThermometer);
delay(100);
float t2 = sensors.getTempC(outsideThermometer);
delay(100);

Serial.print("Temeperature: ");
Serial.print(t1);
Serial.print(" - ");
Serial.println(t2);

 u8x8.clearLine(1);
  u8x8.clearLine(2);
 u8x8.setCursor(0, 1);
  u8x8.print(t1);

 u8x8.clearLine(3);
  u8x8.clearLine(4);
 u8x8.setCursor(0, 3);
  u8x8.print(t2);

/*
 u8x8.clearLine(5);
  u8x8.clearLine(6);
 u8x8.setCursor(0, 5);
  u8x8.print(String (Temperature) + " - " +String (Umidity) );
 */



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

  ThingSpeak.setField(1,t1 );
  ThingSpeak.setField(2, t2 );
  //ThingSpeak.setField(3, );
  //ThingSpeak.setField(4, );


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
 time_now = millis();

}
}



//  -- END OF FILE --
