#include <LiquidCrystal.h>

const int time = 3600;
const int buzzerPin = 7;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  pinMode(buzzerPin, OUTPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Explosion in:");
}

void loop() {
  buzz();
  //int out = millis() / 1000;

 //int left = time - out;
 //int mins = left / 60;
 //int secs = left % 60;
 //printTime(mins, secs);
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
