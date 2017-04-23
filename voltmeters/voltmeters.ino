#define TIME_TO_WIN 20 //время которое нужно крутить правильные вентили чтобы выиграть
#define TIMEOUT 5

#define FIRST_BLINK 10 //первое включение реле (секунды)
#define BLINK 1 //последующие включения реле (секунды)
#define BLINK_AFTER 100

#define SERVO_START 0 //стартовая позиция серваков

#define GREENZONE_BEGIN 30 //начало зеленой зоны
#define GREENZONE_END 60 //конец зеленой зоны

#define REDZONE_BEGIN 80 //начало красной зоны
#define REDZONE_END 110 //конец красной зоны

#define NONE 0
#define GREEN 1
#define RED 2

#define DEFAULT -32768

#include <Servo.h>

const int wheels_l = 3;
const int wheels1[wheels_l] = {2, 3, 4};
const int wheels2[wheels_l] = {6, 7, 8};

const int winRelay = 12;
const int blinkRelay = 13;
int blinkRelayOff = FIRST_BLINK;
int lastBlinkedAt = 0;

const int voltmeterPin1 = A1;
const int voltmeterPin2 = A0;

bool won = false;

long lastWrong1 = DEFAULT;
long lastRight1 = DEFAULT;
long lastWrong2 = DEFAULT;
long lastRight2 = DEFAULT;

Servo voltmeter1;
int state1 = NONE;
Servo voltmeter2;
int state2 = NONE;
long greenSeconds = 0;

void setup(){
    for(int i = 0; i<wheels_l; i++)
        pinMode(wheels1[i], INPUT_PULLUP);
    for(int i = 0; i<wheels_l; i++)
        pinMode(wheels2[i], INPUT_PULLUP);

    pinMode(blinkRelay, OUTPUT);
    digitalWrite(blinkRelay, LOW);
    pinMode(winRelay, OUTPUT);
    digitalWrite(winRelay, HIGH);

    Serial.begin(9600);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }  

    voltmeter1.attach(voltmeterPin1);
    voltmeter1.write(SERVO_START);

    voltmeter2.attach(voltmeterPin2);
    voltmeter2.write(SERVO_START);

    Serial.println("starting!");
}

bool isWinning(long seconds){
    long some = seconds - TIME_TO_WIN;
    return lastRight1 != DEFAULT
        && lastRight2 != DEFAULT
        && lastRight1 > some
        && lastRight2 > some
        && lastWrong1 < some
        && lastWrong2 < some
        && greenSeconds >= TIME_TO_WIN;
}

bool isGreen(long seconds, long lastRight, long lastWrong){
    return lastRight != DEFAULT
        && lastRight + TIMEOUT > seconds
        && lastRight > lastWrong;
}

int getGreen(){
    Serial.println("GREEN");
    return random(GREENZONE_BEGIN, GREENZONE_END);
}

bool isRed(long seconds, long lastRight, long lastWrong){
    return (lastWrong != DEFAULT
        && lastWrong >= lastRight)
        || lastRight + TIMEOUT < seconds;
}

int getRed(){
    Serial.println("RED");
    return random(REDZONE_BEGIN, REDZONE_END);
}

void win(){
    Serial.println("You've won!");
    won = true;
    digitalWrite(winRelay, LOW);
}

long seconds = 0;
long clicks = 0;

void loop(){
    if(won)
        return;

    long newSeconds = millis()/1000;

    if(newSeconds != seconds){
        seconds = newSeconds;

        if(isWinning(seconds))
            win();
        
        if(seconds == blinkRelayOff)
            digitalWrite(blinkRelay, HIGH);
        
        if(state1 == GREEN && state2 == GREEN)
            greenSeconds++;
        else
            greenSeconds = 0;
    }

    if(isGreen(seconds, lastRight1, lastWrong1))
    {
        if(state1 !=  GREEN){
            voltmeter1.write(getGreen());
            state1 = GREEN;
        }
    }
    if(isRed(seconds, lastRight1, lastWrong1))
    {
        if(state1 !=  RED){
            voltmeter1.write(getRed());
            state1 = RED;
        }
    }

    if(isGreen(seconds, lastRight2, lastWrong2))
    {
        if(state2 !=  GREEN){
            voltmeter2.write(getGreen());
            state2 = GREEN;
        }
    }
    if(isRed(seconds, lastRight2, lastWrong2))
    {
        if(state2 !=  RED){
            voltmeter2.write(getRed());
            state2 = RED;
        }
    }        

    if(readRight(wheels1)){
        lastRight1 = seconds;
        clicks++;
    }
    if(readWrong(wheels1)){
        clicks++;
        lastWrong1 = seconds;
    }        

    if(readRight(wheels2)){
        clicks++;
        lastRight2 = seconds;
    }
    if(readWrong(wheels2)){
        clicks++;
        lastWrong2 = seconds;    
    }
}

bool readRight(const int wheels[]){
    if(digitalRead(wheels[2]) == LOW){
        return true;
    }
    return false;
}

bool readWrong(const int wheels[]){
    for(int i = 0; i<2; i++)
        if(digitalRead(wheels[i]) == LOW){
            return true;
        }
    return false;
}