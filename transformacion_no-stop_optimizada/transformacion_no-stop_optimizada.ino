/*
#define percent 1.0
//#define VBOOST 3.5
#define nDato 3
#define cLimit 3
#define ignoreP 1.01
#define ignoreD 5
#define dataCenter 6
#define limPruebas 5
#define window 5
#define meanWindow 11
#define stdLimit 0.015
#define ratioLimit .98
#define WAIT 1000
#define DELAY 4000

int val1 = 0;
int nPruebas = 0;
int diffCount = 0;
int count2 = 0;
int countSel = 0;
int countInit = 0;
int number_count2 = 0;
int countDiff = 0;
float ratio = 1;
float rOld = 1.0;
float rInit = 0;
float Diff = 0;
float instMean = 0;
float fMedian = 1;
float minVal = 0;
float fTH = 0;

boolean firstSort = true;
boolean firstTime = true;
boolean firstTime2 = true;
boolean ratioCheck = true;
boolean ratioCheck2 = true;

boolean meanDelete = true;
boolean finish = false;

int i=0;
float K=1;
float D=1;
float squared=0;
float simple=0;

int number_count=0;
boolean init2 = true;
*/

// Variables & Definitions 
#define STOP 12           // Stop not used.
#define PINEND 11         // Pin End not used.
#define CLOSE 8           // Close not used. 
#define CAPTURE 7         // Capture not used. 
#define VIDEOEND 6        // Videoend not used.         

#define VS A0             // Voltage Source. 
#define analogPin A3      // Potentiometer wiper (middle terminal) connected to analog pin 3.
#define SW 2              // Switch pin. 
#define R1 2.2            // Resistor 2.2Ohms
#define lapseTime 5000    // DURACION DEL PULSO EN milisegundos  

int ledPin = 9;           // LED pin on mosfet gate. 


float thisTime = 0;       // Current program timing. 
float initTime = 0;       // Initial time.
bool checkTime = true;    // Check time variable. 

// Electrincal values. 
int val = 0;              // Variable to store read value. 
float current = 0;
float vout = 0;
float vestoma = 0;
float VBOOST = 0;
float resistance = 1.0;

void setup() {
  Serial.begin(9600);         // Configure Baud Rate. 
  
  pinMode(ledPin,   OUTPUT);
  pinMode(STOP,     OUTPUT);
  pinMode(PINEND,   OUTPUT);
  pinMode(CLOSE,    OUTPUT);
  
  pinMode(VS,        INPUT);
  pinMode(analogPin, INPUT);
  pinMode(SW,        INPUT);
  pinMode(CAPTURE,   INPUT);
  pinMode(VIDEOEND,  INPUT);
  
  digitalWrite(PINEND, LOW);
  digitalWrite(STOP,   LOW);
  digitalWrite(ledPin, LOW);
  digitalWrite(CLOSE,  LOW);
 
  Serial.println("CONFIG SUCCESSFUL");

}

void loop() {
  delay(4000);                  // Wait four seconds.
  thisTime = millis();          // Capture time at the begining of the loop -- ? not needed. 
  initTime = millis();          // -- ? not needed. 
  
  val = analogRead(VS);         // Read voltage source. 
  VBOOST = 5.0*(val/1023.0);    // Measure boost voltage.  
  
  checkTime = true;             // -- ? not needed. 
  
  val = analogRead(analogPin);  // Read resistor variation.
  
  {
    Serial.println("INICIA PRUEBA");
    Serial.println("Tiempo / Corriente / Voltaje / Resistencia / Switch");
 
    digitalWrite(PINEND, HIGH);   // -- ? not sure. 
    
    initTime = millis();          // Register init time. 
    digitalWrite(ledPin, HIGH);     
    digitalWrite(STOP, LOW);      // Disable stop. 
    
    while(true) {
      //1. Register time. 
      Serial.print(thisTime);
      Serial.print(",");

      //2. Current. 
      val = analogRead(analogPin);
      vout = 5.0*(val/1023.0);
      current = vout/R1;
      Serial.print(current*1000,4);
      Serial.print(",");

      //3. Resistance. 
      resistance = vestoma/current;
      Serial.print(resistance,4);
      Serial.print(",");
      
      //4. Voltage. 
      //vrms = (DC/255.0)*(VBOOST^2)/R1;
      vestoma = VBOOST - vout;
      Serial.print(VBOOST);
      Serial.print(",");

      //5. Switch. 
      int expand = digitalRead(SW); // Read switch. 
      Serial.println(expand);
            
      thisTime = millis();    // Register time end of loop. 
      checkTime = ((thisTime - initTime) < lapseTime);
    }
    digitalWrite(ledPin, LOW);
    digitalWrite(PINEND, LOW);
    Serial.println("TERMINADO");
  }
}


