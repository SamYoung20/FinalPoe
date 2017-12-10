/**
   Takes input from circuit to determine speed of metronome/song.
   Sends bpm to Think/Act Arduino.
   To manually find beat, send 'f' to the serial.
   Serial displays information about each listen then displays bpm
*/

#include <SoftwareSerial.h>
//#include <Wire.h>
#include <math.h>

SoftwareSerial mySerial(11, 10);
const int samples = 10;
int rolling[samples]; // global variable will keep track of relevant samples
int POT = A0;
char state = 'o';                                  //determines whether to look for beat or not
const int beat_pin = A2;                            //looks for pulse to find period fo beat
int bpm = 60;                                       //initial beats per minute value
float period = 1000;                               //length of period, found from beat detection
int pin_val = 0;                                   //reading from beat_pin
int t1, t2 = 0;                                    //time variable to avoid use of delays
int lis = 2;                                       //how many beats should listen for
int start_time = 0;                                //timing for processing time calibration
unsigned long end_time, run_time, lastTime, revTime = 0;
const unsigned int debounce = 100000;               //throw out anything faster than this (microseconds)
const int threshold = 100;                         //threshold analogIn reading, must be UNDER this value
int totalTime = 0;
int sense = 0;
unsigned int data;

void setup() {
  Serial.begin(9600);                              //set up display Serial
  mySerial.begin(14400);                           //set up Arduino communication Serial
  pinMode(POT, INPUT);
  pinMode(beat_pin, INPUT);
  lastTime = micros();
}

void loop() {
  int sense = analogRead(beat_pin);
  unsigned long revTime;
  Serial.print("analog reading: ");
  Serial.println(sense);
  if (sense < threshold){// && (unsigned long)(micros() - lastTime) > debounce) {
    Serial.println("New Beat");
    revTime = micros() - lastTime;
    lastTime = micros();
    double freq = convertUnits(revTime);
    //unsigned long avg = updateAverage(freq);
    //data = avg;
    // Serial.print("Freq: ");
    //Serial.println(freq);

    //if(Serial.available() > 0) {                      //checks for input from Serial
    //state = Serial.read();
    //Serial.write(state);
    //Serial.println(analogRead(beat_pin));
    //if(state == 'f'){                               //finds beat if type 'f'
    //start_time = millis();
    //findPeriod();                                 //finds function to find period
    //Serial.print("BPM: ");
    //Serial.println(bpm);
    //mySerial.write(bpm);                          //send bpm, convert back to period in think/act
    //}
    //}
  }
}

int updateAverage(unsigned int newVal) {
  unsigned int tempRolling[samples];

  tempRolling[samples - 1] = newVal; // the last element is the new value

  for (int i = 0; i < samples - 1; i ++) { // copy the last elements of the old array into the beginning of the new array
    tempRolling[i] = rolling[i + 1];
  }

  for (int i = 0; i < samples; i ++) { // overwrite the old array with the new one
    rolling[i] = tempRolling[i];
  }
  unsigned long sum = 0;
  for (int i = 0; i < samples; i ++) { // sum the elements
    sum += rolling[i];
  }
  unsigned int avg = sum / samples;
  return avg;
}

unsigned int convertUnits(unsigned long input) {
  //to Hz:
  double freq = 1000000 / (double(input));
  bpm = 60 * freq;
  return bpm;
  /*
    //MPH:
    float circ = wheelDia * 3.1416;
    int speedMPH = (freq * 3600 * circ)/63360;
    return speedMPH;
    /*
    //RPM:
    unsigned int RPM = input * 60 * 2;
    return RPM;
  */
}

void printArray(int arr[], int len) { // useful for debugging
  Serial.print("{");
  for (int i = 0; i < len - 1; i ++) {
    Serial.print(arr[i]);
    Serial.print(", ");
  }
  Serial.print(arr[len - 1]);
  Serial.println("}");
}

void absolute(long x) {                             //takes absolute value of a long
  if (x < 0)
    x = -x;
  else
    x = x;
}
/*
  void findPeriod(){                                   //function to find period from beat detection
  start_time = millis();
  long avgPeriod = 0;
  int sense = analogRead(beat_pin);
  unsigned long revTime;
  Serial.println(sense);
  if(sense < threshold && (unsigned long)(micros() - lastTime) > debounce){
      revTime = micros() - lastTime;
      lastTime = micros();
      unsigned int freq = convertUnits(revTime);
      unsigned int avg = updateAverage(freq);
      data = avg;
      Serial.print("Avg Freq: ");
      Serial.println(data);
      //Serial.println(revTime);
      //period = revTime/1000;
      //int totalTime = totalTime + period;
      //avgPeriod = float(totalTime);
    }
    //avgPeriod = float(avgPeriod)/lis;
    //bpm = 60000/avgPeriod;                            //finds bpm with read period
  }

  //processingCalibration();

  // bpm = int(bpm);                                     //turns bpm to int since bpm is never not an int
  //period = float(60000)/float(bpm);                   //finds proper period from corrected bpm
  }

  void processingCalibration(){
  end_time = millis();
  run_time = end_time - start_time;

  float periods_taken = float(run_time)/float(period);
  int ideal_time = round(periods_taken)*period;
  int delay_time = ideal_time-run_time;

  delay(delay_time);
  int final_end_time = millis();
  Serial.print("Processing Time: ");
  Serial.println(final_end_time-start_time);
  }
*/
