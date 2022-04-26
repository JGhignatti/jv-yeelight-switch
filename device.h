#ifndef Device_h
#define Device_h

#include <Arduino.h>

class Device {
  public:
    Device();
    Device(String id, String ip, int port);
    void toggle();
    void reset();
  private:
    String _id;
    String _ip;
    int _port;
    void sendCommand(String method, String params);
};

#endif
