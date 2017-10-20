int striketime = 3000; // the length of time the motor will be on
int resttime = 1000; // the length of rest between strikes

void setup() {
  pinMode(8, OUTPUT); // set the output pins
}

void loop() {
  digitalWrite(8, HIGH); // turn on motor 1
  delay(striketime);
  digitalWrite(8, LOW); // turn off motor 1
  delay(resttime); // wait
}
