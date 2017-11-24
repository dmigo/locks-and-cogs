#ifndef RFID_LOCK
#define RFID_LOCK
#include <rdm630.h>

#define TIME_TO_LOSE 500

class RfidLock{
private:
  rdm630 *_rfid = new rdm630(6, 0);  //TX-pin of RDM630 connected to Arduino pin 6
  unsigned long _key;
  unsigned long _state = 0;
  unsigned long _lastAvailable = 0;
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

  void _updateState(unsigned long state) {
    _state = state;
  }

  bool _stateChanged(unsigned long state) {
    return state != _state;
  }

  bool _isLost(){
    return millis() > _lastAvailable + TIME_TO_LOSE;
  }

  bool _isOpen(){
    return _state == _key;
  }

  void _close(){
    if(_isOpen()){
      _onClose();
    }
  }

  void _open(){
    if(!_isOpen())
      _onOpen();
  }
  
public:
  RfidLock(unsigned long key){
    _rfid->begin();
    _key = key;
  }

  void check(){
    if(!_isRfidAvailable()){
      if(_isLost()){
        _updateState(0);
        _close();
      }
        
      return;
    }
      
    unsigned long newState = _getRfid();
    _lastAvailable = millis();
    
    if(_stateChanged(newState)){
      _updateState(newState);
      
      if(_isOpen())
        _open();
      else
        _close();
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
