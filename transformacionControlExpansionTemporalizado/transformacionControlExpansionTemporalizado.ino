#include <QueueArray.h>

#define PINEND 11


#define R1 2.2
#define percent 1.0
#define VBOOST 3.0
#define lapseTime 25000
#define nDato 3
#define cLimit 3
#define ignoreP 1.01
#define ignoreD 5
#define dataCenter 6
#define STOP 12

#define limPruebas 5

#define window 5
#define meanWindow 11
#define stdLimit 0.015
#define ratioLimit .98
#define WAIT 1000
#define DELAY 4000

int val1 = 0;

int analogPin = 3;     // potentiometer wiper (middle terminal) connected to analog pin 3
int ledPin = 9;
int nPruebas = 0;
#define CLOSE 8

#define CAPTURE 7
#define VIDEOEND 6

int val = 0;           // variable to store the value read
float current = 0;
float vout = 0;
float vestoma = 0;
int diffCount = 0;
int count2 = 0;
int countSel = 0;
int countInit = 0;
int number_count2 = 0;
int countDiff = 0;
float ratio = 1;
float resistance = 1.0;
float rOld = 1.0;
float thisTime = 0;
float initTime = 0;
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
boolean checkTime = true;
boolean meanDelete = true;
boolean finish = false;

QueueArray <float> rawQueue;
QueueArray <float> copyQueue;
QueueArray <float> sortedQueue;
QueueArray <float> tempQueue;
QueueArray <float> medianQueue;

int i=0;
float K=1;
float D=1;
float squared=0;
float simple=0;

int number_count=0;
boolean init2 = true;

void setup() {
  Serial.begin(9600);          //  setup serial
  pinMode(ledPin, OUTPUT);
  pinMode(STOP, OUTPUT);
  pinMode(PINEND, OUTPUT);
  pinMode(CLOSE, OUTPUT);

  pinMode(CAPTURE, INPUT);
  pinMode(VIDEOEND, INPUT);

  digitalWrite(PINEND, LOW);
  digitalWrite(STOP, LOW);
  digitalWrite(ledPin, LOW);
  digitalWrite(CLOSE, LOW);
  
  medianQueue.setPrinter (Serial);
  Serial.println("INICIA PRUEBA");
  

}

void loop() {
  
  delay(4000);
  thisTime = millis();
  initTime = millis();

  ratioCheck = true;
  ratioCheck2 = true;
  checkTime = true;
  
  val = analogRead(analogPin);
  
  
  
  {
    Serial.println("INICIA PRUEBA");
    digitalWrite(PINEND, HIGH);
    initTime = millis();
    digitalWrite(ledPin, HIGH);
    digitalWrite(STOP, LOW);
    while(checkTime)
    {
      
      val = analogRead(analogPin);
      Serial.print(thisTime);
      Serial.print(",");
      vout = 5*(val/1023.0);
  
      
      current = vout/R1;
      //vrms = (DC/255.0)*(VBOOST^2)/R1;
      
      
  
      vestoma = VBOOST;
  
      resistance = vestoma/current;
      
      Serial.print(current*1000,4);
      Serial.print(",");
      Serial.print(resistance,4);
      Serial.print(",");

      
      
  
      
      if(firstTime)
      {
        if(countInit < ignoreD)
        {
          countInit++;
        }
        else
        {
          rOld = resistance;
          rInit = instMean;
          ratio = 1.0;
          firstTime = false;
        }
        
      }
      
      
      
      
      Serial.print(rInit,4);
      Serial.print(",");
      Serial.print(fTH,4);
      Serial.print(",");
      Serial.println(fMedian,4);
      
      entry(resistance);

      
      if(fMedian >= rInit)
      {
        rInit = fMedian;
      }
      ratio = fMedian/rInit;


      
      if(ratioCheck)
      {
        ratioCheck = (ratio > ratioLimit);
      }
      else
      {
        
        
        if(Diff < 0)
        {
          if(firstTime2)
          {
            fTH = minVal + ((rInit - minVal)*0.20);
            firstTime2 = false;
          }
          ratioCheck2 = (fMedian < fTH);
        }
        else
        {
          diffCount = 0;
        }
        
      }
      
      
      thisTime = millis();

      checkTime = ((thisTime - initTime) < lapseTime);
    }

    
    
/*
    while((thisTime - initTime) < WAIT)
    {
      
      val = analogRead(analogPin);
      Serial.print(thisTime);
      Serial.print(",");
      vout = 5*(val/1023.0);
  
      
      current = vout;
      //vrms = (DC/255.0)*(VBOOST^2)/R1;
      
      
  
      vestoma = VBOOST;
  
      resistance = vestoma/current;
         
      Serial.print(current*1000,4);
      Serial.print(",");
      Serial.print(resistance,4);
      Serial.print(",");
      
      Serial.print(rInit,4);
      Serial.print(",");
      Serial.print(fTH,4);
      Serial.print(",");
      
      Serial.println(fMedian,4);
      
  
      
      
      thisTime = millis();
    }
    */
    Serial.print(thisTime);
    Serial.print(",");
    Serial.print(current*1000,4);
    Serial.print(",");
    Serial.print(resistance,4);
    Serial.print(",");
    
    Serial.print(rInit,4);
    Serial.print(",");
    Serial.print(fTH,4);
    Serial.print(",");
    
    Serial.println(fMedian,4);
      
    digitalWrite(ledPin, LOW);
    
    digitalWrite(PINEND, LOW);
    
    Serial.println("TERMINADO");
    nPruebas++;

    delay(10000);
    
    thisTime = millis();
    initTime = millis();
    while((thisTime - initTime) < 2500)
    {
      digitalWrite(CLOSE, HIGH);
      thisTime = millis();
    }
    digitalWrite(CLOSE, LOW);
    delay(20000);

    reiniciarVariables();
    limpiarQueue();
  }

  while(nPruebas >= limPruebas)
  {
    digitalWrite(STOP, HIGH);
  }
  

}

void entry(float debug_s){
  int number_count = medianQueue.count();
  
  if(number_count != window || number_count > window){
    if(rawQueue.count() != meanWindow)
    {
      rawQueue.enqueue(debug_s);
      copyQueue.enqueue(debug_s);
      
    }
    else
    {
      
      if(firstSort)
      {
        queueSave(debug_s);
        copyQueue.dequeue();
        copyQueue.enqueue(debug_s);
        firstSorting();
        firstSort = false;
      }
      else
      {
        removeOld();
        addNew(debug_s);
        findMedian();
      }
    }
    medianQueue.enqueue(fMedian);
  }

  if(number_count>=window){
    if(rawQueue.count() != meanWindow)
    {
      rawQueue.enqueue(debug_s);
      copyQueue.enqueue(debug_s);
      
    }
    else
    {
      
      if(firstSort)
      {
        queueSave(debug_s);
        copyQueue.dequeue();
        copyQueue.enqueue(debug_s);
        firstSorting();
        firstSort = false;
      }
      else
      {
        removeOld();
        addNew(debug_s);
      }
    }
    dynamicQueue(debug_s);
  }
}

void queueSave(float a)
{
  float x=0;

  x=rawQueue.dequeue();
  rawQueue.enqueue(a);
}

void dynamicQueue(float a){
  float x=0;

  findMedian();
  x = medianQueue.dequeue();
  medianQueue.enqueue(fMedian);

  Diff = x - fMedian;
  minVal = x;
  countDiff++;
  }



void firstSorting()
{
  float x1 = 0;
  float x2 = 0;
  float w1 = 0;
  float w2 = 0;

  x1 = copyQueue.dequeue();
  tempQueue.enqueue(x1);

  while(!copyQueue.isEmpty())
  {
    x2 = copyQueue.dequeue();
    while(!tempQueue.isEmpty())
    {
      x1 = tempQueue.dequeue();
      if(x1 < x2)
      {
        w1 = x1;
        w2 = x2;
      }
      else
      {
        w1 = x2;
        x2 = x1;
      }
      sortedQueue.enqueue(w1);
    }
    sortedQueue.enqueue(x2);
    while(!sortedQueue.isEmpty())
    {
      tempQueue.enqueue(sortedQueue.dequeue());
    }
  }
  while(!tempQueue.isEmpty())
  {
    sortedQueue.enqueue(tempQueue.dequeue());
  }
}

void removeOld()
{
  float x1 = 0;
  float x2 = 0;
  float w1 = 0;
  boolean firstMatch = false;

  x1 = rawQueue.dequeue();

  while(!sortedQueue.isEmpty())
  {
    tempQueue.enqueue(sortedQueue.dequeue());
  }

  while(!tempQueue.isEmpty())
  {
    x2 = tempQueue.dequeue();
    if(x1 != x2)
    {
      sortedQueue.enqueue(x2);
    }
    else
    {
      if(firstMatch)
      {
        sortedQueue.enqueue(x2);
      }
      else
      {
        firstMatch = true;
      }
    }
  }
}

void addNew(float newData)
{
  float x1 = 0;
  float x2 = 0;
  float w1 = 0;
  boolean sorted = false;

  rawQueue.enqueue(newData);

  while(!sortedQueue.isEmpty())
  {
    tempQueue.enqueue(sortedQueue.dequeue());
  }

  while(!tempQueue.isEmpty())
  {
    x2 = tempQueue.dequeue();
    if(sorted)
    {
      sortedQueue.enqueue(x2);
    }
    else
    {
      if(newData <= x2)
      {
        sortedQueue.enqueue(newData);
        sorted = true;
      }
      sortedQueue.enqueue(x2);
      
    }
    
  }
  if(!sorted)
  {
    sortedQueue.enqueue(newData);
  }
}

void findMedian()
{
  float x1 = 0;
  int countMedian = 0;
  while(!sortedQueue.isEmpty())
  {
    x1 = sortedQueue.dequeue();
    
    countMedian++;
    if(countMedian == dataCenter)
    {
      fMedian = x1;
    }
    tempQueue.enqueue(x1);
  }
  
  while(!tempQueue.isEmpty())
  {
    sortedQueue.enqueue(tempQueue.dequeue());
  }
}

void reiniciarVariables()
{
   val = 0;           // variable to store the value read
   current = 0;
   vout = 0;
   vestoma = 0;
   diffCount = 0;
   count2 = 0;
   countSel = 0;
   countInit = 0;
   number_count2 = 0;
   countDiff = 0;
   ratio = 1;
   resistance = 1.0;
   rOld = 1.0;
   thisTime = 0;
   initTime = 0;
   rInit = 0;
   Diff = 0;
   instMean = 0;
   fMedian = 1;
   minVal = 0;
   fTH = 0;
  
   firstSort = true;
   firstTime = true;
   firstTime2 = true;
   ratioCheck = true;
   ratioCheck2 = true;
   meanDelete = true;
}

void limpiarQueue()
{

  while(!rawQueue.isEmpty())
  {
    rawQueue.pop();
  }
  while(!copyQueue.isEmpty())
  {
    copyQueue.pop();
  }
  while(!sortedQueue.isEmpty())
  {
    sortedQueue.pop();
  }
  while(!tempQueue.isEmpty())
  {
    tempQueue.pop();
  }
  while(!medianQueue.isEmpty())
  {
    medianQueue.pop();
  }
}
