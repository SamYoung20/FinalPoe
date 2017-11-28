/**
 * Takes input from circuit to determine speed of metronome and beats along to it
 * Preset rhythms, some random logic to determine which rhythm to play
 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10);


char state = 'o'; //determines whether to look for beat or not

int bpm = 60;
float period = 1000; //length of period, found from beat detection

int t1, t2 = 0; //time variable to avoid use of delays

float st_quarter, rt_quarter = 0; //strike time and rest time quarter note length from period
const int m1 = 8; //motor pin
int rhythm = 0; //original rhythm to play with no user input

//function to make a hit
void beat(int pin, int st, int rt){ 
  //t1 and t2 used to avoid use of delay
  t2 = millis();
  while(t2-t1 <= st){ //turns motor on for length of st
    digitalWrite(m1, HIGH); //turns motor on
    t2 = millis();
  }
  t1 = t2; 

  t2 = millis();
  while(t2-t1 <= rt){ //turns motor off for length of rt
    digitalWrite(m1, LOW); // turns motor off
    t2 = millis();
  }
  t1 = t2;
}


void setup() {
  pinMode(m1, OUTPUT); // set the output pins
  Serial.begin(9600);
  mySerial.begin(4800);
}

void loop() {

  if(mySerial.available() > 0){ //if serial value is input, read it
    Serial.println("Receiving");
    bpm = mySerial.read();
    period = float(60000)/float(bpm);
    Serial.println("****************************");


//    t1 = millis();
//    t2 = millis();
//    //for quarter note: period*3.53 + 2*period/3 - 50
//    //for eighth note: period*3.53
//    while(t2-t1 <= period){ //*3.53 + 2*period/3 - 50){ //2300 for 92bpm
//      t2 = millis();
//    }
  }
  //Serial.println(period);

  rhythm = 0;
//  if(bpm <=80){
//    rhythm = 7;
//  }
//  else if(bpm <= 100 && bpm >= 80){
//    rhythm = 0;
//  }
//  else if(bpm>=100 && bpm <= 130){
//    rhythm = 0;
//  }
//  else if(bpm>=130){
//    rhythm = 7;
//  }
  
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

    case 7: // don't move
      Serial.print("Outside Range");
      digitalWrite(m1, LOW);
      break;
      
  }
}




