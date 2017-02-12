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

const int time = 3600;
const int buzzerPin = 10;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup() {
  Serial.begin(9600);
  
  pinMode(buzzerPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Explosion in:");
}



void loop() {
  printKey();
  //int out = millis() / 1000;

 //int left = time - out;
 //int mins = left / 60;
 //int secs = left % 60;
 //printTime(mins, secs);
}

void printKey(){
char keypressed = myKeypad.getKey();
if (keypressed != NO_KEY)
    Serial.print(keypressed);
}

void buzz(){
  tone(buzzerPin, 1000);
    delay(100);
    noTone(buzzerPin); 
    delay(1000);
}

void printTime(int mins, int secs){
  lcd.setCursor(6, 1);
  if(mins/10==0)  
    lcd.print("0"); 
  lcd.print(mins);
  lcd.print(":");
  if(secs/10==0)
    lcd.print("0"); 
  lcd.print(secs);
}
