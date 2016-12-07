// ESTE ALGORITMO EJECUTA LA MISMA FUNCION QUE EL ALGORITMO
// DE MATLAB varianza2.m
// SI DESEAN PROBAR EL FUNCIONAMIENTO DE varianza1.m ES
// NECESARIO REALIZAR PEQUEÑAS MODIFICACIONES
// NOTA: necesitan copiar la carpeta QueueArray a su carpeta
// de librerias de Arduino para que el codigo compile

/*
 * Calculo de varianza
 * Busca evitar efecto de cancelación cuando SumSqr y Sum*Sum
 * son muy similares. 
 * 
 * Se usa una propiedad de la varianza Var(X - k) = Var(X), 
 * donde k es una constante cualquiera. 
 * 
 * La nueva fórmula es: 
 * s^2 = (((sum i->n)(xi-K)^2)-((sum i->n)(xi-K)^2)/n)/(n-1)
 * 
 * Si K es cercana al promedio el resultado sera más preciso, 
 * pero escoger un valor dentro de los samples garantizará 
 * estabilidad. 
 * 
 * La compensasión de sumas se utiliza para combatir el error en
 * la acumulación de sumas, cuando n es un número grande. 
 */
 
/*  Python program to calculate the variance:
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
 *    
 *    
 *    
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

void setup() {
  Serial.begin (9600);
  queue_trial.setPrinter (Serial);
}

void loop() {
  float debug = random(2, 9);     // Generate a random number between 2 and 9.
  Serial.println(millis());
  entry(debug);
}

void entry(float debug_s) {
  if (number_count != window || number_count > window) { // Counter
    calcSum(debug_s);
    Serial.print("Fila: ");
    Serial.println(debug_s);
    queue_trial.enqueue(debug_s);  // Add to the queue.
    number_count += 1;             // Increase counter.
  }
  if (number_count >= window) {
    Variance();
    Serial.print("Varianza: ");
    Serial.println(K);
    Serial.print("Desviación Estándar: ");
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
  Serial.print("Numero agregado: ");
  Serial.println(a);
  x = queue_trial.dequeue();
  Serial.print("Numero eliminado: ");
  Serial.println(x);
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

