#include "button-handler.h";

#define BUTTON_PIN 2

int lastButtonState;
long pressMillis = -1;
long releaseMillis = -1;
boolean holdHappened;

ButtonHandler::ButtonHandler() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  lastButtonState = digitalRead(BUTTON_PIN);
}

void ButtonHandler::act(Device device) {
  int buttonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && buttonState == LOW && millis() - releaseMillis > 20) {
    pressMillis = millis();
    holdHappened = false;
  } else if (lastButtonState == LOW && buttonState == HIGH && millis() - pressMillis > 20 && millis() - pressMillis <= 1000) {
    releaseMillis = millis();

    device.toggle();
  }

  if (buttonState == LOW && millis() - pressMillis > 1000) {
    if (!holdHappened) {
      holdHappened = true;

      device.reset();
    }
  }

  lastButtonState = buttonState;
}
