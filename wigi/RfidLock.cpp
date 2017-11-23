#ifndef RFID_LOCK
#define RFID_LOCK
#include <rdm630.h>

#define DEBOUNCE 100

class RfidLock{
private:
  rdm630 *_rfid = new rdm630(6, 0);  //TX-pin of RDM630 connected to Arduino pin 6
  unsigned long _key;
  unsigned long _state = 0;
  unsigned long _stateTimestamp = 0;
  bool _isOpen = false;
  void (*_onOpen)();
  void (*_onClose)();
  
  bool _isRfidAvailable(){
    return _rfid->available();
  }
  unsigned long _getRfid(){
    byte data[6];
    byte length;
    
    _rfid->getData(data, length);
    unsigned long result = 
      ((unsigned long int)data[1]<<24) + 
      ((unsigned long int)data[2]<<16) + 
      ((unsigned long int)data[3]<<8) + 
      data[4];

    return result;
  }
  
  unsigned long _read(){
    if(_isRfidAvailable()){
      return _getRfid();
    }
    return 0;
  }

  void _updateState() {
    _state = _read();
    _stateTimestamp = millis();
  }

  bool _stateChanged() {
    return _read() != _state;
  }
  bool _debounced() {
    return millis() > _stateTimestamp + DEBOUNCE;
  }
  
  
public:
  RfidLock(unsigned long key){
    _rfid->begin();
    _key = key;
  }

  void check(){
    if(_stateChanged() 
    && _debounced()){
      _updateState();
      if(_state == _key)
        _onOpen();
      else
        _onClose();
    }
  }

  void onOpen(void (*callback)()) {
    _onOpen = callback;
  }
  void onClose(void (*callback)()) {
    _onClose = callback;
  }
  unsigned long getUid(){
    return _state;
  }
};

#endif
