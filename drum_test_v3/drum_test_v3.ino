/**
 * Takes input from circuit to determine speed of metronome and beats along to it
 */

//const int PIN_GATE_IN = 2;
//const int IRQ_GATE_IN = 1;
//const int PIN_ANALOG_IN = A0;
//int pin_val = 0;

int t1, t2 = 0;

int striketime = 200; // the length of time the motor will be on
int resttime = 800; // the length of rest between strikes

const int m1 = 8; //motor pin
int rhythm = 0; //original rhythm to play with no user input

//function to make a hit
void beat(int pin, int st, int rt){ 
  t2 = millis();
  Serial.print(t2-t1);
 
  while(t2-t1 <= st){
    digitalWrite(m1, HIGH); //turns motor on
    t2 = millis();
  }
  t1 = t2; 

  t2 = millis();
  while(t2-t1 <= rt){
    digitalWrite(m1, LOW); // turns motor off
    t2 = millis();
  }
  t1 = t2;
}

void setup() {
  //pinMode(PIN_GATE_IN, INPUT);
  //attachInterrupt(IRQ_GATE_IN, soundISR, CHANGE);
  
  pinMode(m1, OUTPUT); // set the output pins
  Serial.begin(9600);
}

void loop() {
  //pin_val = digitalRead(PIN_GATE_IN);
  //Serial.println(pin_val);
  
  if(Serial.available() > 0){ //if serial value is input, read it
    rhythm = Serial.read()-'0'; // make byte read from Serial into the actual in it is
    //Serial.print(rhythm);
  //
  
  }

  switch(rhythm){ //switches between 7 different rhythms depending on what variable rhythm equals
    case 0: //quarter notes
      beat(m1, 200, 800);
      break;

    case 1: //eigth notes
      beat(m1, 100, 400);
      break;

    case 2: //sixteenth notes
      beat(m1, 50, 200);
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
  }
}




