/**
 * Takes input from circuit to determine speed of metronome and beats along to it
 * Preset rhythms, some random logic to determine which rhythm to play
 * Uses the encoder motor to move
 */
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

SoftwareSerial mySerial(11, 10);
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *m1 = AFMS.getMotor(1); //hand motor
Adafruit_DCMotor *m2 = AFMS.getMotor(2); //foot motor


char state = 'o'; //determines whether to look for beat or not

int bpm = 0; //beats per minute, calculated using period
float period = 0; //length of period, found from beat detection

int t1, t2 = 0; //time variable to avoid use of delays

float st_quarter, rt_quarter = 0; //strike time and rest time quarter note length from period
int rhythm = 1; //original rhythm to play with no user input

int Aencoder0PinA = 3; //yellow wire/pin A pin
int Aencoder0PinB = 4; //green wire/pin B pin
int Fencoder0PinA = 5; //yellow wire/pin A pin
int Fencoder0PinB = 6; //green wire/pin B pin

int encoder0Pos = 0; //starting position of encoder
int encoder0PinALast = LOW; //tells last state of Pin A
int n = LOW; //current state of Pin A

int targetUp = 20; //target position going up
int targetDown = -20; //target position going down

//int legTargetUp = 20; //target position going up
//int legTargetDown = -20; //target position going down

int target = 20;


//function to make a hit
void beat(Adafruit_DCMotor *m, int st, int rt, int encoder0PinA, int encoder0PinB){
  while(encoder0Pos < target){
    Serial.println("WHILE");
    n = digitalRead(encoder0PinA);
    if ((encoder0PinALast == LOW) && (n == HIGH)) {
      Serial.println("IF 1");
      if (digitalRead(encoder0PinB) == LOW) {
        Serial.println("SUB");
        encoder0Pos--;
      } else {
        Serial.println("ADD");
        encoder0Pos++;
      }
      Serial.print (encoder0Pos);
      Serial.println ("/");
    }
    encoder0PinALast = n;
    
    m->run(FORWARD);
  }

  Serial.print("STAHP");
  m->run(RELEASE);
  
  //wait a certain amount of time before going down
  t2 = millis();
  while(t2-t1 <= 1000){ //turns motor off for length of rt
    t2 = millis();
  }
  t1 = t2;

  while(encoder0Pos > -target){
    Serial.println("NEXT WHILE");
    n = digitalRead(encoder0PinA);
    if ((encoder0PinALast == LOW) && (n == HIGH)) {
      if (digitalRead(encoder0PinB) == LOW) {
        encoder0Pos--;
      } else {
        encoder0Pos++;
      }
      Serial.print (encoder0Pos);
      Serial.println ("/");
    }
    encoder0PinALast = n;
    
    m->run(BACKWARD);
  }

  m->run(RELEASE);
  
  //wait a certain amount of time before going down
  t2 = millis();
  while(t2-t1 <= 1000){ //turns motor off for length of rt
    t2 = millis();
  }
  t1 = t2;
}


void setup() {
  Serial.begin(115200);
  mySerial.begin(14400);
  AFMS.begin();

  m1->setSpeed(150);

  pinMode (Aencoder0PinA, INPUT);
  pinMode (Aencoder0PinB, INPUT);
}

void loop() {

  if(mySerial.available() > 0){ //if serial value is input, read it
    Serial.println("Receiving");
    bpm = mySerial.read();
    period = float(60000)/float(bpm);
    Serial.println("****************************");
    Serial.println(bpm);

  
    st_quarter = float(.2)*float(period);
    rt_quarter = float(.8)*float(period);

    rhythm = 0;
  }

  
  Serial.println(st_quarter);
  Serial.println(rt_quarter);

  //beat(m1, st_quarter, rt_quarter,  Aencoder0PinA, Aencoder0PinB);

  
  switch(rhythm){ //switches between 7 different rhythms depending on what variable rhythm equals
    case 0: //quarter notes
      Serial.println("Case 0");
      beat(m1, st_quarter, rt_quarter,  Aencoder0PinA, Aencoder0PinB);
      break;
//  
//    case 1: //eigth notes
//      //Serial.println("Case 1");
//      beat(m1, st_quarter/2, rt_quarter/2, Aencoder0PinA, Aencoder0PinB);
//      break;
//  
//    case 2: //sixteenth notes
//      //Serial.println("Case 2");
//      beat(m1, st_quarter/4, rt_quarter/4, Aencoder0PinA, Aencoder0PinB);
//      break;
//  
//    case 3: //eigth+sixteenth
//      beat(m1, 100, 400, Aencoder0PinA, Aencoder0PinB);
//      beat(m1, 50, 200, Aencoder0PinA, Aencoder0PinB);
//      break;
//  
//    case 4: //eighth+sixteenth+sixteenth
//      beat(m1, 100, 400, Aencoder0PinA, Aencoder0PinB);
//      beat(m1, 50, 200, Aencoder0PinA, Aencoder0PinB);
//      beat(m1, 50, 200, Aencoder0PinA, Aencoder0PinB);     
//      break;
//  
//    case 5://e+s+s+e+s
//      beat(m1, 100, 400, Aencoder0PinA, Aencoder0PinB);
//      beat(m1, 50, 200, Aencoder0PinA, Aencoder0PinB);
//      beat(m1, 50, 200, Aencoder0PinA, Aencoder0PinB); 
//      beat(m1, 100, 400, Aencoder0PinA, Aencoder0PinB);
//      beat(m1, 50, 200, Aencoder0PinA, Aencoder0PinB);
//      break;
//  
//    case 6://1++3+4
//      beat(m1, 100, 400, Aencoder0PinA, Aencoder0PinB);
//      beat(m1, 100, 800, Aencoder0PinA, Aencoder0PinB);
//      beat(m1, 100, 400, Aencoder0PinA, Aencoder0PinB);
//      beat(m1, 100, 400, Aencoder0PinA, Aencoder0PinB);
//      beat(m1, 100, 400, Aencoder0PinA, Aencoder0PinB);
//      beat(m1, 100, 800, Aencoder0PinA, Aencoder0PinB);
//      break;  
//
//    case 7: //foot and hand
//      beat(m1, st_quarter, rt_quarter, Aencoder0PinA, Aencoder0PinB); //hand
//      beat(m2, st_quarter, rt_quarter, Fencoder0PinA, Fencoder0PinB); //foot
//      break;
//  
//    case 8: // don't move
//      Serial.print("Outside Range");
//      digitalWrite(m1, LOW);
//      break;
        
  }
}




