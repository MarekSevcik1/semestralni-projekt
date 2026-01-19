#ifndef BUZZER_H
#define BUZZER_H

void buzzerInit(int pin);
void beep(int durationMs);
void beepStart();
void beepEnd();
void beepTransition();

#endif
