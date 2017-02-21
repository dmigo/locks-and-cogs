
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#define NOTE_SUSTAIN 250

/*************************************************
 * Notes
 *************************************************/

#include <LiquidCrystal.h>
#include <Keypad.h>

const byte numRows= 4; 
const byte numCols= 4; 

char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

byte rowPins[numRows] = {6,7,8,9};
byte colPins[numCols]= {A1,A2,A3}; 

const int totalTime = 98300; // in decisecs
int time = totalTime;
bool tick = false;

//3600, 3300, 3000, 2700, 2400, 2100, 1800, 1500, 1200, 900, 600, 300, 30, 20, 10
int beeps [] = { 98300, 98200, 98100, 98000, 97900, 97800, 97700, 97600, 97500, 97400, 97300, 97200, 97100, 97000, 96900, 96800, 96700, 96600, 96500, 96400, 96300, 96200, 96100, 96000, 95900, 95800, 95700, 95600, 95500, 95400, 95300, 95200, 95100, 95000, 94900, 94800, 94700, 94600, 94500, 94400, 94300, 94200, 94100, 94000, 93900, 93800, 93700, 93600, 93500, 93400, 93300, 93200, 93100, 93000, 92900, 92800, 92700, 92600, 92500, 92400, 92300, 92200, 92100, 92000, 91900, 91800, 91700, 91600, 91500, 91400, 91300, 91200, 91100, 91000, 90900, 90800, 90700, 90600, 90500, 90400, 90300, 90200, 90100, 90000, 89900, 89800, 89700, 89600, 89500, 89400, 89300, 89200, 89100, 89000, 88900, 88800, 88700, 88600, 88500, 88400, 88300, 88200, 88100, 88000, 87900, 87800, 87700, 87600, 87500, 87400, 87300, 87200, 87100, 87000, 86900, 86800, 86700, 86600, 86500, 86400, 86300, 86200, 86100, 86000, 85900, 85800, 85700, 85600, 85500, 85400, 85300, 85200, 85100, 85000, 84900, 84800, 84700, 84600, 84500, 84400, 84300, 84200, 84100, 84000, 83900, 83800, 83700, 83600, 83500, 83400, 83300, 83200, 83100, 83000, 82900, 82800, 82700, 82600, 82500, 82400, 82300, 82200, 82100, 82000, 81900, 81800, 81700, 81600, 81500, 81400, 81300, 81200, 81100, 81000, 80900, 80800, 80700, 80600, 80500, 80400, 80300, 80200, 80100, 80000, 79900, 79800, 79700, 79600, 79500, 79400, 79300, 79200, 79100, 79000, 78900, 78800, 78700, 78600, 78500, 78400, 78300, 78200, 78100, 78000, 77900, 77800, 77700, 77600, 77500, 77400, 77300, 77200, 77100, 77000, 76900, 76800, 76700, 76600, 76500, 76400, 76300, 76200, 76100, 76000, 75900, 75800, 75700, 75600, 75500, 75400, 75300, 75200, 75100, 75000, 74900, 74800, 74700, 74600, 74500, 74400, 74300, 74200, 74100, 74000, 73900, 73800, 73700, 73600, 73500, 73400, 73300, 73200, 73100, 73000, 72900, 72800, 72700, 72600, 72500, 72400, 72300, 72200, 72100, 72000, 71900, 71800, 71700, 71600, 71500, 71400, 71300, 71200, 71100, 71000, 70900, 70800, 70700, 70600, 70500, 70400, 70300, 70200, 70100, 70000, 69900, 69800, 69700, 69600, 69500, 69400, 69300, 69200, 69100, 69000, 68900, 68800, 68700, 68600, 68500, 68400, 68300, 68200, 68100, 68000, 67900, 67800, 67700, 67600, 67500, 67400, 67300, 67200, 67100, 67000, 66900, 66800, 66700, 66600, 66500, 66400, 66300, 66200, 66100, 66000, 65900, 65800, 65700, 65600, 65500, 65400, 65300, 65200, 65100, 65000, 64900, 64800, 64700, 64600, 64500, 64400, 64300, 64200, 64100, 64000, 63900, 63800, 63700, 63600, 63500, 63400, 63300, 63200, 63100, 63000, 62900, 62800, 62700, 62600, 62500, 62400, 62300, 62200, 62100, 62000, 61900, 61800, 61700, 61600, 61500, 61400, 61300, 61200, 61100, 61000, 60900, 60800, 60700, 60600, 60500, 60400, 60300, 60200, 60100, 60000, 59900, 59800, 59700, 59600, 59500, 59400, 59300, 59200, 59100, 59000, 58900, 58800, 58700, 58600, 58500, 58400, 58300, 58200, 58100, 58000, 57900, 57800, 57700, 57600, 57500, 57400, 57300, 57200, 57100, 57000, 56900, 56800, 56700, 56600, 56500, 56400, 56300, 56200, 56100, 56000, 55900, 55800, 55700, 55600, 55500, 55400, 55300, 55200, 55100, 55000, 54900, 54800, 54700, 54600, 54500, 54400, 54300, 54200, 54100, 54000, 53900, 53800, 53700, 53600, 53500, 53400, 53300, 53200, 53100, 53000, 52900, 52800, 52700, 52600, 52500, 52400, 52300, 52200, 52100, 52000, 51900, 51800, 51700, 51600, 51500, 51400, 51300, 51200, 51100, 51000, 50900, 50800, 50700, 50600, 50500, 50400, 50300, 50200, 50100, 50000, 49900, 49800, 49700, 49600, 49500, 49400, 49300, 49200, 49100, 49000, 48900, 48800, 48700, 48600, 48500, 48400, 48300, 48200, 48100, 48000, 47900, 47800, 47700, 47600, 47500, 47400, 47300, 47200, 47100, 47000, 46900, 46800, 46700, 46600, 46500, 46400, 46300, 46200, 46100, 46000, 45900, 45800, 45700, 45600, 45500, 45400, 45300, 45200, 45100, 45000, 44900, 44800, 44700, 44600, 44500, 44400, 44300, 44200, 44100, 44000, 43900, 43800, 43700, 43600, 43500, 43400, 43300, 43200, 43100, 43000, 42900, 42800, 42700, 42600, 42500, 42400, 42300, 42200, 42100, 42000, 41900, 41800, 41700, 41600, 41500, 41400, 41300, 41200, 41100, 41000, 40900, 40800, 40700, 40600, 40500, 40400, 40300, 40200, 40100, 40000, 39900, 39800, 39700, 39600, 39500, 39400, 39300, 39200, 39100, 39000, 38900, 38800, 38700, 38600, 38500, 38400, 38300, 38200, 38100, 38000, 37900, 37800, 37700, 37600, 37500, 37400, 37300, 37200, 37100, 37000, 36900, 36800, 36700, 36600, 36500, 36400, 36300, 36200, 36100, 36000, 35900, 35800, 35700, 35600, 35500, 35400, 35300, 35200, 35100, 35000, 34900, 34800, 34700, 34600, 34500, 34400, 34300, 34200, 34100, 34000, 33900, 33800, 33700, 33600, 33500, 33400, 33300, 33200, 33100, 33000, 32900, 32800, 32700, 32600, 32500, 32400, 32300, 32200, 32100, 32000, 31900, 31800, 31700, 31600, 31500, 31400, 31300, 31200, 31100, 31000, 30900, 30800, 30700, 30600, 30500, 30400, 30300, 30200, 30100, 30000, 29900, 29800, 29700, 29600, 29500, 29400, 29300, 29200, 29100, 29000, 28900, 28800, 28700, 28600, 28500, 3600, 3300, 3000, 2700, 2400, 2100, 1800, 1500, 1200, 900, 600, 300, 30, 20, 10 };
int ibeep = 0;
int unbeep = 0;
bool beeping = false;

const int buzzerPin = 10;

bool nextKey = false;
bool disarmed = false;
bool exploded = false;
int attempts = 6;
const int combilen = 6;
char combi [combilen] = {'5','1','3','9','2','4'};
int combindex = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup() {
  Serial.begin(9600);
  
  pinMode(buzzerPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Explosion in:");
}

void loop() {
  if(exploded || disarmed)
    return;
  
  if(time<=0){
    loose();
    return;
  }
  
  tickTock();
  checkEvents();
  listenKey(); 
}

void tickTock(){
  int out = millis() / 100;
  int newTime = totalTime - out;
  tick = newTime != time;
  time = newTime;
}

void checkEvents(){
  if(!tick)
    return;
    
  if(time <= beeps[ibeep])
    {
      ibeep++;
      unbeep = time - 1;
      beep();
    }
  if(time <= unbeep && beeping)
    {
      dontbeep();
    }
  
  printTime();
}

void listenKey(){
  char keypressed = myKeypad.getKey();
  
  if (keypressed == NO_KEY)
  {
    nextKey = true;
    return;
  }
  
  if(!nextKey)
    return;
  
  nextKey = false;
  
  if(keypressed == combi[combindex])
    combindex++;
  else
  {
    combindex = 0;
    attemptFailed();
  }
  
  Serial.print('[');
  Serial.print(keypressed);
  Serial.print(':');
  Serial.print(combindex);
  tone(10, 800, 100);
  
  if(combindex == combilen)
    win();
  
  Serial.print(']');
}

void attemptFailed(){
  attempts-=1;
  Serial.print("<Attempts: ");
  Serial.print(attempts);
  Serial.print(">");
  Serial.println("");
  if(attempts ==0){
    tone(buzzerPin,NOTE_G4);
    delay(250);
    tone(buzzerPin,NOTE_C4);
    delay(500);
    noTone(buzzerPin);
    attempts = 6;
  }
  else{
    unbeep = time - 2;
    beep();
  }
}

void beep(){
  beeping = true;
  Serial.println("beep!");
  Serial.println(ibeep);
  tone(buzzerPin, 1000);
}
void dontbeep(){
  beeping = false;
  Serial.println("unbeep!");
    noTone(buzzerPin); 
}

void printTime(){
  int mins = time / 600;
  int secs = time % 600 / 10;
  lcd.setCursor(6, 1);
  if(mins/10==0)  
    lcd.print("0"); 
  lcd.print(mins);
  lcd.print(":");
  if(secs/10==0)
    lcd.print("0"); 
  lcd.print(secs);
}

void win(){
  
  disarmed = true;
  
  tone(buzzerPin,NOTE_A5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_B5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_C5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_B5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_C5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_D5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_C5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_D5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_E5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_D5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_E5);
  delay(NOTE_SUSTAIN);
  tone(buzzerPin,NOTE_E5);
  delay(NOTE_SUSTAIN);
  noTone(buzzerPin);
}
void loose(){
  exploded = true;

  tone(buzzerPin,NOTE_G4);
  delay(250);
  tone(buzzerPin,NOTE_C4);
  delay(500);
  noTone(buzzerPin);
}
