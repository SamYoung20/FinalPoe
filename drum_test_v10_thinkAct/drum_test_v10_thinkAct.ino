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
Adafruit_DCMotor *m2 = AFMS.getMotor(4); //foot motor

int POT = A0;

int bpm = 0; //beats per minute, calculated using period
float period = 0; //length of period, found from beat detection

int t1, t2 = 0; //time variable to avoid use of delays
int foot_time1, foot_time2, foot_time = 0;
int foot_start = 0;

float st_quarter, rt_quarter = 0; //strike time and rest time quarter note length from period
int rhythm = 1; //original rhythm to play with no user input

int calibrateTime = 1;

char user_in = 'o';
char drum_status = 's';

void setup() {
  //start Serials and motor shield
  Serial.begin(115200);
  mySerial.begin(14400);
  AFMS.begin();

  //set initial speeds for motors
  //m1->setSpeed(150); //arm
  m1->setSpeed(150); //arm
  m2->setSpeed(105); //foot

}

/*
void loop() {
  m1->setSpeed(70);
  t2 = millis();
  while (t2 - t1 <= 60) { //turns motor off for length of rt
    m1->run(BACKWARD);
    int ana_in = analogRead(POT);
      Serial.println(ana_in);
    foot_time2 = millis();
    t2 = millis();
  }
  t1 = t2;

  while (t2 - t1 <= 2000) { //turns motor off for length of rt
    m1->run(RELEASE);
    int ana_in = analogRead(POT);
      Serial.println(ana_in);
    foot_time2 = millis();
    t2 = millis();
  }
  t1 = t2;
  
}
*/


void loop() {
  if (mySerial.available() > 0) { //if serial value is input, read it
    if (Serial.available()){
      user_in = Serial.read(); }
    else
      user_in = 'o';
   
      if(user_in == 'p'){
        bpm = mySerial.read(); //takes bpm from sense Arduino
        //bpm = 57;
        Serial.println(bpm);
        period = float(60000) / float(bpm);
        rhythm = 0;
      }

    if (user_in == 's')
      drum_status = 's';

    if (user_in == 'g'){
      drum_status = 'g'; 
      Serial.println("go");}
      
    //run arm and/or foot
    if(user_in == 'f'){
      calibrateTime = 5;
      Serial.println(calibrateTime);}
    else if(user_in == 'd'){
      calibrateTime = -5;
      Serial.println(calibrateTime);}
    else if(user_in == 'm'){
      calibrateTime = 20;
      Serial.println(calibrateTime);}
    else if(user_in == 'n'){
      calibrateTime = -20;
      Serial.println(calibrateTime);}
    else
      calibrateTime = 0;
  
    if (drum_status == 'g') {
      //hand(m1, period, calibrateTime);
      int ana_in = analogRead(POT);
      //Serial.println(ana_in);
      foot_start = millis();
      //foot(m2, period, calibrateTime); 
      hand(m1, period, calibrateTime); 
      }
}
}


//function to make a hit, params: motor, strike and rest time,
//runs motor forwards, then backwards, then waits
void hand(Adafruit_DCMotor *m, float period, int calibrateTime) {
  float fd = 253; //determine forward time
  int bk = 82; //determine back time
  int brkTime = 40; //determine rest time
  int rt = period - fd - bk - brkTime + calibrateTime; //determine rest time
  //add or subtract to rest time here temporarily
  
  //t1 and t2 used to avoid use of delay
  t2 = millis();
  while (t2 - t1 <= fd) { //turns motor on for length of fd
    m->setSpeed(130);
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
    m->setSpeed(130);
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
void foot(Adafruit_DCMotor *m, float period, int calibrateTime) {
  int up = 500;
  int down = 900;
  m->setSpeed(100);

  int ana_in = analogRead(POT);
  //Serial.println(ana_in);

  t2 = millis();
    while (analogRead(POT) < up) { //turns motor off for length of rt
      m->setSpeed(60);
      m->run(FORWARD);
      foot_time2 = millis();
      t2 = millis();
      }
    t1 = t2;
    m->setSpeed(100);
  

  t2 = millis();
  while (analogRead(POT) > down) { //turns motor off for length of rt
      m->setSpeed(60);
      m->run(BACKWARD);
      foot_time2 = millis();
      t2 = millis();
      }
    t1 = t2;
    m->setSpeed(100);

  t2 = millis();
  while (analogRead(POT) < down) { //turns motor off for length of rt
    ana_in = analogRead(POT);
  //Serial.println(ana_in);
    m->run(FORWARD);
    foot_time2 = millis();
    t2 = millis();
  }
  t1 = t2;

  t2 = millis();
  while (t2 - t1 <= 150) { //turns motor off for length of rt
    m->run(RELEASE);
    foot_time2 = millis();
    t2 = millis();
  }
  t1 = t2;

  t2 = millis();
  while (analogRead(POT) > up) { //turns motor off for length of rt
    ana_in = analogRead(POT);
  //Serial.println(ana_in);
    m->run(BACKWARD);
    foot_time2 = millis();
    t2 = millis();
  }
  t1 = t2;

  int foot_end = millis();
  int foot_time = foot_end-foot_start;
  
  t1 = millis();
  t2 = millis();
  int foot_delay_time = period-foot_time+calibrateTime;
  Serial.println(foot_delay_time);
  while (t2 - t1 <= foot_delay_time) { //turns motor off for length of rt
    m->run(RELEASE);
    t2 = millis();
  }
  t1 = t2;
  foot_time1 = foot_time2;
}
