/**
   Takes bpm from sense Arduino to determine how to hit the drum.
   Requires user input p
   Will diplay when and what diplaying and strike and rest times
*/

//include all libraries
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"


//Sets up Software serial and motor shield
SoftwareSerial mySerial(11, 10);
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

//sets up motors
Adafruit_DCMotor *m1 = AFMS.getMotor(1); //hand motor
Adafruit_DCMotor *m2 = AFMS.getMotor(2); //foot motor

int POT = A0;

int bpm = 0; //beats per minute, calculated using period
float period = 0; //length of period, found from beat detection

int t1, t2 = 0; //time variable to avoid use of delays

float st_quarter, rt_quarter = 0; //strike time and rest time quarter note length from period
int rhythm = 1; //original rhythm to play with no user input

int calibrateTime = 1;

void setup() {
  //start Serials and motor shield
  Serial.begin(115200);
  mySerial.begin(14400);
  AFMS.begin();

  //set initial speeds for motors
  m1->setSpeed(150); //arm
  m2->setSpeed(100); //foot

}

void loop() {
  //if (mySerial.available() > 0) { //if serial value is input, read it
      if(Serial.read() == 'p'){
      //bpm = mySerial.read(); //takes bpm from sense Arduino
      bpm = 75;
      period = float(60000) / float(bpm);
      Serial.println(bpm);

    rhythm = 0;
      }
    //run arm and/or foot
    if(Serial.read() == 'f')
      calibrateTime = calibrateTime+5;
    if(Serial.read() == 'd')
      calibrateTime = calibrateTime-5;
      
    hand(m1, period);

    //foot(m1, 1023, 341, rt_quarter);
  //}
}


//function to make a hit, params: motor, strike and rest time,
//runs motor forwards, then backwards, then waits
void hand(Adafruit_DCMotor *m, float period) {
  float fd = 193; //determine forward time
  //Serial.println(fd);
  int bk = 62; //determine back time
  int brkTime = 40; //determine rest time
  int rt = period - fd - bk - brkTime; //determine rest time
  
  //t1 and t2 used to avoid use of delay
  t2 = millis();
  while (t2 - t1 <= fd) { //turns motor on for length of fd
    m->run(FORWARD); //turns motor on
    t2 = millis();
  }
  t1 = t2;

  t2 = millis();
  while (t2 - t1 <= brkTime) { //turns motor on for length of fd
    m->run(RELEASE); //turns motor on
    t2 = millis();
  }
  t1 = t2;
  
  t2 = millis();
  while (t2 - t1 <= bk) { //turns motor backwards for length of bk
    m->run(BACKWARD); // turns motor backwards
    t2 = millis();
  }
  t1 = t2;

  t2 = millis();
  while (t2 - t1 <= rt) { //turns motor off for length of rt
    m->run(RELEASE);
    t2 = millis();
  }
  t1 = t2;
}

//function to run the foot
/*
void foot(Adafruit_DCMotor *m, int up, int down, int rt) {
  while (analogRead(POT) < up) { //while POT value is less than the upper bound
    m->run(FORWARD);
  }

  //this will be what I test for calibration when we get there
  //  t2 = millis();
  //  while(t2-t1 <= rt){ //turns motor off for length of rt
  //    t2 = millis();
  //  }
  //  t1 = t2;

  while (analogRead(POT) > down) { //while POT value is greater than the lower bound
    m->run(BACKWARD);
  }
}
*/



<<<<<<< HEAD
  
    st_quarter = float(.2)*float(period); //determines strike time
    rt_quarter = float(.8)*float(period); //determine rest time

    rhythm = 0;
  }

  
  //Serial.println(st_quarter);
  //Serial.println(rt_quarter);

//to run arm, uncomment this one
  //beat(m1, st_quarter, rt_quarter);
  foot(m1, 682, 341, rt_quarter);
  

  
  switch(rhythm){ //switches between 7 different rhythms depending on what variable rhythm equals
    case 0: //quarter notes
      Serial.println("Case 0");
      foot(m2, 1023, 0);
      //st_quarter = rt_quarter; //for foot, still messing with this
      //rt_quarter=0;
      //beat(m1, st_quarter, rt_quarter);
      
      
      break;
//  
//    case 1: //eigth notes
//      //Serial.println("Case 1");
//      beat(m1, st_quarter/2, rt_quarter/2);
//      break;
//  
//    case 2: //sixteenth notes
//      //Serial.println("Case 2");
//      beat(m1, st_quarter/4, rt_quarter/4);
//      break;
//  
//    case 3: //eigth+sixteenth
//      beat(m1, 100, 400);
//      beat(m1, 50, 200);
//      break;
//  
//    case 4: //eighth+sixteenth+sixteenth
//      beat(m1, 100, 400);
//      beat(m1, 50, 200);
//      beat(m1, 50, 200);     
//      break;
//  
//    case 5://e+s+s+e+s
//      beat(m1, 100, 400);
//      beat(m1, 50, 200);
//      beat(m1, 50, 200); 
//      beat(m1, 100, 400);
//      beat(m1, 50, 200);
//      break;
//  
//    case 6://1++3+4
//      beat(m1, 100, 400);
//      beat(m1, 100, 800);
//      beat(m1, 100, 400);
//      beat(m1, 100, 400);
//      beat(m1, 100, 400);
//      beat(m1, 100, 800);
//      break;  
//
//    case 7: //foot and hand
//      beat(m1, st_quarter, rt_quarter); //hand
//      beat(m2, st_quarter, rt_quarter); //foot
//      break;
//  
//    case 8: // don't move
//      Serial.print("Outside Range");
//      digitalWrite(m1, LOW);
//      break;
        
  }
}
=======
>>>>>>> a751f4896b84c602f9f49e4e880fa9785cf7ce2f




