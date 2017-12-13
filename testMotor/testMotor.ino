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
Adafruit_DCMotor *m2 = AFMS.getMotor(4); //foot motor

int POT = A0;

int bpm = 0; //beats per minute, calculated using period
float period = 0; //length of period, found from beat detection

int t1, t2 = 0; //time variable to avoid use of delays

float st_quarter, rt_quarter = 0; //strike time and rest time quarter note length from period
int rhythm = 1; //original rhythm to play with no user input

int calibrateTime = 1;

int time1, time2, time3, time4, time5, time6, time7 = 0

char user_in = 'o';
char drum_status = 's';

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
  if (Serial.available()){
    user_in = Serial.read(); }
  else
    user_in = 'o';
   
  if(user_in == 'p'){
    bpm = 60;
    Serial.println(bpm);
    period = float(60000) / float(bpm);
    drum_status = 'g';
    rhythm = 0;
   }
    
  if(user_in == 'f'){
    calibrateTime = 5;
    Serial.println(calibrateTime);}
  else if(user_in == 'd'){
    calibrateTime = -5;
    Serial.println(calibrateTime);}
  else
    calibrateTime = 0;

  if(user_in == 's'){
    drum_status = 's';
  }
  if (drum_status == 'g')
    beat(m1, m2, period, calibrateTime);
}

void beat(Adafruit_DCMotor *m1, Adafruit_DCMotor *m2, float period, int calibrateTime) {
  float fd = 193; //determine forward time
  int bk = 62; //determine back time
  int brkTime = 40; //determine switch direction time
  int footTime = 62; //stand in for foot down time 
  int rt = period - fd - bk - brkTime + calibrateTime; //determine rest time

  //calibrated arm values
  int arm_up = 193;
  int arm_break = 40;
  int arm_hit = 62;
  int arm_remainder = period - arm_up - arm_break - arm_hit;

  track2 = millis();
  if ((track2 - track1 <= 
    t2 = millis();
    while (t2 - t1 <= wait_time) 
         m1->run(FORWARD);
    t1 = t2;
  
  
}

void wait(char command1, int wait_time){
  t2 = millis();
  while (t2 - t1 <= wait_time) { 
    if (command1 == 'o')
       m1->run(RELEASE);
    else if (command1 == 'f')
       m1->run(FORWARD);
    else if (command1 == 'r')
       m1->run(BACKWARD);
  }
  t1 = t2;
}


