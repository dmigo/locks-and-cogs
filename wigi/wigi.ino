#include <TimerOne.h> 
#include <Encod_er.h>
#include "Sensor.cpp"
#include "Diana.cpp"
#include "RfidLock.cpp"

#define UID 9472  //айдишник нужной нфцшки

#define CLOCKWISE 7 //пин релешки на движение по часовой
#define COUNTERCLOCKWISE 8 //пин релешки на движение против часовой

#define ZERO 12 //пин стартового положения

#define ENCODER_1 3
#define ENCODER_2 4

Encod_er encoder(ENCODER_1, ENCODER_2, 5);

RfidLock *_rfid;
Motor *_motor;
Diana *_diana;

void setup() {
  Serial.begin(9600);
  while (!Serial) {;}

  Serial.println("Version 1.0.10");
  Serial.println("Initializing...");

  _rfid = new RfidLock(UID);
  _rfid->onOpen(onRfidOpen);
  _rfid->onClose(onRfidClose);
  _motor = new Motor(CLOCKWISE, COUNTERCLOCKWISE, ZERO);
  _diana = new Diana(_motor, getEncoderPosition);

  Timer1.initialize(250); // инициализация таймера 1, период 250 мкс 
  Timer1.attachInterrupt(encoderInterrupt, 250); // задаем обработчик прерываний 

  Serial.println("Start...");
}

void onRfidOpen(){
  Serial.println("RFID received!");
  Serial.println(_rfid->getUid());
  _diana->speak();
}

void onRfidClose(){
  Serial.println("RFID removed!");
  Serial.println(_rfid->getUid());
  _diana->signOff();
}

int getEncoderPosition(){
  return encoder.read();
}

void loop() {
  _rfid->check();
  _motor->check();
  _diana->check();
  Serial.println("Encoder:");
  Serial.println(getEncoderPosition());
}

void encoderInterrupt() { 
  encoder.scanState(); 
}
