// ESTE ALGORITMO EJECUTA LA MISMA FUNCION QUE EL ALGORITMO
// DE MATLAB varianza2.m
// SI DESEAN PROBAR EL FUNCIONAMIENTO DE varianza1.m ES 
// NECESARIO REALIZAR PEQUEÑAS MODIFICACIONES

// NOTA: necesitan copiar la carpeta QueueArray a su carpeta
// de librerias de Arduino para que el codigo compile

#include <QueueArray.h>

QueueArray <float> queue_trial;
int i=0;
float K=0;
float D=0;
int squared=0;
int simple=0;
int window=5;
int number_count=0;
boolean init2 = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  queue_trial.setPrinter (Serial);

}

void loop() {
  // put your main code here, to run repeatedly:
  float debug=random(2,9);
  Serial.println(millis());
  entry(debug);
    
}

  void entry(float debug_s){
  if(number_count!=window||number_count>window){
    calcSum(debug_s);
    Serial.print("Fila: ");
    Serial.println(debug_s);
    queue_trial.enqueue(debug_s);
    number_count+=1;
    }


    if(number_count>=window){
      Variance();
      Serial.print("Varianza: ");
      Serial.println(K);
      Serial.print("Desviación Estándar: ");
      Serial.println(D);
      dynamicQueue(debug_s);
      delay(1000);
    }
  }


void Variance(){
     K= (squared - (simple*simple)/window)/(window-1);
     //D= sqrt(K); 
  }

void dynamicQueue(float a){
  float x=0;
  Serial.print("Numero agregado: ");
  Serial.println(a);

 
  x=queue_trial.dequeue();
  Serial.print("Numero eliminado: ");
  Serial.println(x);
  calcSubs(x);
  
  queue_trial.enqueue(a);
  calcSum(a);
  }

void calcSum(float x){
    
    squared+=x*x;
    simple+=x;
  }

void calcSubs(float x){
    squared-=x*x;
    simple-=x;
  }

