#include <SPI.h>
#include <MFRC522.h>

#define DELAY 2100 //задержка между буквами

#define UID 9472  //айдишник нужной нфцшки

#define CLOCKWISE 7 //пин релешки на движение по часовой
#define COUNTERCLOCKWISE 8 //пин релешки на движение против часовой
 
#define SS_PIN 10
#define RST_PIN 9

bool freeze = false; //после одного удачного считывания выставляем в тру и больше не слушаем нфц
const int letters_l = 6; //длинна последовательности букв
const int letters[letters_l] = {3, 4, 2, 5, 2, 6}; // пины букв {d, i, a, n, a, zero}
const int directions[letters_l] = {CLOCKWISE, CLOCKWISE, COUNTERCLOCKWISE, CLOCKWISE, COUNTERCLOCKWISE, COUNTERCLOCKWISE};

MFRC522 mfrc522(SS_PIN, RST_PIN); // рфид ридер

void setup() {  
  for(int i = 0; i< letters_l; i++){
    pinMode(letters[i], INPUT_PULLUP);
  }
  
  pinMode(COUNTERCLOCKWISE, OUTPUT);
  digitalWrite(COUNTERCLOCKWISE, HIGH);  //Ну релеееешки жееееж))
  pinMode(CLOCKWISE, OUTPUT);
  digitalWrite(CLOCKWISE, HIGH);
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  

  Serial.println("Initializing...");
  
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

    mfrc522.PICC_ReadCardSerial();

    return uidDec;       
  }
}

void moveTo(int destination, int direction){ // движемся к букве

  Serial.print("moving pin ");
  Serial.print(direction);
  Serial.print(" to ");
  Serial.println(destination);

  bool moving = true;// индикатор движения
  digitalWrite(direction, LOW);
  
  while(digitalRead(destination) != HIGH) // ждем пока не дойдем до буквы
  {    
    if(moving){
      if(readUid() != UID){
        digitalWrite(direction, HIGH);
        moving = false;
      }
    }
    else{
      if(readUid() == UID){
        digitalWrite(direction, LOW);
        moving = true;
      }
    }
  }
}

void loop() {
  if(!freeze){     
    int uid = readUid();
    Serial.print("uid:");
    Serial.println(uid);
  
    if(uid == UID){
      Serial.println("Here comes Diana!");
      
      for(int i = 0; i< letters_l; i++){
        moveTo(letters[i], directions[i]);
        delay(DELAY);
      }

      freeze = true;
    } 
  }
 }
