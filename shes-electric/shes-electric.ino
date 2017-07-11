#define out 5
#define in A5
#define lock 9
#define tolerance 900
#define opendelay 6000

void setup() {
  pinMode(out, OUTPUT);
  digitalWrite(out, HIGH);
  
  pinMode(lock, OUTPUT);
  closeLock();
  
  Serial.begin(9600);
}

void openLock(){
  Serial.println("open");
  digitalWrite(lock, HIGH);
}
void closeLock(){
  Serial.println("close");
  digitalWrite(lock, LOW);
}

bool isConnected(){
  int value = analogRead(in);
  Serial.println(value);
  return value > tolerance;
}

void loop() {
  if(isConnected()){
    openLock();
    delay(opendelay);
    closeLock();
  }
  delay(500);
}
