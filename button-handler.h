#ifndef ButtonHandler_h
#define Buttonhandler_h

#include "device.h";

class ButtonHandler {
  public:
    ButtonHandler();
    void act(Device device);
  private:
    short _pin;
};

#endif
