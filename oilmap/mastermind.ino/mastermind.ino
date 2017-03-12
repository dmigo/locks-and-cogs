#define CONTENT 0 
#define CAPACITY 1 

#define PROGRESS 0 
#define ADDRESS 1  
#define ADDRESS_ADD 2 
#define DELAY 3 
#define FROM 4 
#define TO 5
#define CONTENT 6
#define TARGET 7

#define WELLID 0
#define MARKETID 1
#define STORAGEID 2

#define DAY 2

#include <Wire.h>

//{wellId, marketId, storageId, pipe 1, pipe 2, pipe 3}
int nodes [3][3] = {
  {-1, -1, 0, 0, -1, -1},
  {-1, -1, 1, 0, 1, -1},
  {-1, -1, 2, 1, -1, -1},
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

//{progress, address, delay, from, to, content, target}
const int pipesCount = 2;
int pipes [pipesCount][8] = {
  {0, 7, 0, 2, 0, 1, 0, 0},
  {0, 8, 0, 3, 1, 2, 0, 0}
};

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
}

//=== PIPES ===//
void send(int index, int quantity, bool reverse){
  if(pipes[index][PROGRESS] != 0)
    return;

  pipes[index][PROGRESS] = pipes[index][DELAY];
  pipes[index][CONTENT] = quantity;//ToDo consider reverse
    
  Wire.beginTransmission(pipes[index][ADDRESS]);
  Wire.write(pipes[index][TARGET]);
  Wire.write(1);//ToDo consider reverse
  Wire.endTransmission();

  spend(pipes[index][FROM], pipes[index][CONTENT]);//ToDo consider reverse
  //ToDo consider not complete quantity
}

void pump(int index){
  Serial.println();
  Serial.print("[pipe ");
  Serial.print(index);
  Serial.print("] ");
  Serial.print(pipes[index][PROGRESS]);
  pipes[index][PROGRESS]-=1;
  Serial.print("-->");
  Serial.print(pipes[index][PROGRESS]);
  
  if(pipes[index][PROGRESS] == 0)
    receive(index);//ToDo consider reverse
}

void receive(int index){
  pipes[index][IS_ACTIVE] = INACTIVE;
  
  Wire.beginTransmission(pipes[index][ADDRESS]);
  Wire.write(pipes[index][TARGET]);
  Wire.write(0);
  Wire.endTransmission();

  obtain(pipes[index][TO], pipes[index][CONTENT]);//ToDo consider reverse
}

void spend(int object, int quantity){
  Serial.println();
  if(nodes[object][WELLID]>=0){
    //TODO implement well   
    Serial.print("well");
  }
  if(nodes[object][MARKETID]>=0){
    Serial.print("market");
    //TODO implement market
  }
  if(nodes[object][STORAGEID]>=0){
    Serial.print("[storage ");
    int index = nodes[from][STORAGEID];
    Serial.print(index);
    Serial.print("] -- ")
    storages[index][CONTENT] -= quantity;
    Serial.print(quantity);
  }
}

void obtain(int object, int quantity){
   Serial.println();
  if(nodes[object][WELLID]>=0){
    //TODO implement well   
    Serial.print("well");
  }
  if(nodes[object][MARKETID]>=0){
    Serial.print("market");
    //TODO implement market
  }
  if(nodes[object][STORAGEID]>=0){
    Serial.print("[storage ");
    int index = nodes[from][STORAGEID];
    Serial.print(index);
    Serial.print("] ++ ")
    storages[index][CONTENT] += quantity;
    Serial.print(quantity);
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
      if(pipes[i][PROGRESS] > 0){
          pump(i);
      }
    }
    //ToDo do stuff
    tick = false;
  }
}
//=== TIME ===//

