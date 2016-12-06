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

void setup() {
  Serial.begin(9600);         // Configure Baud Rate.

  pinMode(VS,        INPUT);
  pinMode(analogPin, INPUT);
  pinMode(SW,        INPUT);

  pinMode(gate_Mosfet, OUTPUT);

  digitalWrite(gate_Mosfet, LOW); 
  
  Serial.println("CONFIG SUCCESSFUL");
  Serial.println("INICIA PRUEBA");
  Serial.println("Tiempo / Corriente / Voltaje / Resistencia / Switch");

}

void loop() {
  val = analogRead(VS);         // Read voltage source.
  VBOOST = 5.0 * (val / 1023.0); // Measure boost voltage.
  chkVoltage = VBOOST > 2.1;

  digitalWrite(gate_Mosfet, HIGH); 
  
  while (chkVoltage) {
    val = analogRead(VS);         // Read voltage source.
    VBOOST = 5.0 * (val / 1023.0); // Measure boost voltage.
    chkVoltage = VBOOST > 2.1;

    //1. Register time.
    Serial.print(thisTime);
    Serial.print(",");

    //2. Current.
    val = analogRead(analogPin);
    vout = 5.0 * (val / 1023.0);
    current = vout / R1;
    Serial.print(current * 1000, 4);
    Serial.print(",");

    vestoma = VBOOST - vout;

    //3. Resistance.
    resistance = vestoma / current;
    Serial.print(resistance, 4);
    Serial.print(",");

    //4. Voltage.
    Serial.print(VBOOST); // Voltage source.
    Serial.print(",");

    //5. Switch.
    int expand = digitalRead(SW); // Read switch.
    Serial.println(expand);

    thisTime = millis();    // Register time end of loop.
  }
  digitalWrite(gate_Mosfet, LOW); 
}


