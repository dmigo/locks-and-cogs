//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___ 
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __| 
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/ 
// 
// The Unnamed Circuit
// 
// Made by Dmitry Goryunov
// License: CC-BY-SA 3.0
// Downloaded from: https://circuits.io/circuits/3955347-the-unnamed-circuit

#include <LiquidCrystal.h>

int time = 3600;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Explosion in:");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  // print the number of seconds since reset:
  int out = millis() / 1000;
  int left = time - out;
  int mins = left / 60;
  int secs = left % 60;
  printTime(mins, secs);
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
