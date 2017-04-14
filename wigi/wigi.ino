#include <SPI.h>
#include <MFRC522.h>

#define DELAY 1100 //задержка между буквами

#define UID 32167 //айдишник нужной нфцшки

#define CLOCKWISE 1 //пин релешки на движение по часовой
#define COUNTERCLOCKWISE 1 //пин релешки на движение против часовой
 
#define SS_PIN 8
#define RST_PIN 7

bool freeze = false; //после одного удачного считывания выставляем в тру и больше не слушаем нфц
const int letters_l = 5; //длинна последовательности букв
const int letters[letters_l] = {6, 7, 5, 8, 5}; // пины букв {d, i, a, n, a}
const int directions[letters_l] = {CLOCKWISE, CLOCKWISE, COUNTERCLOCKWISE, CLOCKWISE, COUNTERCLOCKWISE};

MFRC522 mfrc522(SS_PIN, RST_PIN); // рфид ридер

void setup() {  
  for(int i = 0; i< letters_l; i++){
    pinMode(letters[i], INPUT_PULLUP);
  }
  
  pinMode(COUNTERCLOCKWISE, OUTPUT);
  digitalWrite(COUNTERCLOCKWISE, LOW);
  pinMode(CLOCKWISE, OUTPUT);
  digitalWrite(CLOCKWISE, LOW);
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  
  
  SPI.begin();            // инициализация SPI
  mfrc522.PCD_Init();     // инициализация MFRC522
  Serial.println("Start...");
}

int readUid(){// читаем ид карточки
  byte status;
  byte byteCount;
  byte buffer[2]; // длина массива (16 байт + 2 байта контрольная сумма) 
  
  byteCount = sizeof(buffer);
  int uidDec = 0;
  int uidDecTemp = 0;
  status = mfrc522.PICC_RequestA(buffer, &byteCount); 
  if (mfrc522.PICC_ReadCardSerial()) {
    for (byte i = 0; i < mfrc522.uid.size; i++) {  
      uidDecTemp=mfrc522.uid.uidByte[i];
      uidDec=uidDec*256+uidDecTemp;
    }

    return uidDec;
    //todo ??? mfrc522.PICC_ReadCardSerial();    // не факт что нужно       
  }
}

void moveTo(int destination, int direction){ // движемся к букве
    digitalWrite(direction, HIGH);
    while(digitalRead(destination) != LOW) // ждем пока не дойдем до буквы
      {;}
    digitalWrite(direction, LOW);
}

void loop() {
  int uid = readUid();
  Serial.print("uid:");
  Serial.println(uid);

  if(uid == UID 
    && !freeze){
    Serial.println("Here comes Diana!");
    freeze = true;
    for(int i = 0; i< letters_l; i++){
      Serial.print("Move to pin:");
      Serial.println(letters[i]);
      moveTo(letters[i], directions[i]);
      delay(DELAY);
    }    
  }
 }
