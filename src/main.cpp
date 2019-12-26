#include <Arduino.h>

const uint8_t LED_PIN = 13;
const uint8_t BUTTON_PIN = 2;

void setup() {
  // Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}

bool hasNewPinLevel(const uint8_t pin, bool level) {
  static const uint16_t DEBOUNCE_TIME = 20;
  static uint32_t lastTimestamp = 0;
  const bool pinState = digitalRead(pin);
  static bool lastPinState = pinState;
  bool newPinLevelMatch = false;

  if (pinState != lastPinState) {
    lastPinState = pinState;
    lastTimestamp = millis();
  } else if (lastTimestamp && (millis() - lastTimestamp) >= DEBOUNCE_TIME) {
    newPinLevelMatch = pinState == level;
    lastTimestamp = 0;
  }

  return newPinLevelMatch;
}

void loop() {
  static const uint16_t LED_TIMEOUT = 5000;
  static uint32_t timer = 0;

  if (hasNewPinLevel(BUTTON_PIN, HIGH)) {
      const bool ledState = digitalRead(LED_PIN);

      // set timer if we switch the LED from LOW to HIGH
      if (!ledState) {
        timer = millis() + LED_TIMEOUT;
      } else {
        timer = 0;
      }

      digitalWrite(LED_PIN, !ledState);
  }

  if (timer && ((int32_t)millis() > (int32_t)timer)) {
    timer = 0;

    digitalWrite(LED_PIN, LOW);
  }
}