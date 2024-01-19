#include <LiquidCrystal.h>
float temp;
int sensor = A2;
float tempc;
float tempf;
const int rs = 12, en = 9, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(12, 9, 5, 4, 3, 2);
int pulsePin = A1;
int blinkPin = 13;
volatile int BPM;
volatile int Signal
volatile int IBI = 600;
volatile boolean Pulse = false;
volatile boolean QS = false;
static boolean serialVisual = true;
volatile int rate[10]
volatile unsigned long sampleCounter = 0;
volatile unsigned long lastBeatTime = 0;
volatile int P = 512;
volatile int T = 512;
volatile int thresh = 525;
volatile int amp = 100;
volatile boolean firstBeat = true;
volatile boolean secondBeat = false;
void setup()
{
 Serial.begin(9600);
pinMode(10, INPUT);
pinMode(11, INPUT);
 pinMode(blinkPin,OUTPUT);
 Serial.begin(115200);
 interruptSetup();

lcd.begin(16, 2);
lcd.clear();
}
void loop()
{

 {
 temp=analogRead(sensor);
tempc=(temp*4.88)/10;
lcd.setCursor(0,0);
lcd.println(tempc);
delay(4000);
}
 serialOutput();

 if (QS == true)
{


 serialOutputWhenBeatHappens();
 QS = false;
 }

 delay(20);
}
void interruptSetup()
{

 TCCR2A = 0x02;
 TCCR2B = 0x06;
 OCR2A = 0X7C;
 TIMSK2 = 0x02;
 sei();
}
void serialOutput()
{
if (serialVisual == true)
 {
 arduinoSerialMonitorVisual('-', Signal);
 }
else
 {
 sendDataToSerial('S', Signal);
 }
}
void serialOutputWhenBeatHappens()
{
if (serialVisual == true)
 {
 Serial.print(" Heart-Beat Found ");
 Serial.print("BPM: ");
 Serial.println(BPM);
 lcd.setCursor(1,1);
 lcd.print("BPM: ");
 lcd.setCursor(5,1);
 lcd.print(BPM);
 delay(3000);
 lcd.clear();
 }
else
 {
 sendDataToSerial('B',BPM);
 sendDataToSerial('Q',IBI);
 }
}
void arduinoSerialMonitorVisual(char symbol, int data )
{
 const int sensorMin = 0;
 const int sensorMax = 1024;
 int sensorReading = data;
 int range = map(sensorReading, sensorMin, sensorMax, 0, 11);
}
void sendDataToSerial(char symbol, int data )
{
 Serial.print(symbol);
 Serial.println(data); 
}
ISR(TIMER2_COMPA_vect)
{
 cli();
 Signal = analogRead(pulsePin);
 sampleCounter += 2;
 int N = sampleCounter - lastBeatTime;
 if(Signal < thresh && N > (IBI/5)*3)
 {
 if (Signal < T)
 {
 T = Signal;
 }
 }
 if(Signal > thresh && Signal > P)
 { 
P = Signal;
 }
 if (N > 250)
 {
 if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) )
 {
 Pulse = true;
 digitalWrite(blinkPin,HIGH);
 IBI = sampleCounter - lastBeatTime;
 lastBeatTime = sampleCounter;

 if(secondBeat)
 { ,
if secondBeat == TRUE
 secondBeat = false; 

 {
 rate[i] = IBI;
 }
 }

 if(firstBeat)
if firstBeat == TRUE
 {
 firstBeat = false;
 secondBeat = true;
 sei();
 return;
 }

 word runningTotal = 0;
 for(int i=0; i<=8; i++)
 {
 rate[i] = rate[i+1];
 runningTotal += rate[i];
 }
 rate[9] = IBI;
 runningTotal += rate[9];
 runningTotal /= 10;
 BPM = 60000/runningTotal;
 QS = true;
 }
 }
 if (Signal < thresh && Pulse == true)
 {
digitalWrite(blinkPin,LOW);
 Pulse = false;
 amp = P - T;
 thresh = amp/2 + T;
 P = thresh;
 T = thresh;
 }
 if (N > 2500)
 {
 thresh = 512;
 P = 512;
 T = 512;
 lastBeatTime = sampleCounter;
 firstBeat = true;
 secondBeat = false;
 }
 sei();
}
void ecg()
{
if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
 Serial.println('!');
}
Else{
Serial.println(analogRead(A0));
}
delay(1);
}

