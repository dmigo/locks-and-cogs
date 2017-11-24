#include <Arduino.h>
#include "Relays.cpp"

#define DELAY_ON 100 // задержка на включение
#define DELAY_OFF 500 // задержка на выключение

class Motor{
private:
	SimpleRelay *_clock;
	SimpleRelay *_counterclock;

	bool _moveClock = false;
	bool _moveCounterclock = false;

	long _startDelay = 0;
	int _delay = 0;

public:
	Motor(int clockPin, int counterclockPin){
		_clock = new SimpleRelay(clockPin);
		_counterclock = new SimpleRelay(counterclockPin);
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

	void stop(){
		_moveClock = false;
		_moveCounterclock = false;

		_clock->switchOff();
		_counterclock->switchOff();
	}

	void check(){
		if(millis() <= _startDelay + _delay)
		  return;
     
		if(_moveClock)
			_clock->switchOn();
		else if(_moveCounterclock)
			_counterclock->switchOn();
	}
};
