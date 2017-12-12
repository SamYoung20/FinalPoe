// change this value to match your striketime
int striketime = 100; // the length of time the motor will be on

// will hold the delay times for various beats
int fastbeat = 0;
int beat = 0;
int slowbeat = 0;
void setup() {
pinMode(7, OUTPUT); // set the output pins
pinMode(8, OUTPUT); // make sure these pinnumbers match your wiring
// set the delay times
fastbeat = 150 - striketime;
beat = 300 - striketime;
slowbeat = 600 - striketime;
}
void loop() {
digitalWrite(8, HIGH);
delay(striketime);
digitalWrite(8, LOW);
delay(beat);
digitalWrite(8, HIGH);
delay(striketime);
digitalWrite(8, LOW);
delay(beat);
}


