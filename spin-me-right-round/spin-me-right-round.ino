#define LEFT 9
#define RIGHT 10
#define BUTTON 5

#define SPIN_DELAY  1000

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

    long _longMillis(){
      long value = millis();
      return value;
    }
    bool _isPressed(){
      return digitalRead(_pin) == LOW;
    }
    bool _isDebounced(){
      return _longMillis() > _lastClick + _debounceDelay;
    }
    bool _isTimedOut(){
      return _longMillis() > _lastClick + _timeout;
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
    }

    void check(){
      if(_isPressed() && _isDebounced()){
         Serial.println("pressed and debounced");
         _lastClick = _longMillis();
         _clicks++;
      } else if(_isTimedOut()){
        _clicks = 0;
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
  pinMode(BUTTON, INPUT_PULLUP);
  motor = new Motor(LEFT, RIGHT);
  clicker = new Clicker(BUTTON, 500, 700);
  Serial.begin(9600);
}

void loop() {
  clicker->check();
  if(!clicker->isStillWaiting()){
    if(clicker->isAboveClicks(3)){
      motor->spinReverse();
      delay(SPIN_DELAY);
      motor->tearDown();
    } else if (clicker->isAboveClicks(1)){
      motor->spinRightRound();
      delay(SPIN_DELAY);
      motor->tearDown(); 
    }
  }
}
