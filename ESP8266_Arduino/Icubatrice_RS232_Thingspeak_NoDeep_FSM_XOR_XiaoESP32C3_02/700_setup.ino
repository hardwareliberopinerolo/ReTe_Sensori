void setup() {
 Serial.begin(9600);

// Configure MySerial0 on pins TX=D6 and RX=D7 (-1, -1 means use the default) Xiao ESP32C3
MySerial0.begin(9600, SERIAL_8N1, -1, -1);

delay(100);
if (!MySerial0) { // If the object did not initialize, then its configuration is invalid
  Serial.println("Invalid HWSerial pin configuration, check config"); 
  while (1) { // Don't continue with invalid configuration
    delay (1000);
  }
}
Serial.println("OK HWSerial"); 
pinMode(D1, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

 delay(100);
 Serial.println("Incubatrice ESP32C3 - Thinkspeak"); 

    // Initialize display
    u8x8.begin();
    oled_clean();
    u8x8.setFont(u8x8_font_8x13B_1x2_r);
    u8x8.setCursor(0, 3);
    u8x8.print("INCUBATOR-RETE");

  tempo_serial=millis();

  MySerial0.flush();
  digitalWrite(D1, HIGH); //per set a 1
}
