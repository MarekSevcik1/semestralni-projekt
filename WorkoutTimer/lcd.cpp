#include "lcd.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>    //knihovna na displej

// Inicializace displeje - adresa 0x27, 20 znaku, 4 radky
LiquidCrystal_I2C lcd(0x27, 20, 4);

// globalni proimenne
const char* lcdPhase = "";
int lcdSeconds = 0;
int lcdRound = 0;
int lcdRoundsTotal = 0;

void lcdInit() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("   WORKOUT TIMER    ");
  lcd.setCursor(0, 1);
  lcd.print("      READY...      ");
}

void lcdShowIdle() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   WORKOUT TIMER    ");
  lcd.setCursor(0, 1);
  lcd.print("STATE: IDLE");
  lcd.setCursor(0, 2);
  lcd.print("On your comand...");
}

void lcdShowRunning() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   WORKOUT TIMER    ");
  lcd.setCursor(0, 1);
  lcd.print("STATE: RUNNING");
}

void lcdShowWorkout(const char* name, int round, int totalRounds) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(name); 

  lcd.setCursor(0, 1);
  lcd.print("KOLO: ");
  lcd.print(round);
  lcd.print(" / ");
  lcd.print(totalRounds);
  
  // aby neblikaly
  lcd.setCursor(0, 2);
  lcd.print("FAZE WORKOUTU: ");
  lcd.setCursor(0, 3);
  lcd.print("ZBYVAJICI CAS: ");
}

void lcdShowPhase(const char* phase, int seconds) {
  lcd.setCursor(7, 2);
  lcd.print("            "); 
  lcd.setCursor(7, 2);
  lcd.print(phase);

  // vymaz stary a napis novy cas
  lcd.setCursor(16, 3);
  lcd.print("    "); // mazani mista po vteriny
  lcd.setCursor(16, 3);
  lcd.print(seconds);
  lcd.print("s");
}

void lcdShowStopped() {
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print("!!! STOP !!!");
}

void lcdShowDone() {
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("WORKOUT U KONCE!");
}

// aktualizace vseho najednou
void lcdUpdateWorkout() {
  lcdShowPhase(lcdPhase, lcdSeconds);
}

//loading screen 
void lcdShowCustomScreen(const char* l1, const char* l2, const char* l3, const char* l4) {
  lcd.clear();
  
  lcd.setCursor(0, 0); lcd.print(l1);
  lcd.setCursor(0, 1); lcd.print(l2);
  lcd.setCursor(0, 2); lcd.print(l3);
  lcd.setCursor(0, 3); lcd.print(l4);
}
