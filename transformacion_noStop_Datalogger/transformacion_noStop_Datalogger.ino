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

// Variables & Definitions
#define VS A0             // Voltage Source. 
#define analogPin A3      // Potentiometer wiper (middle terminal) connected to analog pin 3.
#define SW 2              // Switch pin. 
#define R1 2.2            // Resistor 2.2Ohms

bool chkVoltage = true;   // For checking source voltage.
float thisTime = 0;       // Current program timing.

// Electrincal values.
int val = 0;              // Variable to store read value.
int gate_Mosfet = 9;
float current = 0;
float vout = 0;
float vestoma = 0;
float VBOOST = 0;
float resistance = 1.0;
const int chipSelect = 10; // Check arduino hardware for SS pin.

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(VS,        INPUT);
  pinMode(analogPin, INPUT);
  pinMode(SW,        INPUT);

  pinMode(gate_Mosfet, OUTPUT);

  digitalWrite(gate_Mosfet, LOW);

  Serial.println("Init SD card..."); // Initialize SD card.
  if (!SD.begin(chipSelect)) { Serial.println("Init failed"); return; }
  Serial.println("Init successful");

  Serial.println("Starting test...");
  Serial.println("Tiempo / Corriente / Voltaje / Resistencia / Switch");
}

void loop() {

  String datalog = "";

  val = analogRead(VS);         // Read voltage source.
  VBOOST = 5.0 * (val / 1023.0); // Measure boost voltage.
  chkVoltage = VBOOST > 2.1;

  digitalWrite(gate_Mosfet, HIGH);
  
  while (chkVoltage) {
    val = analogRead(VS);         // Read voltage source.
    VBOOST = 5.0 * (val / 1023.0); // Measure boost voltage.
    chkVoltage = VBOOST > 2.1;

    //1. Register time.
    datalog += (String) thisTime + ",";
    //Serial.print(thisTime);
    //Serial.print(",");

    //2. Current.
    val = analogRead(analogPin);
    vout = 5.0 * (val / 1023.0);
    current = vout * 1000 / R1;
    datalog += (String) current +  ","; // Not sure if it will output decimal values.
    //Serial.print(current * 1000, 4);
    //Serial.print(",");

    vestoma = VBOOST - vout;

    //3. Resistance.
    resistance = vestoma / current;
    datalog += (String) resistance + ",";
    //Serial.print(resistance, 4);
    //Serial.print(",");

    //4. Voltage.
    datalog += (String) VBOOST + ",";
    //Serial.print(VBOOST); // Voltage source.
    //Serial.print(",");

    //5. Switch.
    int expand = digitalRead(SW); // Read switch.
    datalog += (String) expand + ",";
    //Serial.println(expand);

    thisTime = millis();    // Register time end of loop.
  }
  digitalWrite(gate_Mosfet, LOW);

  // Once the string is complete. Create a file to store the string. 
  File datalogFile = SD.open("datalog.txt", FILE_WRITE); 
  if (datalogFile) {
    datalogFile.println(datalog); 
    datalogFile.close(); 
    Serial.println(datalog); 
  } else { Serial.println("error opening file"); }  
}

