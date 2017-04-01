
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

void start(){
  Serial.println("[Starting the game]");
  index = 0;
  won = false;
  
  restrainTheKraken();
}

void next(){
    irdrop(sequence1[index1]);
    uvblink(sequence1[index1]);
    index1++;
        
    if(index1 == slength1)
      win1();
    else
      irup(sequence1[index1]);
}

void win(){
  Serial.println("[Victory!]");
  won = true;
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
  Serial.print(actual1);
  Serial.print(" ");
  Serial.print(actual2);
  Serial.println("]");
}


bool match(int expected, int one, int another){
    return (expected[0] == actual[0] && expected[1] == actual[1]);
}

void lose(){
  start1();
}

void win(){
  releaseTheKraken();
}

void releaseTheKraken(){
  digitalWrite(relays[0], HIGH);
  digitalWrite(relays[1], HIGH);
}

void restrainTheKraken(){
  digitalWrite(relays[0], LOW);
  digitalWrite(relays[1], LOW);
}

int getButton(){
  for(int i=0; i<length; i++)
    if(digitalRead(buttons[i]) == LOW)
      return i;
  return -1;
}

void getButtons(int result[]){
    result[0] = -1;
    result[1] = -1;
    int j=0;

  for(int i=0; i<length; i++){
    if(j<2){
        if(digitalRead(buttons[i]) == LOW){
            result[j] = i;
            j++;
        }
    }
  }
}

void lightOut(int address){
  digitalWrite(address, LOW);
  Serial.print("[light ");
  Serial.print(address);
  Serial.println(" is down]");
}

void lightIn(int address){
  digitalWrite(address, HIGH);
  Serial.print("[light ");
  Serial.print(address);
  Serial.println(" is up]");
}

void setup() {  
  Serial.begin(9600);
  while (!Serial) {
    ;
  }  
  
  for(int i = 0; i<length; i++){
    pinMode(irleds[i], OUTPUT);
    pinMode(uvleds[i], OUTPUT);
    pinMode(buttons[i], INPUT_PULLUP);
  }

  pinMode(relays[0], OUTPUT);
  pinMode(relays[1], OUTPUT);

  start();
}

int side1 = -1;
int side2 = -1;

void loop() {
    if(!won){
        int desired = sequence[index];
        int newSide1 = getSide1();
        int newSide2 = getSide2();

        if(side1 != newSide1){
          if(side1 != -1)
            lightOut(side1);
          if(newSide1 != -1)
            lightIn(newSide1);

          side1 = newSide1;
        }

        if(side2 != newSide2){
          if(side2 != -1)
            lightOut(side2);
          if(newSide2 != -1)
            lightIn(newSide2);

          side2 = newSide2;
        }
        
        if(match(desired, side1, side2)){
            printSuccess(index, desired);
            next2();
        }
        else if(side1 != -1 && side2 != -1){
            printFail(index, desired, side1, side2);
            lose();
        }
    }
    else{
      if(getButton() != -1)
        lose();
    }
}
