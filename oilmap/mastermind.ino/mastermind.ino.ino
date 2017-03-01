#define INACTIVE 0 
#define ACTIVE 1 

#define CONTENT 0 
#define CAPACITY 1 

#define IS_ACTIVE 0 
#define ADDRESS 1 
#define SPEED 2 
#define FROM 3 
#define TO 1

#include <Wire.h>
//{content, capacity}
int storages [3][2] ={
  {300, 300},
  {0, 100},
  {50, 300},
};

//{isActive, address, speed, from, to}
int pipes [2][5] ={
  {INACTIVE, 7, 2000, 0, 1},
  {INACTIVE, 8, 500, 1, 2}
};

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
}

void activatePipe(int index){
  int speed = pipes[index][SPEED];
  pipes[index][IS_ACTIVE] = ACTIVE;
  Wire.beginTransmission(pipes[index][ADDRESS]);
  Wire.write(speed);
  Wire.write(speed>>8);
  Wire.endTransmission();
}


void deactivatePipe(int index){
  pipes[index][IS_ACTIVE] = INACTIVE;
  Wire.beginTransmission(pipes[index][ADDRESS]);
  Wire.write(0);
  Wire.write(0);
  Wire.endTransmission();
}

void loop() {
  activatePipe(0);
  delay(500);
  activatePipe(1);
  delay(500);
  deactivatePipe(0);
  delay(500);
  deactivatePipe(1);
  delay(500);
}
