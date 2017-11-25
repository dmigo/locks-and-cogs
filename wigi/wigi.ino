#include <rdm630.h>

#define DELAY 2100 //задержка между буквами

#define UID 10775353  //айдишник нужной нфцшки

#define CLOCKWISE 7 //пин релешки на движение по часовой
#define COUNTERCLOCKWISE 8 //пин релешки на движение против часовой

#define ZERO 12 //пин стартового положения

#define CLICKS_BEFORE_LOSE 300

bool freeze = false; //после одного удачного считывания выставляем в тру и больше не слушаем нфц
const int letters_l = 5; //длинна последовательности букв
const int letters[letters_l] = {3, 4, 2, 5, 2}; // пины букв {d, i, a, n, a}
const int directions[letters_l] = {CLOCKWISE, CLOCKWISE, COUNTERCLOCKWISE, CLOCKWISE, COUNTERCLOCKWISE};

rdm630 *_rfid = new rdm630(6, 0);  //TX-pin of RDM630 connected to Arduino pin 6

void setup() {
  
  Serial.println("Version 2.0.1");
  
  for (int i = 0; i < letters_l; i++) {
    pinMode(letters[i], INPUT_PULLUP);
  }
  pinMode(ZERO, INPUT_PULLUP);

  pinMode(COUNTERCLOCKWISE, OUTPUT);
  digitalWrite(COUNTERCLOCKWISE, HIGH);  
  pinMode(CLOCKWISE, OUTPUT);
  digitalWrite(CLOCKWISE, HIGH);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Initializing...");
  _rfid->begin();

  Serial.println("Start...");
}

unsigned long lastUid =0;
int lastSeen=0;
unsigned long readUid() { // читаем ид карточки
  if(_rfid->available())
    {
    lastSeen=0;
  lastUid = getUid();
    return lastUid;
  }
  else
    {
    if(lastSeen<CLICKS_BEFORE_LOSE){
      lastSeen++;
    return lastUid;
    }
     else{
     return 0;
     }
    }
}
 unsigned long getUid(){
  byte data[6];
  byte length;
  
  _rfid->getData(data, length);
  unsigned long result = 
    ((unsigned long int)data[1]<<24) + 
    ((unsigned long int)data[2]<<16) + 
    ((unsigned long int)data[3]<<8) + 
    data[4];

  return result;
}

bool moveTo(int destination, int direction) { // движемся к букве

  Serial.print("moving pin ");
  Serial.print(direction);
  Serial.print(" to ");
  Serial.println(destination);

  digitalWrite(direction, LOW);

  while (digitalRead(destination) != LOW) // ждем пока не дойдем до буквы
  {
    if (readUid() != UID) {
      Serial.println("RFID removed!");
      digitalWrite(direction, HIGH);
      return false;
    }
  }

  Serial.println("got to the point");

  digitalWrite(direction, HIGH);
  return true;
}

void moveToZero() {
  Serial.println("moving to start");

  int blinker = 1;
  int interval = 1500;
  digitalWrite(COUNTERCLOCKWISE, LOW);
  while (digitalRead(ZERO) != LOW) // ждем пока не дойдем до старта
  {
    blinker++;
    if (blinker % (interval * 2) == 0)
      digitalWrite(COUNTERCLOCKWISE, HIGH);
    else if (blinker % interval == 0)
      digitalWrite(COUNTERCLOCKWISE, LOW);
  }
  digitalWrite(COUNTERCLOCKWISE, HIGH);
}

void loop() {
  if (!freeze) {
    unsigned long uid = readUid();
    Serial.print("uid:");
    Serial.println(uid);

    if (uid == UID) {
      Serial.println("Here comes Diana!");

      bool moveon = true;
      for (int i = 0; i < letters_l && moveon; i++) {
        moveon = moveTo(letters[i], directions[i]);
        delay(DELAY);
      }
      moveToZero();

      freeze = true;
    }
  }
}
