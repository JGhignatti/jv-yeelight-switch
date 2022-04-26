#include <ESP8266WiFi.h>
#include "device.h";

WiFiClient client;

unsigned int cmdId = 0;

Device::Device() {
}

Device::Device(String id, String ip, int port) {
  _id = id;
  _ip = ip;
  _port = port;
}

void Device::toggle() {
  sendCommand("toggle", "[]");
}

void Device::reset() {
  sendCommand("set_ct_abx", "[4021, \"smooth\", 500]");
}

void Device::sendCommand(String method, String params) {
  if (_ip && _port && client.connect(_ip, _port)) {
    String payload = String("") + "{\"id\":" + (++cmdId) + ",\"method\":\"" + method + "\",\"params\":" + params + "}";
    client.println(payload);
  }
}
