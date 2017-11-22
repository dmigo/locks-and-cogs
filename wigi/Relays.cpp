#include <Arduino.h>

class SimpleRelay{
  private:
  int _pin;
  bool _state;

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

    _startBlinking = 0;
  }
  
  void check(){
    if(_startBlinking == 0)
      return;
    
    long current = millis();
    long offset = current - _startBlinking;
    
    bool isOn = _isOn(offset);
    bool isOff = _isOff(offset);

    if(isOn)
      _relay->switchOn();
    else if(isOff)
      _relay->switchOff();
  }
  
  void switchOn(){
    _startBlinking = millis();
  }
  
  void switchOff(){
    _startBlinking = 0;
  }
};


