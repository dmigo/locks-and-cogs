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
int nodes [3][3] = {
  {-1, -1, 0},
  {-1, -1, 1},
  {-1, -1, 2},
};

int wells [2][0] ={
  {},
  {},  
};

int markets [2][0] ={
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
const int pipesCount = 2;
int pipes [pipesCount][6] = {
  {ACTIVE, 7, 0, 2, 0, 1},
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

Serial.println();
Serial.print("speed: ");
Serial.print(speed);
Serial.print(" [");

  int toTransfer = 0;
  if(nodes[from][WELLID]>=0){
    //TODO implement well   
    Serial.print("well");
  }
  if(nodes[from][MARKETID]>=0){
    Serial.print("market");
    //TODO implement market
  }
  if(nodes[from][STORAGEID]>=0){
    Serial.print("storage ");
    int index = nodes[from][STORAGEID];
    int content = storages[index][CONTENT];
    
    Serial.print(index);
    Serial.print(":(");
    Serial.print(content);
    Serial.print(")] >>");

    if(content < speed)
      toTransfer = content;
    if(content >= speed)
      toTransfer = speed;
    
    Serial.print("try: ");
    Serial.print(toTransfer);
    Serial.print(">> to [");
    
    if(nodes[to][WELLID]>=0){ 
      //TODO implement well
      Serial.print("well");
    }
    if(nodes[to][MARKETID]>=0){
      //TODO implement market
      Serial.print("market");
    }
    if(nodes[to][STORAGEID]>=0){
      int toindex = nodes[to][STORAGEID];
      int capacity = storages[toindex][CAPACITY];
      int content = storages[toindex][CONTENT];
      int available = capacity - content;

      if(available <= toTransfer)
        toTransfer = available;
      
      Serial.print("storage ");
      Serial.print(toindex);
      Serial.print(":(");
      Serial.print(content);
      Serial.print("/");
      Serial.print(capacity);
      Serial.print(")] ");
      
      storages[toindex][CONTENT] += toTransfer;
    }

    Serial.print("actual: ");
    Serial.println(toTransfer);
      
    storages[index][CONTENT] -= toTransfer;
  }
    
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

void doTick(){
  int newTime = millis() / 1000;
  if(newTime - seconds >= DAY)
  {
      tick = true;
      seconds = newTime;
  }
}

void doTock(){
  if(tick){
    for(int i = 0; i < pipesCount; i++)
    {
      if(pipes[i][IS_ACTIVE] == ACTIVE)
        pump(pipes[i][FROM],pipes[i][TO], pipes[i][SPEED]);
        
      if(pipes[i][IS_ACTIVE] == -ACTIVE)
        pump(pipes[i][TO],pipes[i][FROM], pipes[i][SPEED]);
    }
    //ToDo do stuff
    tick = false;
  }
}
//=== TIME ===//

