
// Need this for the lower level access to set them up.
#include <HardwareSerial.h>

//Define two Serial devices mapped to the two internal UARTs
HardwareSerial MySerial0(0);
//HardwareSerial MySerial1(1);
//ESP32C3 HW Serial 0
#define FONA_RX 8
#define FONA_TX 10



//#include <String.h>


void setup()
{
  Serial.begin(9600);    // the GPRS baud rate


  delay(1000);

  delay (2000);
  Serial.println(F("Initializzazione SIM800L...."));



  // Configure MySerial0 on pins TX=D6 and RX=D7 (-1, -1 means use the default)
  MySerial0.begin(9600, SERIAL_8N1, FONA_RX,  FONA_TX);
  //MySerial0.print("MySerial0");



  if (!MySerial0) { // If the object did not initialize, then its configuration is invalid
    Serial.println("Invalid HWSerial pin configuration, check config");
    while (1) { // Don't continue with invalid configuration
      delay (1000);
    }

  }
}

void loop()
{
  float h = 50.0;
  float t = 20.5;
  delay(100);

  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.println(" °C");
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.println(" %");


  /*

    if (MySerial0.available()) }
      Serial.write(MySerial0.read());
    }
  */

  MySerial0.println("AT");
  delay(1000);
  ShowSerialData();

  MySerial0.println("AT+CPIN?");
  delay(1000);
  ShowSerialData();

  MySerial0.print("AT+CFUN=1\r\n"); //Predispone la SIM per la funzionalità minima (AT*CFUN=0) per consumare meno energia
  delay(100);
  ShowSerialData();


  MySerial0.println("AT+CREG?");
  delay(1000);
  ShowSerialData();

  MySerial0.println("AT+CGATT?");
  delay(1000);
  ShowSerialData();

  MySerial0.println("AT+CIPSHUT");
  delay(1000);
  ShowSerialData();

  MySerial0.println("AT+CIPSTATUS");
  delay(2000);
  ShowSerialData();

  MySerial0.println("AT+CIPMUX=0");
  delay(2000);

  ShowSerialData();

  MySerial0.println("AT+CSTT=\"hologram\"");//start task and setting the APN,
  delay(1000);

  ShowSerialData();

  MySerial0.println("AT+CIICR");//bring up wireless connection
  delay(3000);

  ShowSerialData();

  MySerial0.println("AT+CIFSR");//get local IP adress
  delay(2000);

  ShowSerialData();

  MySerial0.println("AT+CIPSPRT=0");
  delay(3000);

  ShowSerialData();

  MySerial0.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);

  ShowSerialData();

  MySerial0.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();

  String str = "GET http://api.thingspeak.com/update?api_key=xxx&field3=" + String(t) + "&field4=" + String(h);
  Serial.println(str);
  MySerial0.println(str);//begin send data to remote server

  delay(4000);
  ShowSerialData();

  MySerial0.println((char)26);//sending CTLR+Z
  delay(5000);//waitting for reply, important! the time is base on the condition of internet
  MySerial0.println();

  ShowSerialData();

  MySerial0.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();

  MySerial0.print("AT+CFUN=0\r\n"); //Predispone la SIM per la funzionalità minima (AT*CFUN=0) per consumare meno energia
  delay(100);
  ShowSerialData();
  delay(30000);

}
void ShowSerialData()
{
  while (MySerial0.available() != 0)
    Serial.write(MySerial0.read());
  delay(5000);

}
