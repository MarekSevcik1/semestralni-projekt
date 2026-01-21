#include <Arduino.h>
#include "buzzer.h"

static int buzzerPin;

void buzzerInit(int pin) {
  buzzerPin = pin;
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

void beep(int durationMs) {
  digitalWrite(buzzerPin, HIGH);
  delay(durationMs);
  digitalWrite(buzzerPin, LOW);
}

//davkrat pipnuti na zacatku
void beepStart() {
  beep(300);
  delay(100);
  beep(300);
}

void beepEnd() {
  beep(600);
}

void beepTransition() {
  beep(150);
}
