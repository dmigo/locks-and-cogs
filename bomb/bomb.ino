//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___ 
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __| 
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/ 
// 
// Ma Baby
// 
// Made by Dmitry Goryunov
// License: CC-BY-SA 3.0
// Downloaded from: https://circuits.io/circuits/3955347-ma-baby

#include <LiquidCrystal.h>
#include <Keypad.h>

const byte numRows= 4; 
const byte numCols= 4; 

char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

byte rowPins[numRows] = {6,7,8,9};
byte colPins[numCols]= {A0,A1,A2,A3}; 

const int totalTime = 3600; // in decisecs
int time = 0;
bool tick = false;

//3600, 3300, 3000, 2700, 2400, 2100, 1800, 1500, 1200, 900, 600, 300, 30, 20, 10
int beeps [] = { 3600, 3300, 3000, 2700, 2400, 2100, 1800, 1500, 1200, 900, 600, 300, 30, 20, 10 };
int ibeep = 0;
int unbeep = 0;
bool beeping = false;

const int buzzerPin = 10;

bool nextKey = false;
bool disarmed = false;
const int combilen = 5;
char combi [combilen] = {'3','2','1','6','7'};
int combindex = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup() {
	Serial.begin(9600);
  
  pinMode(buzzerPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Explosion in:");
}

void loop() {
  if(time<0 || disarmed)
    return;
  
  tickTock();
  checkEvents();
  listenKey(); 
}

void tickTock(){
	int out = millis() / 100;
  int newTime = totalTime - out;
  tick = newTime != time;
  time = newTime;
}

void checkEvents(){
  if(!tick)
    return;
    
	if(time <= beeps[ibeep])
    {
      ibeep++;
      unbeep = time - 1;
      beep();
    }
	if(time <= unbeep && beeping)
    {
      dontbeep();
    }
  
  printTime();
}

void listenKey(){
  char keypressed = myKeypad.getKey();
  
  if (keypressed == NO_KEY)
  {
    nextKey = true;
    return;
	}
  
  if(!nextKey)
    return;
  
  nextKey = false;
  
  Serial.print(keypressed);
  
  if(keypressed == combi[combindex])
    combindex++;
  else
    combindex = 0;
  
  if(combindex == combilen)
    disarmed = true;  
}

void beep(){
  beeping = true;
  Serial.println("beep!");
  Serial.println(ibeep);
	tone(buzzerPin, 1000);
}
void dontbeep(){
  beeping = false;
  Serial.println("unbeep!");
  	noTone(buzzerPin); 
}

void printTime(){
  int mins = time / 600;
  int secs = time % 600 / 10;
  lcd.setCursor(6, 1);
  if(mins/10==0)  
  	lcd.print("0"); 
  lcd.print(mins);
  lcd.print(":");
  if(secs/10==0)
  	lcd.print("0"); 
  lcd.print(secs);
}
