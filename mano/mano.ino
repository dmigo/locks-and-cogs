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

const int servoPin = 10;
const int positionStart = 0;
const int positionEnd = 180;
const int positionDelay = 2000;

const int trembleCount = 10;
int trembles[trembleCount] = {110,80,105,80,100,85,100,85,105,90};
const int trembleDelay = 300;

const int ledPin = 9;

Servo servo;

void setup(){
 	pinMode(ledPin, OUTPUT);
	servo.attach(servoPin);
}

void moveToStart(){
  servo.write(positionStart);
  delay(positionDelay);
}
void moveToEnd(){
  digitalWrite(ledPin, HIGH);           
  servo.write(positionEnd);
  delay(positionDelay);  
  digitalWrite(ledPin, LOW); 
}
void tremble(){
  for(int i =0; i< trembleCount; i++){
    servo.write(trembles[i]);
    delay(trembleDelay);
  }
}

void loop(){
  moveToStart();
  tremble();
  moveToEnd();
}
