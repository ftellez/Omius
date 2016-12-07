/* Calculo de varianza
 * Busca evitar efecto de cancelación cuando SumSqr y Sum*Sum son muy similares. 
 * 
 * Se usa una propiedad de la varianza Var(X - k) = Var(X), donde k es una constante cualquiera. 
 * 
 * La nueva fórmula es: 
 * s^2 = (((sum i->n)(xi-K)^2)-((sum i->n)(xi-K)^2)/n)/(n-1)
 * 
 * Si K es cercana al promedio el resultado sera más preciso, pero escoger un valor dentro de los 
 * samples garantizará estabilidad. 
 * 
 * La compensasión de sumas se utiliza para combatir el error en la acumulación de sumas, cuando 
 * n es un número grande. 
 * 
 * Python program to calculate the variance:
 * 
 *    k = 0
 *    n = 0
 *    ex = 0
 *    ex2 = 0
 *    
 *    add_variable(x)
 *    if (n == 0)
 *      K = x
 *    n = n + 1
 *    ex = ex + x - K
 *    ex2 = ex2 + (x - K)*(x - K)
 *    
 *    remove_variable(x)
 *    n = n - 1
 *    ex = ex -  (x - K)
 *    ex2 = ex2 - (x - K)*(x - K)
 *    
 *    get_meanvalue(x)
 *    return K + ex / n
 *    
 *    get_variance(x) 
 *    return (ex2 - (ex * ex) / n) / (n - 1)
 */
#include <QueueArray.h>

QueueArray <float> queue_trial;   // Create queue object of type float.
int i = 0;
float K = 0;
float D = 0;
int squared = 0;
int sample = 0;       // sample
int window = 5;       // sample size
int number_count = 0;
boolean init2 = true;

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
  queue_trial.setPrinter (Serial);

  pinMode(VS,        INPUT);
  pinMode(analogPin, INPUT);
  pinMode(SW,        INPUT);

  pinMode(gate_Mosfet, OUTPUT);

  digitalWrite(gate_Mosfet, LOW); 
  
  Serial.println("CONFIG SUCCESSFUL");
  Serial.println("INICIA PRUEBA");
  Serial.println("Tiempo / Corriente / Voltaje / Resistencia / Switch / Varianza / Desv Stand");

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

    float debug = current*1000; 
    entry(debug); 

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
    Serial.print(expand);

    thisTime = millis();    // Register time end of loop.
  }
  digitalWrite(gate_Mosfet, LOW); 
}

void entry(float debug_s) {
  if (number_count != window || number_count > window) { // Counter
    calcSum(debug_s);
   // Serial.print("Fila: ");
   // Serial.println(debug_s);
    queue_trial.enqueue(debug_s);  // Add to the queue.
    number_count += 1;             // Increase counter.
  }
  if (number_count >= window) {
    Variance();
    //Serial.print("Varianza: ");
    Serial.print(",");
    Serial.print(K);
    //Serial.print("Desviación Estándar: ");
    Serial.print(",");
    Serial.println(D);
    dynamicQueue(debug_s);
    delay(1000);
  }
}

void Variance() {
  K = (squared - (sample * sample) / window) / (window - 1);
  D = sqrt(K);
}

void dynamicQueue(float a) {
  float x = 0;
  //Serial.print("Numero agregado: ");
  //Serial.println(a);
  x = queue_trial.dequeue();
  //Serial.print("Numero eliminado: ");
  //Serial.println(x);
  calcSubs(x);
  queue_trial.enqueue(a);
  calcSum(a);
}

void calcSum(float x) {
  squared += x * x;
  sample  += x;
}

void calcSubs(float x) {
  squared -= x * x;
  sample  -= x;
}

