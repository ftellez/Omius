//SD library (Program -> Include Library -> Manage Libraries -> SD library)
// - Supports FAT16 and FAT32.
// - Uses 8.3 names for files. 
//      - Filename should be no longer than 8 characters. 
//      - Extension should be 3 (.TXT)
// - Uses SPI communication protocol (pins: 11, 12, 13 Arduino Uno).
// - Additionally uses Chip Select pin, to select the SD card (pin: 10 most Arduinos).
//      - If CS hardware pin is not selected, it should be left as output anyway. 
//--------------------------------------------------------------------------------------
//SPI library 
// - Synchronous protocol. 
// - Lines:
//      - MISO - line to send data to the master. 
//      - MOSI - master line to send data to the peripherals. 
//      - SCK  - serial clock. 
//      - CS   - chip select. 
// - Considerations
//      - Maximum SPI speed a device can use. 
//      - LSB or MSB shifting?
//      - Data idle at rising or falling edge?
//--------------------------------------------------------------------------------------
#include <SD.h>
#include <SPI.h>


// This code implements a datalogger. 
const int chipSelect = 10; 

void setup() {
  Serial.begin(9600); 
  while (!Serial);
  Serial.print("Init SD card..."); 
  if (!SD.begin(chipSelect)) {
    Serial.println("Init failed"); 
    return;
  }
  Serial.println("Init successful"); 
}

void loop() {
  String datalog = "";    // String to assemble data log.

  // Code to read specific sensor(s) to log (May vary). 
  int sensorX = analogRead(analogPin); 
  datalog += String(sensorX); 
  datalog += ",";
  
  // After reading sensor. Open file and save datalog. 
  File datalogFile = SD.open("datalog.txt", FILE_WRITE); 
  if (datalogFile) {
    datalogFile.println(datalog); // If file available, write to it. 
    datalogFile.close(); 
    Serial.println(datalog); 
  } else { Serial.println("error opening file"); }
}

