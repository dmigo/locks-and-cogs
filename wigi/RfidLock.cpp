#ifndef RFID_LOCK
#define RFID_LOCK
#include <rdm630.h>

class RfidLock{
private:
  rdm630 *_rfid = new rdm630(6, 0);  //TX-pin of RDM630 connected to Arduino pin 6
  unsigned long _key;
  unsigned long _uid;
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
  
public:
  RfidLock(unsigned long key){
    _rfid->begin();
    _key = key;
  }

  void check(){
    int newUid = _read();

    if(newUid != _uid){
      if(newUid == _key)
        _onOpen();
      else
        _onClose();
      _uid = newUid;
    }
  }

  void onOpen(void (*callback)()) {
    _onOpen = callback;
  }
  void onClose(void (*callback)()) {
    _onClose = callback;
  }
};

#endif
