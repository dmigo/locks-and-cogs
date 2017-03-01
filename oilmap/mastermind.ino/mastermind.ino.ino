#define INACTIVE 0 
#define ACTIVE 1 

#define CONTENT 0 
#define CAPACITY 1 

#define IS_ACTIVE 0 
#define ADDRESS 1  
#define ADDRESS_ADD 2 
#define SPEED 3 
#define FROM 4 
#define TO 5

#define WELLID 0
#define MARKETID 1
#define STORAGEID 2

#define DAY 2

#include <Wire.h>

//{wellId, marketId, storageId,}
int nodes [][] = {
  {-1, -1, 0},
  {-1, -1, 1},
  {-1, -1, 2},
};

int wells [][] ={
  {},
  {},  
};

int markets [][] ={
  {},
  {},  
};

//{content, capacity}
int storages [3][2] ={
  {300, 300},
  {0, 100},
  {50, 300},
};

//{isActive, address, speed, from, to}
int pipesCount = 2;
int pipes [pipesCount][5] ={
  {INACTIVE, 7, 0, 2000, 0, 1},
  {INACTIVE, 8, 0, 500, 1, 2}
};

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
}

//=== PIPES ===//
void activatePipe(int index){
  int speed = pipes[index][SPEED];
  
  pipes[index][IS_ACTIVE] = ACTIVE;//ToDo set -ACTIVE for reverse
    
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

void pump(int from, int to, int speed){
  
}

//=== PIPES ===//

void loop() {
  doTick();
  doTock();
  delay(100); 
}

//=== TIME ===//
int seconds = 0;
bool tick = false;

void dotick(){
  var newTime = millis() / 1000;
  if(newTime - seconds >= DAY)
  {
      tick = true;
      seconds = newTime;
  }
}

void doTock(){
  if(tick){
    for(int i = 0; i < pipesCount;
    {
      if(pipes[i][IS_ACTIVE] == ACTIVE)
        pump(pipes[i][FROM],pipes[i][TO], pipes[SPEED]);
        
      if(pipes[i][IS_ACTIVE] == -ACTIVE)
        pump(pipes[i][TO],pipes[i][FROM], pipes[SPEED]);
    }
    //ToDo do stuff
    tick = false;
  }
}
//=== TIME ===//

