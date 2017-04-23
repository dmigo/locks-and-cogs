//todo порефакторить

#define IR_FLICKER 500 //задержка мерцания ик
#define UV_DELAY 1000 //задержка ультрафиолета в миллисекундах

const int length = 6;//количество кнопок
int buttons[length] = {12, 9, 6, A0, A1, A3};//список кнопок
int irleds[length] = {11, 8, 5, 13, A2, A4};//список ик
bool irstates[length] = {false, false, false, false, false, false}; //состояния икшек
int uvleds[length] = {10, 7, 4, 3, 2, 0};//список ультрафиолеток

int relays[2] = {A5, 1};// релешки

//=== задача 1

bool won1 = false;// победа в первом раунде

int lastButton = -1;

int index1 = 0;
const int slength1 = 6;//длинна правильной последовательности
int sequence1[slength1] = {5, 4, 3, 2, 1, 0};//правильная последовательность

void start1(){
  Serial.println("[Starting the first game]");
  index1 = 0;
  won1 = false;
  lastButton = -1;

  restrainTheKraken();
  
  for(int i = 0; i<length; i++)
    irdrop(i);
  
  irup(sequence1[index1]);
}

void printState(int i, int expected, int actual){
  Serial.print("[index1: ");
  Serial.print(i);
  Serial.print(" expected: ");
  Serial.print(expected);
  Serial.print(" actual: ");
  Serial.print(actual);
  Serial.println("]");
}

void drop1(int one){
    irdrop(sequence1[index1]);
    uvblink(one);
}

void up1(){
    if(index1 <= slength1)
      irup(sequence1[index1]);
}

void next1(){
    index1++;
        
    if(index1 == slength1)
      win1();
}

void win1(){
  Serial.println("[Round 1 win!]");
  won1 = true;
  start2();
}
//=== задача 1

//=== задача 2

bool won2 = false;// победа во втором раунде
int index2 = 0;
const int slength2 = 3;//длинна правильной последовательности
int sequence2[slength2][2] = {{1,5}, {3,5}, {2,4}};//правильная последовательность

void start2(){
  Serial.println("[Starting the second game]");
  index2 = 0;
  won2 = false;
  
  for(int i = 0; i<length; i++){
    irdrop(i);
  }
  
  irup(sequence2[index2][0]);
  irup(sequence2[index2][1]);
}

void printState2(int i, int expected[], int actual[]){
  Serial.print("[index1: ");
  Serial.print(i);
  Serial.print(" expected: ");
  Serial.print(expected[0]);
  Serial.print(" ");
  Serial.print(expected[1]);
  Serial.print(" actual: ");
  Serial.print(actual[0]);
  Serial.print(" ");
  Serial.print(actual[1]);
  Serial.println("]");
}


bool match2(int expected[], int actual[]){
    return (expected[0] == actual[0] && expected[1] == actual[1])
        || (expected[0] == actual[1] && expected[1] == actual[0]);
}

void drop2(int one, int another){
    irdrop(sequence2[index2][0]);
    irdrop(sequence2[index2][1]);
    uvsblink(one, another);
}

void next2(){
    index2++;
        
    if(index2 == slength2)
      win2();
}

void up2(){   
    if(index2 <= slength2){
      irup(sequence2[index2][0]);
      irup(sequence2[index2][1]);
    }
}

void win2(){
  Serial.println("[Round 2 win!]");
  won2 = true;
  win();
}
//=== задача 2

//=== общее

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

void irdrop(int i){
  irstates[i] = false;
  Serial.print("[ir ");
  Serial.print(irleds[i]);
  Serial.println(" is down]");
}

void irup(int i){
  irstates[i] = true;
  Serial.print("[ir ");
  Serial.print(irleds[i]);
  Serial.println(" is up]");
}

void lightup(){  
  for(int i; i < length; i++)
  {
    if(irstates[i])
      digitalWrite(irleds[i], HIGH);
    else
      digitalWrite(irleds[i], LOW);
  }
}

void shutdown(){  
  for(int i; i < length; i++)
      digitalWrite(irleds[i], LOW);
}

void uvblink(int i){
  digitalWrite(uvleds[i], HIGH);
  delay(UV_DELAY);
  digitalWrite(uvleds[i], LOW);
}

void uvsblink(int i, int j){
  digitalWrite(uvleds[i], HIGH);
  digitalWrite(uvleds[j], HIGH);
  delay(UV_DELAY);
  digitalWrite(uvleds[i], LOW);
  digitalWrite(uvleds[j], LOW);
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

  start1();
}

long irstate = 0;
void loop() {
  if(!won2){
    long newirState = millis()/IR_FLICKER;
    if(newirState!=irstate){
      if(newirState%2 == 1)
        lightup();
      else
        shutdown();
    }
  }
    if(!won1){
        int desired = sequence1[index1];
        int newButton = getButton();
        
        if(newButton != lastButton){
          if(lastButton == desired){
              printState(index1, desired, lastButton);
              drop1(lastButton);
              next1();
              up1();
          }
          else if(lastButton != -1){
              printState(index1, desired, lastButton);
              drop1(lastButton);
              lose();
          }

          lastButton = newButton;
        }
    }
    else if(!won2) {        
        int desired[2] = {sequence2[index2][0], sequence2[index2][1]};
        int pressed[2] = {-1,-1};
        getButtons(pressed);
            
        if(match2(desired, pressed)){
            printState2(index2, desired, pressed);
          	drop2(pressed[0], pressed[1]);
          	next2();
          	up2();
        }
        else if(pressed[0] != -1 && pressed[1] != -1){
            printState2(index2, desired, pressed);
          drop2(pressed[0], pressed[1]);
          	lose();
        }
    }
    else{
      if(getButton() != -1)
        lose();
    }
}
//=== общее
