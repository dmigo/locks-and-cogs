const int lockPin = 2;
const int motorPin = 12;
const int motorLedPin = 11;
const int off = HIGH;         
const int on = LOW;         
const int statePins[] = {3,4,5};
const int powerPin = 9;
bool isLocked = false;

void setup() {
  pinMode(lockPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(motorLedPin, OUTPUT);

  pinMode(statePins[0], INPUT_PULLUP);
  pinMode(statePins[1], INPUT_PULLUP);
  pinMode(statePins[2], INPUT_PULLUP);
  pinMode(powerPin, INPUT_PULLUP);
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  
}

void loop() {
  stopMotor();
  if(!isLocked){
        lock();
  }
  
  if(digitalRead(powerPin)==on){
    startMotor();
    delay(2000);
    
    bool isToBeUnlocked = getCogsState();
    if (isToBeUnlocked) {
      delay(8000);
      unlock();
      stopMotor();      
      delay(10000);    
    }
    else
      stopMotor();
  }
  
}

bool getCogsState(){
  bool state1 = digitalRead(statePins[0]) == on;
  bool state2 = digitalRead(statePins[1]) == on;
  bool state3 = digitalRead(statePins[2]) == on;

  bool result = state1 && state2 && state3;

  if(result)
    Serial.write("[all buttons] ");
  else
    Serial.write("[not all buttons] ");

  return result;
}

void startMotor(){
    digitalWrite(motorPin, on);
    digitalWrite(motorLedPin, HIGH);
    Serial.write("started ");  
}
void stopMotor(){
    digitalWrite(motorPin, off);
    digitalWrite(motorLedPin, LOW);
}

void lock(){
    digitalWrite(lockPin, on);
    isLocked = true;
    Serial.write("locked ");
}
void unlock(){
    digitalWrite(lockPin, off);
    isLocked=false;
    Serial.write("unlocked ");
}
