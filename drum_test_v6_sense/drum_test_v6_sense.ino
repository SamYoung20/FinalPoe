/**
 * Takes input from circuit to determine speed of metronome and beats along to it
 * Preset rhythms, some random logic to determine which rhythm to play
 */

char state = 'o'; //determines whether to look for beat or not

const int beat_pin = 2; //looks for pulse to find period fo beat
int bpm = 60;

int pin_val = 0; //reading from beat_pin

int t1, t2 = 0; //time variable to avoid use of delays


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
  Serial.println(period);
  Serial.println(error);
  Serial.println(period, 7);
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
  Serial.begin(9600);
}

void loop() {

  if(Serial.available() > 0) { //checks for input from Serial
    state = Serial.read(); //0 or 1
    Serial.write(state);
    if(state == 'f'){ //finds beat if type 'f'
      findPeriod();
    }
  } 

  Serial.println(period);
  bpm = 60000/period;

}




