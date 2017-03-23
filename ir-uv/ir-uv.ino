
//=== задача 1
#define UV_DELAY 1000 //задержка ультрафиолета в миллисекундах
const int length = 3;//количество кнопок
int buttons[length] = {12, 9, 6};//список кнопок
int irleds[length] = {11, 8, 5};//список ик
int uvleds[length] = {10, 7, 4};//список ультрафиолеток

int index = 0;
const int slength = 3;//длинна правильной последовательности
int sequence[slength] = {0, 1, 2};//правильная последовательность

void setup() {
  for(int i = 0; i<length; i++){
  	pinMode(irleds[i], OUTPUT);
  	pinMode(uvleds[i], OUTPUT);
  	pinMode(buttons[i], INPUT_PULLUP);
  }
  
  Serial.begin(9600);
  while (!Serial) {
    ;
  }  
  
  start();
}

void start(){
  Serial.println("[Starting the game]");
  index = 0;
  
  for(int i = 0; i<length; i++)
    irdrop(i);
  
  irup(sequence[index]);
}

void irdrop(int i){
  int pin = irleds[i];
  digitalWrite(irleds[i], LOW);
  Serial.print("[ir ");
  Serial.print(pin);
  Serial.println(" is down]");
}

void irup(int i){
  int pin = irleds[i];
  digitalWrite(irleds[i], HIGH);
  Serial.print("[ir ");
  Serial.print(pin);
  Serial.println(" is up]");
}

void uvblink(int i){
  digitalWrite(uvleds[i], HIGH);
  delay(UV_DELAY);
  digitalWrite(uvleds[i], LOW);
}

void win(){
  Serial.println("[You have won]");
  while(true)
    for(int i = 0;i<length;i++){
      digitalWrite(irleds[i], HIGH);
      digitalWrite(uvleds[i], HIGH);
      delay(500);
      digitalWrite(irleds[i], LOW);
      digitalWrite(uvleds[i], LOW);
      delay(500);
    }
}

void loop() {
  int desired = sequence[index];//todo check if out of scope
  int pressed = getButton();
    
  if(pressed == desired){
    printState(index, desired, pressed);
    irdrop(sequence[index]);
  	index++;
    
    uvblink(desired);
    
    if(index == slength)
      win();
    else
      irup(sequence[index]);
  }
  else if(pressed != -1){
    printState(index, desired, pressed);
  	start();
  }
}

void printState(int i, int expected, int actual){
  Serial.print("[index: ");
  Serial.print(i);
  Serial.print(" expected: ");
  Serial.print(expected);
  Serial.print(" actual: ");
  Serial.print(actual);
  Serial.println("]");
}


int getButton(){
  for(int i=0; i<length; i++)
    if(digitalRead(buttons[i]) == LOW)
      return i;
  return -1;
}
