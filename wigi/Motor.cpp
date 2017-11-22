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

public:
	Motor(int clockPin, int counterclockPin){
		_clock = new SimpleRelay(clockPin);
		_counterclock = new SimpleRelay(counterclockPin);
		_home = new BlinkingRelay(_counterclock, 500, 100);
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
			else if(_goHome)
				_home->switchOn();
		}
	}
};
