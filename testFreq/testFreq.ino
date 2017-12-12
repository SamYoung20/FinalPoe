#include <SoftwareSerial.h>
unsigned int data =0;
const int samples = 3;
const unsigned int debounce = 350;
const int threshold = 100;
float rolling[samples]; // global variable will keep track of relevant samples
const int inp = A2;
unsigned long lastTime;
SoftwareSerial mySerial(11, 10);
void setup() {
  Serial.begin(9600);
  mySerial.begin(14400);  
  lastTime = millis();
}

void loop() {
  int sense = analogRead(inp);
  unsigned long revTime;
  if(sense < threshold && (unsigned long)(millis() - lastTime) > debounce){         // close to negative rail (is it a beat) and is a new beat (debounce)
    revTime = millis() - lastTime;
    lastTime = millis();
    float freq = convertUnits(revTime);                                             // 
    unsigned int avg = updateAverage(freq);
    data = avg;
    Serial.println(data);
    mySerial.write(data);
  }
}

float updateAverage(float newVal){
  float tempRolling[samples];
  tempRolling[samples - 1] = newVal; // the last element is the new value

  for(int i = 0; i < samples - 1; i ++) // copy the last elements of the old array into the beginning of the new array
    tempRolling[i] = rolling[i+1];

  for(int i = 0; i < samples; i ++) // overwrite the old array with the new one
    rolling[i] = tempRolling[i];
  
  unsigned long sum = 0;
  for(int i = 0; i < samples; i ++) // sum the elements
    sum += rolling[i];
  
  unsigned int avg = round(sum/samples);
  return avg;
}

float convertUnits(unsigned long input){
  //to Hz:
  float freq = 60000.00/input; //micros
  return freq;
}

void printArray(float arr[], int len){ // useful for debugging
  Serial.print("{");
  for(int i = 0; i < len - 1; i ++){
    Serial.print(arr[i]);
    Serial.print(", ");
  }
  Serial.print(arr[len - 1]);
  Serial.println("}");
}

