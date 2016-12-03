#include <QueueArray.h>


#define R1 2.2
#define percent 1.0
#define VBOOST 5.0
#define lapseTime 1000
#define nDato 3
#define cLimit 3
#define ignoreP 1.01
#define ignoreD 5
#define dataCenter 21
#define STOP 12

#define window 5
#define meanWindow 41
#define stdLimit 0.015
#define ratioLimit .98
#define WAIT 3000

int val1 = 0;

int analogPin = 3;     // potentiometer wiper (middle terminal) connected to analog pin 3
int ledPin = 9;
#define truLedPin 8

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
boolean firstSort = true;
boolean firstTime = true;
boolean ratioCheck = true;
boolean meanDelete = true;

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
  pinMode(truLedPin, OUTPUT);
  
  digitalWrite(STOP, HIGH);
  digitalWrite(ledPin, LOW);
  digitalWrite(truLedPin, LOW);
  
  medianQueue.setPrinter (Serial);
  Serial.println("INICIA PRUEBA");
  

}

void loop() {
  Serial.println(millis());
  digitalWrite(truLedPin, HIGH);
  delay(50);
  digitalWrite(truLedPin, LOW);
  Serial.println(millis());
  
  delay(4000);
  thisTime = millis();
  digitalWrite(ledPin, HIGH);
  val = analogRead(analogPin);
  
  if(val > 0)
  {
    digitalWrite(STOP, LOW);
    while((diffCount < cLimit) || ratioCheck)
    {
      
      val = analogRead(analogPin);
      Serial.print(thisTime);
      Serial.print(",");
      vout = 5*(val/1023.0);
  
      
      current = vout/R1;
      //vrms = (DC/255.0)*(VBOOST^2)/R1;
      
      
  
      vestoma = VBOOST - vout;
  
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
          diffCount++;
        }
        else
        {
          diffCount = 0;
        }
      }
      
      
      thisTime = millis();
    }

    initTime = millis();
    thisTime = millis();

    while((thisTime - initTime) < WAIT)
    {
      
      val = analogRead(analogPin);
      Serial.print(thisTime);
      Serial.print(",");
      vout = 5*(val/1023.0);
  
      
      current = vout/R1;
      //vrms = (DC/255.0)*(VBOOST^2)/R1;
      
      
  
      vestoma = VBOOST - vout;
  
      resistance = vestoma/current;
         
      Serial.print(current*1000,4);
      Serial.print(",");
      Serial.print(resistance,4);
      Serial.print(",");
      
      Serial.print(rInit,4);
      Serial.print(",");
      
      Serial.println(fMedian,4);
      entry(resistance);
  
      
      
      thisTime = millis();
    }
    digitalWrite(ledPin, LOW);
    digitalWrite(STOP, HIGH);
    
    Serial.println(millis());
    digitalWrite(truLedPin, HIGH);
    delay(50);
    digitalWrite(truLedPin, LOW);
    Serial.println(millis());
    
    Serial.println("TERMINADO");
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


