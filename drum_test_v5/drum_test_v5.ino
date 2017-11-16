/**
 * Takes input from circuit to determine speed of metronome and beats along to it
 * Preset rhythms, some random logic to determine which rhythm to play
 */

char state = 'o'; //determines whether to look for beat or not

const int beat_pin = 2; //looks for pulse to find period fo beat
//const int IRQ_GATE_IN = 1;
//const int PIN_ANALOG_IN = A0;
int bpm = 60;

int pin_val = 0; //reading from beat_pin

int t1, t2 = 0; //time variable to avoid use of delays

int striketime = 200; // the length of time the motor will be on
int resttime = 800; // the length of rest between strikes
float period = 1000; //length of period, found from beat detection

float st_quarter, rt_quarter = 0; //strike time and rest time quarter note length from period
const int m1 = 8; //motor pin
int rhythm = 0; //original rhythm to play with no user input

int ontime = 5; // the length of time the motor will be on
int offtime = 8; // the length of rest between strikes

void wait(int t){
  t2 = millis();
  while(t2-t1 <= t){ //turns motor off for length of rt
    t2 = millis();
  }
  t1 = t2;
}

//function to make a hit
void beat(int pin, int st, int rt){ 
  //t1 and t2 used to avoid use of delay
  t2 = millis();
  while(t2-t1 <= st){ //turns motor on for length of st
    digitalWrite(8, HIGH); // turn on motor 1
    wait(ontime);
    digitalWrite(8, LOW); // turn off motor 1
    wait(offtime); // wait
    //digitalWrite(m1, HIGH); //turns motor on
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
  for(int i = 1; i<=8; i++){
    Serial.println(i);
    pin_val = digitalRead(beat_pin); //beat detection reading
    
    long dur_on = pulseIn(beat_pin, HIGH, 3000000); //duration beat sounds
    Serial.println(dur_on);
    absolute(dur_on);
    Serial.println(dur_on);
    
    long dur_off = pulseIn(beat_pin, LOW, 3000000); //duration between beats
    absolute(dur_off);

    period = period + dur_on + dur_off; //total period (all together) to allow for averaging
    Serial.println(period);
    avgPeriod = period/long(i); //averags to find period
    Serial.println(avgPeriod);

  }

  period = avgPeriod/1000.0; //finds period
  bpm = 60000/period;
  bpm = int(bpm);
  period = float(60000)/float(bpm);
  Serial.println(period);
  

  t1 = millis();
  t2 = millis();
  //for quarter note: period*3.53 + 2*period/3 - 50
  //for eighth note: period*3.53
  while(t2-t1 <= period*3.53 + 2*period/3 - 50){ //2300 for 92bpm
    t2 = millis();
  }
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

  Serial.println(period);
  bpm = 60000/period;

  if(bpm <=80){
    rhythm = 7;
  }
  else if(bpm <= 100 && bpm >= 80){
    rhythm = 1;
  }
  else if(bpm>=100 && bpm <= 130){
    rhythm = 1;
  }
  else if(bpm>=130){
    rhythm = 7;
  }
  
  st_quarter = float(.2)*float(period);
  rt_quarter = float(.8)*float(period);

  Serial.println(st_quarter);
  Serial.println(rt_quarter);


//  beat(m1, st_quarter, rt_quarter);
//  beat(m1, st_quarter/2, rt_quarter/2);
//  beat(m1, st_quarter/2, rt_quarter/2);

  
  
  
  switch(rhythm){ //switches between 7 different rhythms depending on what variable rhythm equals
    case 0: //quarter notes
      Serial.println("Case 0");
      beat(m1, st_quarter, rt_quarter);
      break;

    case 1: //eigth notes
      Serial.println("Case 1");
      beat(m1, st_quarter/2, rt_quarter/2);
      break;

    case 2: //sixteenth notes
      Serial.println("Case 2");
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




