#define TIME_TO_WIN 20 //время которое нужно крутить правильные вентили чтобы выиграть
#define TIME_TO_REACT 1 //время которое нужно крутить вентили чтобы вольтметр среагировал 

#define FIRST_BLINK 100 //первое включение реле (децисекунды)
#define BLINK 10 //последующие включения реле (децисекунды)

#define SERVO_START 0 //стартовая позиция серваков

#define GREENZONE_BEGIN 10 //начало зеленой зоны
#define GREENZONE_END 30 //конец зеленой зоны

#define REDZONE_BEGIN 40 //начало красной зоны
#define REDZONE_END 70 //конец красной зоны

#include <Servo.h>

const int wheels_l = 3;
const int wheels1[wheels_l] = {2, 3, 4};
const int wheels2[wheels_l] = {6, 7, 8};

const int relay = 12;

const int voltmeterPin1 = 5;
const int voltmeterPin2 = 9;

bool won = false;

long lastWrong1 = 4294967295;
long lastRight1 = 4294967295;
long lastWrong2 = 4294967295;
long lastRight2 = 4294967295;

Servo voltmeter1;
Servo voltmeter2;

void setup(){
    for(int i = 0; i<wheels_l; i++)
        pinMode(wheels1[i], INPUT_PULLUP);
    for(int i = 0; i<wheels_l; i++)
        pinMode(wheels2[i], INPUT_PULLUP);

    pinMode(relay, OUTPUT);
    digitalWrite(relay, LOW);    

    Serial.begin(9600);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }  

    voltmeter1.attach(voltmeterPin1);
    voltmeter1.write(SERVO_START);

    voltmeter2.attach(voltmeterPin1);
    voltmeter2.write(SERVO_START);
}

bool isWinning(long seconds){
    return seconds - lastRight1 < TIME_TO_WIN
        && seconds - lastRight2 < TIME_TO_WIN
        && seconds - lastWrong1 > TIME_TO_WIN
        && seconds - lastWrong2 > TIME_TO_WIN;
}

bool isGreen(long seconds, long lastRight, long lastWrong){
    return lastRight > lastWrong + TIME_TO_REACT;
}

int getGreen(){
    return random(GREENZONE_BEGIN, GREENZONE_END);
}

bool isRed(long seconds, long lastRight, long lastWrong){
    return  lastWrong > lastRight + TIME_TO_REACT;
}

int getRed(){
    return random(REDZONE_BEGIN, REDZONE_END);
}

void win(){
    won = true;
    digitalWrite(relay, HIGH);
}

void loop(){
    if(won)
        return;

    long seconds = millis()/1000;

    if(isWinning(seconds))
        win();

    if(isGreen(seconds, lastRight1, lastWrong1))
        voltmeter1.write(getGreen());
    if(isRed(seconds, lastRight1, lastWrong1))
        voltmeter1.write(getRed());

    if(isGreen(seconds, lastRight2, lastWrong2))
        voltmeter2.write(getGreen());
    if(isRed(seconds, lastRight2, lastWrong2))
        voltmeter2.write(getRed());

    if(readRight(wheels1))
        lastRight1 = seconds;
    if(readWrong(wheels1))
        lastWrong1 = seconds;

    if(readRight(wheels2))
        lastRight2 = seconds;
    if(readWrong(wheels2))
        lastWrong2 = seconds;    
}

bool readRight(int[] right){
    if(digitalRead(wheels[2]) == LOW){
        Serial.println("RIGHT");
        return true;
    }
    return false;
}

bool readWrong(int[] right){
    for(int i = 0; i<2; i++)
        if(digitalRead(wheels[i]) == LOW){
            Serial.println("WRONG");
            return true;
        }
    return false;
}