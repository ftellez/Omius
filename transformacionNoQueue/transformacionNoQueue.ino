

#define PINEND 11


#define R1 2.2
#define percent 1.0
#define VBOOST 3.0
#define lapseTime 25000       // DURACION DEL PULSO EN milisegundos
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
      Serial.println(resistance,4);
      
      
      
      
      thisTime = millis();

      checkTime = ((thisTime - initTime) < lapseTime);
    }
    
    Serial.print(thisTime);
    Serial.print(",");
    Serial.print(current*1000,4);
    Serial.print(",");
    Serial.println(resistance,4);
      
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
  }

  while(nPruebas >= limPruebas)
  {
    digitalWrite(STOP, HIGH);
  }
  

}


