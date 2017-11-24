#include <Arduino.h>

class SimpleRelay{
  private:
  int _pin;
  int _state = -1;

  void _setState(int newState){
    if(_state != newState){
      _state = newState;
      digitalWrite(_pin, _state);
    }
  }
  
  public:
  SimpleRelay(int pin){
    _pin = pin;
    pinMode(_pin, OUTPUT);
    switchOff();
  }

  void switchOn(){
    _setState(LOW);
  }
  void switchOff(){
    _setState(HIGH);
  }
};


class BlinkingRelay{
  private:
  SimpleRelay *_relay;
  int _delayOn;
  int _delayOff;
  bool _isRunning;

  long _startBlinking;
  
  bool _isOn(int offset){
    while(offset>=0)
      if(offset<_delayOn)
        return true;
      else
        offset-=_delayOn+_delayOff;
    return false;
  }
  
  bool _isOff(int offset){
    while(offset>=_delayOn)
      if(offset<_delayOn + _delayOff)
        return true;
      else
        offset-=_delayOn+_delayOff;
    return false;
  }
  
  public:
  BlinkingRelay(SimpleRelay* relay, int delayOn, int delayOff){
    _relay = relay;
    
    _delayOn = delayOn;
    _delayOff = delayOff;

    _isRunning = false;
    _startBlinking = 0;
  }
  
  void check(){
    if(!_isRunning)
      return;
    
    long current = millis();
    long offset = current - _startBlinking;
    
    Serial.println(offset);
    bool isOn = _isOn(offset);
    bool isOff = _isOff(offset);

    if(isOn){
      _relay->switchOn();
      Serial.println("blink on");
    }
    else if(isOff){
      _relay->switchOff();
      Serial.println("blink off");
    }
  }
  
  void switchOn(){
    if(_isRunning)
      return;
      
    _isRunning = true;
    _startBlinking = millis();
  }
  
  void switchOff(){
    _isRunning = false;
    _startBlinking = 0;
  }
};


