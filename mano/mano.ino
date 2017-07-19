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
const int servoSpan = 400;
int servoState = 0;
int start = 200;
int bak = 0; //ставим переменную "защиты"

Servo servo;

void setup(){
  
  for(int i = 0; i<4; i++)
    pinMode(switches[i], INPUT_PULLUP);
    
  for(int i = 0; i<7; i++){
    pinMode(relays[i], OUTPUT);//по другому нельзя с реле
    digitalWrite(relays[i], HIGH);    
  }
  pinMode(A5, INPUT);
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
  digitalWrite(relays[0], LOW);
  digitalWrite(relays[1], LOW);
  digitalWrite(relays[2], LOW);

  while(servoState < servoEnd){
    moveTo(servoState + servoStep);
    delay(servoSpan);    
  }
}
void phase2(){
  digitalWrite(relays[3], LOW);
  digitalWrite(relays[4], LOW);
  moveTo(servoEnd);
  int time = random(1000, 5000);
  delay(time);
}
void phase3(){
  int time = random(1000, 5000);
  delay(time);
  digitalWrite(relays[5], LOW);
}
void phase4(){
  int time = random(4000, 8000);
  delay(time);
  digitalWrite(relays[6], LOW);
  delay(20000);
  digitalWrite(relays[3], HIGH);
  digitalWrite(relays[4], HIGH);
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
  start = analogRead(A5);  //снимаем наше значение наличия в ааналоге. При стабильном контакте, это даёт 1023 в значении. Но чтобы убрать заранее корозию метала итп. ставим >1000 как верное
  Serial.println(start);//это для отстройки значений
  
  if(start > 1000){
    bak = bak+1;  //досчитай до 100 чтобы убедиться что это не наводка :)
     Serial.println(bak);//смотрим, верно ли считает, нет ли пауз при хорошем контакте.
    }
    else {
    bak = 0;//если приходил "левый" сигнал, сбрасываем
   Serial.println(bak);//и смотрим как сбросили
    }
  if(isSolved() && !done && bak > 100){     //проверяем все условия + 100
    phase1();
    phase2();
    phase3();
    phase4();

    done = true;
  }
 }
