const int ledPin = 2;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void blink () {
  digitalWrite(ledPin, HIGH);
  delay(1000);                     
  digitalWrite(ledPin, LOW);  
  delay(2000); 
}

void loop() {
  blink();
}
