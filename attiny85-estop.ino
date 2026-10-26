// simple programmable mach3 cnc e-stop to USB (keyboard) adapter using a normally-closed emergency-stop red button and attiny with digispark usb bootloader.

#include "DigiKeyboard.h"

const uint8_t BUTTON_PIN = 0;  // P0 on silkscreen
const uint8_t LED_PIN = 1;     // P1 on silkscreen

const unsigned long DEBOUNCE_MS = 50;

int lastReading;
int stableState;
unsigned long lastChangeTime = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  lastReading = digitalRead(BUTTON_PIN);
  stableState = lastReading;

  // LED on when NC contact is open (estopped)
  digitalWrite(LED_PIN, stableState == HIGH ? HIGH : LOW);

  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.delay(1000);  // startup wait
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastReading) {
    lastReading = reading;
    lastChangeTime = millis();
  }

  if (millis() - lastChangeTime >= DEBOUNCE_MS && reading != stableState) {

    stableState = reading;

    // Send key on both rising and falling edge
    DigiKeyboard.sendKeyStroke(KEY_ESC);

    // LED on while E-stop is depressed
    digitalWrite(LED_PIN, stableState == HIGH ? HIGH : LOW);
  }

  DigiKeyboard.update();
  //  delay(5);
}