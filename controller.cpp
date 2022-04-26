#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "controller.h";

#define LED_PIN 0

WiFiUDP _udp;

Controller::Controller(String ssid, String pwd) {
  _ssid = ssid;
  _pwd = pwd;

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

Device Controller::getDevice() {
  return _device;
}

/**
   Called on setup function
   Responsible for connecting to the WiFi and starting multicast search
*/
void Controller::connect() {
  // Connect to WiFi using ssid and password passed via constructor
  WiFi.begin(_ssid, _pwd);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(400);
  }

  startMulticastSearch();
}

/**
   Called on loop function
   Responsible for listening to incoming packets and calling the parser method,
   as well as updating the LED status based on WiFi status
*/
void Controller::update() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  if (_udp.parsePacket()) {
    char buffer[512];
    _udp.read(buffer, 512);
    parseFeedback(buffer);
  }
}

/**
   Sends initial search packet via UDP to Yeelight specified address (239.255.255.250:1982)
*/
void Controller::startMulticastSearch() {
  IPAddress multicastIP(239, 255, 255, 250);

  _udp.beginMulticast(WiFi.localIP(), multicastIP, 1982);

  _udp.beginPacketMulticast(multicastIP, 1982, WiFi.localIP());
  _udp.print("M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1982\r\nMAN: \"ssdp:discover\"\r\nST: wifi_bulb");
  _udp.endPacket();

  _udp.begin(1982);
}

/**
   Parses the received packet and store the device's information for further use
*/
void Controller::parseFeedback(char *buffer) {
  String id;
  String ip;
  int port;

  String str = String(buffer);

  short startIndex;
  short endIndex;

  startIndex = str.indexOf("Location: yeelight://");

  if (startIndex >= 0) {
    endIndex = str.indexOf("\r\n", startIndex);

    String location = str.substring(startIndex + 21, endIndex);

    short colonIndex = location.indexOf(":");

    if (colonIndex >= 0) {
      ip = location.substring(0, colonIndex);
      port = location.substring(colonIndex + 1).toInt();
    }
  }

  startIndex = str.indexOf("id: ");
  if (startIndex >= 0) {
    endIndex = str.indexOf("\r\n", startIndex);

    id = str.substring(startIndex + 4, endIndex);
  }

  // Populates device
  _device = Device(id, ip, port);
}
