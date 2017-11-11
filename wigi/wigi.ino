#include <SPI.h>
#include <MFRC522.h>

#define DELAY 2100 //задержка между буквами

#define UID 9472  //айдишник нужной нфцшки

#define CLOCKWISE 7 //пин релешки на движение по часовой
#define COUNTERCLOCKWISE 8 //пин релешки на движение против часовой

#define ENCODER_1 3
#define ENCODER_2 4

#define ZERO 6 //пин стартового положения

#define SS_PIN 10
#define RST_PIN 9

bool freeze = false; //после одного удачного считывания выставляем в тру и больше не слушаем нфц
const int letters_l = 5;
const int letters[letters_l] = {3, 4, 2, 5, 2}; // количество щелчков для каждой из букв {d, i, a, n, a}
const int directions[letters_l] = {CLOCKWISE, CLOCKWISE, COUNTERCLOCKWISE, CLOCKWISE, COUNTERCLOCKWISE};

MFRC522 mfrc522(SS_PIN, RST_PIN); // рфид ридер

void setup() {
  pinMode(ZERO, INPUT_PULLUP);
  pinMode(ENCODER_1, INPUT_PULLUP);
  pinMode(ENCODER_2, INPUT_PULLUP);
    
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

int readUid() { // читаем ид карточки
  byte status;
  byte byteCount;
  byte buffer[2]; // длина массива (16 байт + 2 байта контрольная сумма)

  byteCount = sizeof(buffer);
  int uidDec = 0;
  int uidDecTemp = 0;
  status = mfrc522.PICC_RequestA(buffer, &byteCount);
  if (mfrc522.PICC_ReadCardSerial()) {
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uidDecTemp = mfrc522.uid.uidByte[i];
      uidDec = uidDec * 256 + uidDecTemp;
    }

    mfrc522.PICC_ReadCardSerial();

    return uidDec;
  }
}

bool moveTo(int clicks_to_destination, int direction) { // движемся к букве

  Serial.print("moving pin ");
  Serial.print(direction);
  Serial.print(clicks_to_destination);
  Serial.println(" clicks");

  digitalWrite(direction, LOW);

  int pos = 0;
  int lastn = LOW;
  int n = LOW;
  
  while (pos < clicks_to_destination) // ждем пока не дойдем до буквы
  {
    n = digitalRead(ENCODER_1);
    if(lastn == LOW && n==HIGH){
      if (digitalRead(ENCODER_2) == LOW) {
        pos--;
      } else {
        pos++;
      }
    }
    lastn = n;
    
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
    int uid = readUid();
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
