#include <Arduino.h>
#include "Relays.cpp"

class Motor{
private:
	SimpleRelay *_clock;
	SimpleRelay *_counterclock;
	BlinkingRelay *_home;

	bool _moveClock = false;
	bool _moveCounterclock = false;
	bool _goHome = false;

	long _startDelay = 0;
	int _delay =0;
  int _homeSensorPin;

public:
	Motor(int clockPin, int counterclockPin, int homeSensorPin){
		_clock = new SimpleRelay(clockPin);
    _homeSensorPin = homeSensorPin;
		_counterclock = new SimpleRelay(counterclockPin);
   
		_home = new BlinkingRelay(_counterclock, 100, 500);
    pinMode(_homeSensorPin, INPUT_PULLUP);
	}

	void spinClock(int delay){
		stop();
		_startDelay = millis();
		_delay = delay;

		_moveClock = true;
	}

	void spinCounterclock(int delay){
		stop();
		_startDelay = millis();
		_delay = delay;

		_moveCounterclock = true;
	}

	void goHome(int delay){
		stop();
		_startDelay = millis();
		_delay = delay;

		_goHome = true;
	}

	void stop(){
		_moveClock = false;
		_moveCounterclock = false;
		_goHome = false;

		_clock->switchOff();
		_counterclock->switchOff();
		_home->switchOff();
	}

	void check(){
		_home->check();

		if(millis() > _startDelay + _delay){
			if(_moveClock)
				_clock->switchOn();
			else if(_moveCounterclock)
				_counterclock->switchOn();
			else if(_goHome){
        if(digitalRead(_homeSensorPin) != LOW){
          _home->switchOn();
          Serial.println("Going home!");
        }
        else{
          stop();
          Serial.println("Honey I'm home!");
        }
			}
		}
	}
};
