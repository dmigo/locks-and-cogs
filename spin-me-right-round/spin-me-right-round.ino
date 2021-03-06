#define LEFT 9
#define RIGHT 10
#define BUTTON 3

#define SPIN_DELAY  400
#include <Servo.h>
Servo myservo;
int pos = 0;
class Motor {
  private:
    int _left;
    int _right;
  public:
    Motor(int left, int right){
      _left = left;
      _right = right;

      pinMode(_left, OUTPUT);
      pinMode(_right, OUTPUT);

      tearDown();
    }

   void spinRightRound(){
    digitalWrite(_left, HIGH);
    digitalWrite(_right, LOW);
   }

   void spinReverse(){
      digitalWrite(_left, LOW);
      digitalWrite(_right, HIGH);
   }

   void tearDown(){
    digitalWrite(_left, LOW);
    digitalWrite(_right, LOW);
   }

   void reverse(){
    int tmp = _left;
    _left = _right;
    _right = tmp;
   }
};

class Clicker{
  private:
    int _pin;
    int _clicks;
    long _delayBetweenClicks;
    long _timeout;
    long _lastClick;
    long _debounceDelay;
    bool _lastState;

    long _longMillis(){
      long value = millis();
      return value;
    }
    int _getState(){
      return digitalRead(_pin);
    }
    bool _isDebounced(){
      return _longMillis() > _lastClick + _debounceDelay;
    }
    bool _isTimeout(){
      return _longMillis() > _lastClick + _timeout;
    }
    void _handleStateChanged(bool oldState, bool newState){
      if(oldState == HIGH){
         _lastClick = _longMillis();
         _clicks++;
         Serial.println();
         Serial.print("[");
         Serial.print(_lastClick);
         Serial.print("]");
         Serial.print(" click! total: ");
         Serial.print(_clicks);
         Serial.println();
      }
      _lastState = newState;
    }
    void _handleTimeout(){
      if(_clicks>0)
        Serial.println("dropping the clicks amount");
      _clicks = 0;
    }
    
  public:
    Clicker(int pin, long delayBetweenClicks, long timeout){
      _clicks = 0;
      _lastClick = -2147483648;
      _delayBetweenClicks = delayBetweenClicks;
      _timeout = timeout;
      _debounceDelay = 100;

      _pin = pin;
      pinMode(pin, INPUT_PULLUP);
      _lastState = _getState();
    }

    void check(){
      int newState = _getState();
      if(_lastState != newState && _isDebounced()){
        _handleStateChanged(_lastState, newState);
      }

      if(_isTimeout()){
        _handleTimeout();
      }
    }

    bool isStillWaiting(){
      return _longMillis() < _lastClick + _delayBetweenClicks;
    }

    bool isAboveClicks(int clicks){
      return _clicks >= clicks;
    }
};

Motor* motor;
Clicker* clicker;

void setup() {
  myservo.attach(9);
  pinMode(BUTTON, INPUT_PULLUP);
  motor = new Motor(LEFT, RIGHT);
  clicker = new Clicker(BUTTON, 500, 700);
  
  Serial.begin(9600);
}

void loop() {
  clicker->check();
  if(!clicker->isStillWaiting()){
    if(clicker->isAboveClicks(3)&& pos == 0){
      myservo.write(180);
      delay(SPIN_DELAY);
      pos = 1;
    } else if (clicker->isAboveClicks(1)&& pos == 1){
      myservo.write(0);
      delay(SPIN_DELAY);
      pos = 0; 
    }
  }
}
