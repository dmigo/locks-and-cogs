const int lockPin = 6;
const int unlockPin = 7;
const int lockDetectorPin = 8;

const int motorPin = 12;
const int motorLedPin = 11;

const int off = HIGH;         
const int on = LOW;         

const int statePins[] = {3, 4, 5};
const int powerPin = 9;
bool isLocked = false;

void setup() {
  pinMode(lockPin, OUTPUT);
  pinMode(unlockPin, OUTPUT);
  pinMode(lockDetectorPin, INPUT_PULLUP);
  
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
  
  digitalWrite(lockPin, off);
  digitalWrite(unlockPin, off);
  stopMotor();
  lock();
}

void loop() {  
  if(isButtonStart()){
    if(!isLocked){
      lock();
    }
    else {
      if(atLeastOneCog()){
        startMotor();
        delay(2000);

        bool isToBeUnlocked = allCogs();
        if (isToBeUnlocked) {
          delay(8000);
          unlock();    
        }

        stopMotor(); 
      }
    }
  }  
}

bool isButtonStart(){
  return digitalRead(powerPin) == on;
}

bool atLeastOneCog(){  
  bool state1 = digitalRead(statePins[0]) == on;
  bool state2 = digitalRead(statePins[1]) == on;
  bool state3 = digitalRead(statePins[2]) == on;

  bool result = state1 || state2 || state3;

  if(result)
    Serial.write("[at least one cog] ");
  else
    Serial.write("[not a single cog] ");

  return result;
}

bool allCogs(){
  bool state1 = digitalRead(statePins[0]) == on;
  bool state2 = digitalRead(statePins[1]) == on;
  bool state3 = digitalRead(statePins[2]) == on;

  bool result = state1 && state2 && state3;

  if(result)
    Serial.write("[all cogs] ");
  else
    Serial.write("[not all cogs] ");

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
  do{
    digitalWrite(lockPin, on);
    delay(20);
    digitalWrite(lockPin, off);
    delay(80);
  }while(digitalRead(lockDetectorPin) == off);
  isLocked = true;
  Serial.write("locked ");
}
void unlock(){
  do{
    digitalWrite(unlockPin, on);
    delay(20);
    digitalWrite(unlockPin, off);
    delay(80);
  }while(digitalRead(lockDetectorPin) == on);
  isLocked = false;
  Serial.write("unlocked ");
}
