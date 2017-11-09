int ontime = 5; // the length of time the motor will be on
int offtime = 8; // the length of rest between strikes

void setup() {
  pinMode(8, OUTPUT); // set the output pins
}

void loop() {
  digitalWrite(8, HIGH); // turn on motor 1
  delay(ontime);
  digitalWrite(8, LOW); // turn off motor 1
  delay(offtime); // wait
}
