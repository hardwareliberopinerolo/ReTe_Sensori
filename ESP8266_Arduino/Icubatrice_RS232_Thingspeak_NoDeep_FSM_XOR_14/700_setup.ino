void setup() {
 Serial.begin(9600);
 //SWSerial.begin(9600, SWSERIAL_8N1, MYPORT_RX, MYPORT_TX, false, 256); 
 SWSerial.begin(9600, SWSERIAL_8N1, MYPORT_RX, MYPORT_TX, false); 
if (!SWSerial) { // If the object did not initialize, then its configuration is invalid
  Serial.println("Invalid SoftwareSerial pin configuration, check config"); 
  while (1) { // Don't continue with invalid configuration
    delay (1000);
  }
}

  // high speed half duplex, turn off interrupts during tx
  //SWSerial.enableIntTx(false);
 delay(500);
 Serial.println("Messaggio RS232"); 



  tempo_serial=millis();



}
