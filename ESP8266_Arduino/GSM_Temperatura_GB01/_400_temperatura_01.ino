

/*
// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

*/

void setup_temperatura()
{

  Serial.println(F("Dallas Temperature "));

  // Start up the library
  sensors.begin();


/*

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

 
  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(outsideThermometer);
  Serial.println();

  // set the resolution to 9 bit per device
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC);
  Serial.println();

  Serial.print("Device 1 Resolution: ");
  Serial.print(sensors.getResolution(outsideThermometer), DEC);
  Serial.println();

*/
  
}


/*
   Main function, calls the temperatures in a loop.
*/
void legge_temperatura()
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

/*
  // print the device information
  printData(insideThermometer);
  printData(outsideThermometer);
*/

t1 = (t1 + sensors.getTempC(insideThermometer));
delay(100);
t1 = (t1 + sensors.getTempC(insideThermometer));
delay(100);
t1 = (t1 + sensors.getTempC(insideThermometer))/4;
delay(100);

t2 = (t2 + sensors.getTempC(outsideThermometer));
delay(100);
t2 = (t2 + sensors.getTempC(outsideThermometer));
delay(100);
t2= (t2 + sensors.getTempC(outsideThermometer))/4;
delay(100);

Serial.print("Temperature: ");
Serial.print(t1);
Serial.print(" - ");
Serial.println(t2);

temperatura1 = t1; //Carico la temperatura1 in una variabile per invio dati
temperatura2 = t2; //Carico la temperatura2 in una variabile per invio dati
delay(1000);
}






void legge_temperatura_finta(){

//Sensore temperatura -------------------------------
    Serial.print(F("-----Legge temperatura -----"));

	randomSeed(244);

int	t1 = random(0,30);
int	t2 = random(0,30);
Serial.print(t1);
Serial.print(" - ");
Serial.println(t2);

      delay(2000);
      temperatura1 = t1; //Carico la temperatura1 in una variabile per invio dati
     // umidita1 = h1;
      temperatura2 = t2; //Carico la temperatura2 in una variabile per invio dati
     // umidita2 = h2;
    
}
