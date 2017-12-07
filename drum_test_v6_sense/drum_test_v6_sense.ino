/**
 * Takes input from circuit to determine speed of metronome/song.
 * Sends bpm to Think/Act Arduino.
 * To manually find beat, sned 'f' to the serial.
 * Serial displays information about each listen then displays bpm
 */

 //Set up software serial between Arduinos
#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10);

int POT = A0;

char state = 'o'; //determines whether to look for beat or not

const int beat_pin = 2; //looks for pulse to find period fo beat
int bpm = 60; //initial beats per minute value

float period = 1000; //length of period, found from beat detection

int pin_val = 0; //reading from beat_pin

int t1, t2 = 0; //time variable to avoid use of delays

int lis = 2; //how many beats should listen for

//takes absolute value of a long
void absolute(long x){
  if(x<0){
    x = -x;
  }
  else{
    x = x;
  }
}

//function to find period from beat detection 
void findPeriod(){
  Serial.println("In findPeriod");
  long avgPeriod = 0;
  for(int i = 1; i<=lis; i++){
    Serial.println(i);
    pin_val = digitalRead(beat_pin); //beat detection reading
    
    long dur_on = pulseIn(beat_pin, HIGH, 3000000); //duration beat sounds
    absolute(dur_on);
    while(dur_on < 100000){
      Serial.println("dur too low");
      Serial.println(dur_on);
      long dur_on = pulseIn(beat_pin, HIGH, 3000000); //duration beat sounds
      absolute(dur_on);
    }
    Serial.println(dur_on);
    
    long dur_off = pulseIn(beat_pin, LOW, 3000000); //duration between beats
    absolute(dur_off);
    Serial.println(dur_off);
    while(dur_off < 100000){
      long dur_off = pulseIn(beat_pin, LOW, 3000000); //duration beat sounds
      absolute(dur_on);
    }

    period = period + dur_on + dur_off; //total period (all together) to allow for averaging
    Serial.println(period);
    avgPeriod = period/long(i); //averages to find period
    Serial.println(avgPeriod);

  }

  //finds the correct period and bpm from the reading
  period = avgPeriod/1000.0; //finds period
  Serial.println(period);
  Serial.println(period, 7);
  bpm = 60000/period; //finds bpm with read period
  bpm = int(bpm); //turns bpm to int since bpm is never not an int
  period = float(60000)/float(bpm); //finds proper period from corrected bpm
  Serial.println(period);
  
}

void setup() {
  //set up Serial and software Serial
  Serial.begin(9600);
  mySerial.begin(14400);
  pinMode(POT, INPUT);
}

void loop() {

//  Serial.print("++++++++++");
//  Serial.print(analogRead(POT));
//  Serial.print("++++++++++");

  if(Serial.available() > 0) { //checks for input from Serial
    state = Serial.read(); //0 or 1
    Serial.write(state);
    if(state == 'f'){ //finds beat if type 'f'
      findPeriod(); //finds function to find period
      bpm = 60000/period; //finds the bpm, easier to compare with bpm
      Serial.println(bpm);
      mySerial.write(bpm); //Software serial sends ints, not longs
                           //send bpm, convert back to period in think/act
    }
  } 
//  else{
//    //waits for 10 seconds, then finds period and sends to other Arduino again. Loops! Continuously updates! 
//    t1 = millis();
//    t2 = millis();
//    while(t2-t1 <= 10000){
//      t2 = millis();
//    }
//  
//    findPeriod();
//    bpm = 60000/period;
//    mySerial.write(bpm);
//  
//    Serial.println(bpm);
//  }
}




