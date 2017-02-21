//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___ 
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __| 
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/ 
// 
// Ma Baby
// 
// Made by Dmitry Goryunov
// License: CC-BY-SA 3.0
// Downloaded from: https://circuits.io/circuits/4065601-ma-baby

/*************************************************
 * Notes
 *************************************************/

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#define NOTE_SUSTAIN 250

/*************************************************
 * Notes
 *************************************************/

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
int time = totalTime;
bool tick = false;

int beepterval = 30;
int nextBeep = totalTime-beepterval;
int ibeep = 0;
int unbeep = 0;
bool beeping = false;

const int buzzerPin = 10;

bool nextKey = false;
bool disarmed = false;
bool exploded = false;
int attempts = 3;
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
  if(exploded || disarmed)
    return;
  
  if(time<=0){
  	loose();
    return;
  }
  
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

  if(time <= nextBeep)
  {
    nextBeep = time - beepterval;
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
  
  if(keypressed == combi[combindex])
    combindex++;
  else
  {
    combindex = 0;
    attemptFailed();
  }
  
  Serial.print('[');
  Serial.print(keypressed);
  Serial.print(':');
  Serial.print(combindex);
  
  if(combindex == combilen)
    win();
  
  Serial.print(']');
}

void attemptFailed(){
  attempts-=1;
  Serial.print("<Attempts: ");
  Serial.print(attempts);
  Serial.print(">");
  Serial.println("");
  if(attempts ==0)
    loose();
  else{
    unbeep = time - 2;
    beep();
  }
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

void win(){
  
  disarmed = true;
  
  tone(buzzerPin,NOTE_A5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_B5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_C5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_B5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_C5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_D5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_C5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_D5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_E5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_D5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_E5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_E5);
  delay(NOTE_SUSTAIN);
  noTone(buzzerPin);
}
void loose(){
  exploded = true;

  tone(buzzerPin,NOTE_G4);
  delay(250);
  tone(buzzerPin,NOTE_C4);
  delay(500);
  noTone(buzzerPin);
}
