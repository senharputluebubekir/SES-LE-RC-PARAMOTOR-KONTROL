#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
#include <Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myesc;


VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.

uint8_t records[7]; // save record
uint8_t buf[64];


int swch=0;
int esc=0;

int pot=0;
int mod=0;


int sw=8;
#define sag        (0)
#define sol        (1) 
#define yukari     (2) 
#define asagi      (3)
#define duzgit     (4)
#define dur        (5) 
#define basla      (6) 


void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
  
pot=0;

myesc.write(esc);
pinMode(sw,INPUT);
  
myservo1.attach(9); // attaches the servo on pin 9 to the servo object
myservo2.attach(10);
myservo3.attach(6);
myesc.attach(5);

  /** initialize */
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  
delay(100);     //100 ms bekleme

  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
delay(100);     //100 ms bekleme
 
  if(myVR.load((uint8_t)sag) >= 0){
    Serial.println("sag");
  }
  delay(100);     //100 ms bekleme
  
  if(myVR.load((uint8_t)sol) >= 0){
    Serial.println("sol");
  }
  delay(100);     //100 ms bekleme
  
  if(myVR.load((uint8_t)yukari) >= 0){
    Serial.println("yukari");
  }
  delay(100);     //100 ms bekleme

  if(myVR.load((uint8_t)asagi) >= 0){
    Serial.println("asagi");
  }
delay(100);     //100 ms bekleme

   if(myVR.load((uint8_t)duzgit) >= 0){
    Serial.println("duz gel");
  }
  delay(100);     //100 ms bekleme

  if(myVR.load((uint8_t)dur) >= 0){
    Serial.println("dur");
  }
delay(100);     //100 ms bekleme

  if(myVR.load((uint8_t)basla) >= 0){
    Serial.println("basla");
  }
 delay(100);     //100 ms bekleme
}

void loop()
{
mod=28;


    esc = map(pot, 0, 250, 30, 142);
myesc.write(esc);
    delay(50);


swch = digitalRead(sw);

  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){


      case sag:
      if(swch == LOW){
myservo2.write(125);
        }
   else{
myservo3.write(160);
    }
        break;
        
      case sol:
       if(swch == LOW){
myservo2.write(80);
        }
   else{
myservo3.write(0);
    }
  
        break;
        
      case duzgit:
myservo1.write(90);
myservo2.write(100);
myservo3.write(90);
        break;

         case yukari:
myservo1.write(180);

        break;

         case asagi:

myservo1.write(0);

        break;

             case dur:
      
     if(mod=28 and pot < 250){
for (mod= 28; mod <= 70; mod += 1) {          // yavaşlama
  pot=pot+1;
  esc = map(pot, 0, 250, 30, 142);
  myesc.write(esc);
    delay(50);
  }
      }
        break;
       
      case basla:
      
 if(mod=28 and pot > 0){
for (mod= 28; mod <= 70; mod += 1) {   // hızlanma
  pot=pot-1;
  esc = map(pot, 0, 250, 30, 142);
  myesc.write(esc);
    delay(50);
  }
      }
        break;
        
      default:
        Serial.println("Record function undefined");
        break;
    }
    /** voice recognized */
    printVR(buf);
  }
}
