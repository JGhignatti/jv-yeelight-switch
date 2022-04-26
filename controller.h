#ifndef Controller_h
#define Controller_h

#include <Arduino.h>
#include "device.h";

class Controller {
  public:
    Controller(String ssid, String pwd);
    Device getDevice();
    void connect();
    void update();
  private:
    String _ssid;
    String _pwd;
    Device _device;
    void startMulticastSearch();
    void parseFeedback(char *buffer);
};

#endif
