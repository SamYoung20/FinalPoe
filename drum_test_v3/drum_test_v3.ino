/**
 * Takes input from circuit to determine speed of metronome and beats along to it
 */

char state = 'o'; //determines whether to look for beat or not

const int beat_pin = 2; //looks for pulse to find period fo beat
//const int IRQ_GATE_IN = 1;
//const int PIN_ANALOG_IN = A0;

int pin_val = 0; //reading from beat_pin

int t1, t2 = 0; //time variable to avoid use of delays

int striketime = 200; // the length of time the motor will be on
int resttime = 800; // the length of rest between strikes
long period = 1000; //length of period, found from beat detection

int st_quarter, rt_quarter = 0; //strike time and rest time quarter note length from period
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

//function to find period from beat detection 
void findPeriod(){
  Serial.println("In findPeriod");
  long avgPeriod = 0;
  //long p = 0;
  for(int i = 1; i<=8; i++){
    Serial.println(i);
    pin_val = digitalRead(beat_pin); //beat detection reading
    
    long dur_on = pulseIn(beat_pin, HIGH, 3000000); //duration beat sounds
    dur_on = abs(dur_on);
    
    long dur_off = pulseIn(beat_pin, LOW, 3000000); //duration between beats
    dur_off = abs(dur_off);

    //p = dur_on + dur_off;
    period = period + dur_on + dur_off; //total period (all together) to allow for averaging
    avgPeriod = period/i; //averags to find period
  }

  period = avgPeriod/1000; //finds period
  Serial.println(period);
}

void setup() {
  //pinMode(PIN_GATE_IN, INPUT);
  //attachInterrupt(IRQ_GATE_IN, soundISR, CHANGE);

  pinMode(m1, OUTPUT); // set the output pins
  Serial.begin(9600);
  //findPeriod();
}

void loop() {
//  if(Serial.available() > 0){ //if serial value is input, read it
//    rhythm = Serial.read()-'0'; // make byte read from Serial into the actual in it is
//    Serial.print(rhythm);
//  }

  if(Serial.available() > 0) { //checks for input from Serial
    state = Serial.read(); //0 or 1
    Serial.write(state);
    if(state == 'f'){ //finds beat if type 'f'
      findPeriod();
    }
  } 
  

  st_quarter = .2*period;
  rt_quarter = .8*period;

  //Serial.println(st_quarter);
  //Serial.println(rt_quarter);

  beat(m1, st_quarter, rt_quarter);
  beat(m1, st_quarter/2, rt_quarter/2);
  beat(m1, st_quarter/2, rt_quarter/2);
  
  
//  switch(rhythm){ //switches between 7 different rhythms depending on what variable rhythm equals
//    case 0: //quarter notes
//      beat(m1, 200, 800);
//      break;
//
//    case 1: //eigth notes
//      beat(m1, 100, 400);
//      break;
//
//    case 2: //sixteenth notes
//      beat(m1, 50, 200);
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
//  }
}




