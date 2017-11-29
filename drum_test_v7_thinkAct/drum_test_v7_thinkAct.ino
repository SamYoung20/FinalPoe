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

int bpm = 60; //beats per minute, calculated using period
float period = 1000; //length of period, found from beat detection

int t1, t2 = 0; //time variable to avoid use of delays

float st_quarter, rt_quarter = 0; //strike time and rest time quarter note length from period
int rhythm = 0; //original rhythm to play with no user input

int encoder0PinA = 3; //yellow wire/pin A pin
int encoder0PinB = 4; //green wire/pin B pin
int encoder0Pos = 0; //starting position of encoder
int encoder0PinALast = LOW; //tells last state of Pin A
int n = LOW; //current state of Pin A

int targetUp = 100; //target position going up
int targetDown = -100; //target position going down



//function to make a hit
void beat(Adafruit_DCMotor *m, int st, int rt){
  //this while loor runs motor up until hits encoder value 
  while(encoder0Pos < targetUp){
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
    
    m->run(FORWARD);
  }

  //wait a certain amount of time before going down
  t2 = millis();
  while(t2-t1 <= st){ //turns motor off for length of rt
    m->run(RELEASE); // turns motor off
    t2 = millis();
  }
  t1 = t2;

  //this while loor runs motor down until hits encoder value 
  while(encoder0Pos > targetDown){
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

  //wait a certain amount of time before go back up
  t2 = millis();
  while(t2-t1 <= rt){ //turns motor off for length of rt
    m1->run(RELEASE); // turns motor off
    t2 = millis();
  }
  t1 = t2;
}


void setup() {
  Serial.begin(9600);
  mySerial.begin(4800);
}

void loop() {

  if(mySerial.available() > 0){ //if serial value is input, read it
    Serial.println("Receiving");
    bpm = mySerial.read();
    period = float(60000)/float(bpm);
    Serial.println("****************************");

  
    st_quarter = float(.2)*float(period);
    rt_quarter = float(.8)*float(period);
  
    Serial.println(st_quarter);
    Serial.println(rt_quarter);
  
    switch(rhythm){ //switches between 7 different rhythms depending on what variable rhythm equals
      case 0: //quarter notes
        //Serial.println("Case 0");
        beat(m1, st_quarter, rt_quarter);
        break;
  
      case 1: //eigth notes
        //Serial.println("Case 1");
        beat(m1, st_quarter/2, rt_quarter/2);
        break;
  
      case 2: //sixteenth notes
        //Serial.println("Case 2");
        beat(m1, st_quarter/4, rt_quarter/4);
        break;
  
      case 3: //eigth+sixteenth
        beat(m1, 100, 400);
        beat(m1, 50, 200);
        break;
  
      case 4: //eighth+sixteenth+sixteenth
        beat(m1, 100, 400);
        beat(m1, 50, 200);
        beat(m1, 50, 200);     
        break;
  
      case 5://e+s+s+e+s
        beat(m1, 100, 400);
        beat(m1, 50, 200);
        beat(m1, 50, 200); 
        beat(m1, 100, 400);
        beat(m1, 50, 200);
        break;
  
      case 6://1++3+4
        beat(m1, 100, 400);
        beat(m1, 100, 800);
        beat(m1, 100, 400);
        beat(m1, 100, 400);
        beat(m1, 100, 400);
        beat(m1, 100, 800);
        break;  

      case 7: //foot and hand
        beat(m1, st_quarter, rt_quarter); //hand
        beat(m2, st_quarter, rt_quarter); //foot
        break;
  
      case 8: // don't move
        Serial.print("Outside Range");
        digitalWrite(m1, LOW);
        break;
        
    }
  }
}




