const int lockPin = LED_BUILTIN;
const int bellPins[] = {3,4,5,6};
const int code[] = {3,1,1,0,2,1};

int index = 0;
bool down = false;

void setup() {
  pinMode(lockPin, OUTPUT);

for(int i =0; i<4;i++)
  pinMode(bellPins[i], INPUT_PULLUP);
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  
}

void loop() {
  if(index==6){
    blink();
    index = 0;
  }
    
  int bell = getBell();
  if(bell!=-1)
  {
      int newIndex = 0;
      if(!down && bell == code[index]){
        newIndex = index+1;
        printR(newIndex);
      }
      else if (down && index>0 && bell == code[index-1])
        newIndex = index;
      else
        printR(newIndex);
      
      index= newIndex;
      down = true;
    }
    else{
      down = false;
    }
    
    delay(100);    
}

void printR(int r){
      Serial.print(" [index ");
      Serial.print(r);
      Serial.print("] ");
}

int getBell(){
  for(int i=0;i<4;i++){
    if(digitalRead(bellPins[i])==LOW)
    {
      return i;
     }
   }
  return -1;
}

void blink(){
  Serial.print(" blink-blink ");
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000); 
}
