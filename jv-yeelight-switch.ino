#include "button-handler.h";
#include "controller.h";

ButtonHandler handler;
Controller controller("ssid", "pwd");

void setup() {
  controller.connect();
}

void loop() {
  controller.update();
  handler.act(controller.getDevice());
}
