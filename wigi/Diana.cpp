#include <Arduino.h>
#include "Motor.cpp"

#define DELAY 2100 //задержка между буквами

const int letters_l = 6;
const int positions[letters_l] = {70, 160, 40, 250, 40, 0}; // позиция для каждой из букв {d, i, a, n, a}
const int clockwise[letters_l] = {true, true, false, true, false, false};

class Diana{
private:
	Motor * _motor;
	int _index;
	bool _isSpeaking;
  int (*_getPosition)();
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
    _index = 5;
    _moveTo(_index);
  }

public:
	Diana(Motor *motor, int (*getPosition)()){
		_getPosition = getPosition;
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
    int position = _getPosition();
    if(_reached(position)){
      _index++;
      if(_index<letters_l)
        _moveTo(_index);
      else{
        if(_isSpeaking)
          speak();
      }
    }
	}
};
