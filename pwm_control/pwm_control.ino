int ontime = 5; // the length of time the motor will be on
int offtime = 5; // the length of rest between strikes
int beat_pin = 3;
int pin_val, period, bpm = 0;
int t1,t2 = 0;
int avg_p;
unsigned long highTime2 = 0;

void setup() {
  pinMode(beat_pin, INPUT); // set the output pins
  Serial.begin(9600);
}

void absolute(long x){
  if(x<0){
    x = -x;
  }
  else{
    x = x;
  }
}

void findPeriod(){
  long avgPeriod = 0;
  for(int i = 1; i<= 5; i++){
    pin_val = digitalRead(beat_pin); //beat detection reading
    Serial.println(pin_val);
     
    long dur_on = pulseIn(beat_pin, HIGH, 3000000); //duration beat sounds
    absolute(dur_on);

    long dur_off = pulseIn(beat_pin, LOW, 3000000); //duration between beats
    absolute(dur_off);

    period = dur_on + dur_off; //total period (all together) to allow for averaging
    //Serial.println(period);
    avg_p = avg_p + period;
  }

  period = avg_p/5;
  period = period / 100000; 
  //Serial.println(period);
  }
  
void loop() {
  unsigned long highTime = pulseIn(beat_pin, HIGH);
  Serial.println(highTime2 - highTime);
  unsigned long highTime2 = highTime;
  unsigned long lowTime = pulseIn(beat_pin, LOW);
  unsigned long cycleTime = highTime + lowTime;
  float dutyCycle = (float)highTime / float(cycleTime);

  pin_val = digitalRead(beat_pin);
  //Serial.println(pin_val);
  //Serial.println(cycleTime);
}
  //digitalWrite(8, HIGH); // turn on motor 1
  //delay(ontime);
  //digitalWrite(8, LOW); // turn off motor 1
  //delay(offtime); // wait

