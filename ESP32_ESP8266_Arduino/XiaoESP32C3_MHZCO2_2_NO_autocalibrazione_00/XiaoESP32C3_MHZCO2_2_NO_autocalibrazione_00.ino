
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


void setup()
{
  Serial.begin(9600);

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

  Serial.println("Test sensori MHZ19B...");

  int status0 = MHZ19B0.measure();
  int status1 = MHZ19B1.measure();



Serial.println(status0);
Serial.println(status1);


if (status0 !=0 || status1 !=0){
  Serial.println("Riavvio ESP!!! Errore UART");
   delay(1000);
  ESP.restart();
  }
Serial.println("Elimina autocalibrazione");

  MHZ19B0.calibrateAuto(false);
  MHZ19B1.calibrateAuto(false);

}


void loop() {
 
}


//  -- END OF FILE --
