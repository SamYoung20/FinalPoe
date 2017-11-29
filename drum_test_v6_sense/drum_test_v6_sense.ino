/**
 * Takes input from circuit to determine speed of metronome and beats along to it
 * Preset rhythms, some random logic to determine which rhythm to play
 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10);


char state = 'o'; //determines whether to look for beat or not

const int beat_pin = 2; //looks for pulse to find period fo beat
int bpm = 60;

float period = 1000; //length of period, found from beat detection

int pin_val = 0; //reading from beat_pin

int t1, t2 = 0; //time variable to avoid use of delays

int lis = 2; //how many beats should listen for


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
    Serial.println(dur_on);
    
    long dur_off = pulseIn(beat_pin, LOW, 3000000); //duration between beats
    absolute(dur_off);
    Serial.println(dur_off);

    period = period + dur_on + dur_off; //total period (all together) to allow for averaging
    Serial.println(period);
    avgPeriod = period/long(i); //averags to find period
    Serial.println(avgPeriod);

  }

  period = avgPeriod/1000.0; //finds period
  Serial.println(period);
  Serial.println(period, 7);
  bpm = 60000/period;
  bpm = int(bpm);
  period = float(60000)/float(bpm);
  Serial.println(period);
  
}

void setup() {
  Serial.begin(9600);

  mySerial.begin(4800);
}

void loop() {

  if(Serial.available() > 0) { //checks for input from Serial
    state = Serial.read(); //0 or 1
    Serial.write(state);
    if(state == 'f'){ //finds beat if type 'f'
      findPeriod();
      bpm = 60000/period;
      Serial.println(bpm);
      mySerial.write(bpm); //Software serial sends ints, not longs
                           //send bpm, convert back to period in think/act
    }
  } 

  t1 = millis();
  t2 = millis();
  while(t2-t1 <= 10000){
    t2 = millis();
  }

  findPeriod();
  bpm = 60000/period;
  mySerial.write(bpm);

  Serial.println(bpm);
  //bpm = 60000/period;

}




