
const int length = 6;//количество сенсоров
int sensors1[length] = {12, 9, 6, A0, A1, A3};//список сенсоров сторона 1
int sensors2[length] = {12, 9, 6, A0, A1, A3};//список сенсоров сторона 2
int lights1[length] = {12, 9, 6, A0, A1, A3};//список подсветки 1
int lights2[length] = {11, 8, 5, 13, A2, A4};//список подсветки 2

int relays[2] = {A5, 1};// релешки

bool won = false;// победа
int index = 0;
const int slength = 6;//длинна правильной последовательности
int sequence[slength] = {1, 5, 3, 5, 2, 4};//правильная последовательность

int side1 = -1; //активированный сенсор 1
int side2 = -1; //активированный сенсор 2

void start(){
  Serial.println("[Starting the game]");
  index = 0;
  won = false;
  
  restrainTheKraken();
}

void next(){
  index++;
      
  if(index == slength){
    lightsOut();
    win();
  }
}

void win(){
  Serial.println("[Victory!]");
  won = true;
  releaseTheKraken();
}

void printSuccess(int i, int expected){
  Serial.print("[index1: ");
  Serial.print(i);
  Serial.print(" expected: ");
  Serial.print(expected);
  Serial.println("]");
}

void printFail(int i, int expected, int actua1, int actual2){
  Serial.print("[index: ");
  Serial.print(i);
  Serial.print(" expected: ");
  Serial.print(expected);
  Serial.print(" actual: ");
  Serial.print(actua1);
  Serial.print(" ");
  Serial.print(actual2);
  Serial.println("]");
}

bool match(int expected, int one, int another){
    return (expected == one && expected == another);
}

void lose(){
  Serial.println("[You've lost!]");
  start();
}

void releaseTheKraken(){
  digitalWrite(relays[0], HIGH);
  digitalWrite(relays[1], HIGH);
}

void restrainTheKraken(){
  digitalWrite(relays[0], LOW);
  digitalWrite(relays[1], LOW);
}

void lightsOut(){
  //todo use register  
  for(int i=0; i<length; i++){
    digitalWrite(lights1[i], LOW);
    digitalWrite(lights2[i], LOW);
  }

  Serial.print("[All lights out]");
}

void lightOut(int address){
  //todo use register
  digitalWrite(address, LOW);
  Serial.print("[light ");
  Serial.print(address);
  Serial.println(" is down]");
}

void lightIn(int address){
  //todo use register
  digitalWrite(address, HIGH);
  Serial.print("[light ");
  Serial.print(address);
  Serial.println(" is up]");
}

int getSensor(int sensors[], int length){
  for(int i = 0; i < length; i++)
    if(digitalRead(sensors[i]) == LOW)
      return i;

  return -1;
}

int updateSensor(int sensors[], int lights[], int length, int oldone){
    int newone = getSensor(sensors, length);
    if(oldone != newone){
      if(oldone != -1)
        lightOut(lights[oldone]);
      if(newone != -1)
        lightIn(lights[newone]);

      return newone;
    }
    return oldone;
}

void setup() {  
  Serial.begin(9600);
  while (!Serial) {
    ;
  }  
  
  for(int i = 0; i<length; i++){
    //todo init leds
    pinMode(sensors1[i], INPUT_PULLUP);
    pinMode(sensors2[i], INPUT_PULLUP);
  }

  pinMode(relays[0], OUTPUT);
  pinMode(relays[1], OUTPUT);

  start();
}

void loop() {
    if(!won){
        int desired = sequence[index];
                
        side1 = updateSensor(sensors1, lights1, length, side1);
        side2 = updateSensor(sensors2, lights2, length, side2);
        
        if(match(desired, side1, side2)){
            printSuccess(index, desired);
            next();
        }
        else if(side1 != -1 && side2 != -1){
            printFail(index, desired, side1, side2);
            lose();
        }
    }
}
