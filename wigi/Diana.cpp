#include <Arduino.h>
#include "Motor.cpp"

#define DELAY 2100 //задержка между буквами

const int letters_l = 5;
const int positions[letters_l] = {70, 40, 160, 250, 30}; // позиция для каждой из букв {d, i, a, n, a}
const int clockwise[letters_l] = {true, true, false, true, false};

class Diana{
private:
	Motor * _motor;
	int _index;
	bool _isSpeaking;
  int (*_getPosition)();
  bool (*_isHome)();
	long _letterTime;

	bool _reached(int position){
		if(clockwise[_index] && position>=positions[_index])
			return true;
		if(!clockwise[_index] && position<=positions[_index])
			return true;

		return false;
	}

	void _moveTo(int index){
		if(clockwise[index])
			_motor->spinClock(DELAY);
		else
			_motor->spinCounterclock(DELAY);
	}

  void _goHome(){
    _motor->goHome(DELAY);
  }

public:
	Diana(Motor *motor, int (*getPosition)(), bool (*isHome)()){
		_getPosition = getPosition;
    _isHome = isHome;
		_motor = motor;
		_index = 0;
		_isSpeaking = true;
		_letterTime = 0;
	}

	void speak(){
		_index = 0;
		_moveTo(_index);
		_isSpeaking = true;
	}
	void signOff(){
    if(_isSpeaking){
      _isSpeaking = false;
      _goHome();
    }
	}

	void check(){
		if(!_isSpeaking)
			return;

    if(_index>=letters_l){
      if(_isHome()){
        Serial.println("Honey Im home!");
        speak();
      }
    }
    else{
      int position = _getPosition();
      if(_reached(position)){
        _index++;
        if(_index<letters_l)
          _moveTo(_index);
        else{
          Serial.println("Diana finally goes home");
          _goHome();
        }
      }
    }
	}
};
