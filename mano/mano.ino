//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___ 
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __| 
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/ 
// 
// Manometer
// 
// Made by Dmitry Goryunov
// License: CC-BY-SA 3.0
// Downloaded from: https://circuits.io/circuits/3956103-manometer

#include <Servo.h>

bool done = false;

const int switches[] = {A1,A2,A3,A4};
const int relays[] = {2,3,4, 7,8, 10, 11};

const int servoPin = 5;
const int servoStart = 50;
const int servoEnd = 93;
const int servoStep = 1;
const int servoSpan = 1000;
int servoState = 0;


Servo servo;

void setup(){
  for(int i = 0; i<4; i++)
    pinMode(switches[i], INPUT_PULLUP);
    
  for(int i = 0; i<7; i++){
    pinMode(relays[i], INPUT_PULLUP);
    digitalWrite(relays[i], LOW);    
  }
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  
  
  servo.attach(servoPin);
  moveTo(servoStart);
}

void moveTo(int newState){
  servo.write(newState);
  servoState = newState;
}

void phase1(){
  digitalWrite(relays[0], HIGH);
  digitalWrite(relays[1], HIGH);
  digitalWrite(relays[2], HIGH);

  while(servoState < servoEnd){
    moveTo(servoState + servoStep);
    delay(servoSpan);    
  }
}
void phase2(){
  digitalWrite(relays[3], HIGH);
  digitalWrite(relays[4], HIGH);
  moveTo(servoStart);  
}
void phase3(){
  int time = random(1000, 5000);
  delay(time);
  digitalWrite(relays[5], HIGH);
}
void phase4(){
  int time = random(1000, 5000);
  delay(time);
  digitalWrite(relays[6], HIGH);  
}

bool isSolved(){
  bool result = true;
  for(int i = 0; i<4; i++){
    result = result 
      && digitalRead(switches[i]) == LOW;
  }
  return result;
}

void loop(){
  if(isSolved() && !done){
    phase1();
    phase2();
    phase3();
    phase4();

    done = true;
  }
}
