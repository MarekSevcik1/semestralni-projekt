#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

// deklarace externich promennych z .cpp
extern const char* lcdPhase;
extern int lcdSeconds;
extern int lcdRound;
extern int lcdRoundsTotal;

void lcdInit();
void lcdShowIdle();
void lcdShowWorkout(const char* name, int round, int totalRounds);
void lcdShowPhase(const char* phase, int seconds);
void lcdShowStopped();
void lcdShowDone();
void lcdShowRunning();
void lcdUpdateWorkout();
void lcdShowCustomScreen(const char* l1, const char* l2, const char* l3, const char* l4);

#endif
