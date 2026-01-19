#include <Arduino.h>
#include "buzzer.h"
#include "workouts.h"

const int BUZZER_PIN = 10;

//========= SYSTEM STATE==========
enum SystemState {
  IDLE,
  RUNNING,
  STOPPING
};

SystemState systemState = IDLE;

//=====CONTROL FLAGS==========
bool stopRequested = false;

//========= NEKONECNA LOOP==========
void loop() {
  if (Serial.available()) {
  processCommand(Serial.readStringUntil('\n'));
  }
}

//======= COMMAND PROCESSOR==========
void processCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  // ===== LIST =====
  if (cmd == "LIST") {
    Serial.println("Dostupne workouty:");
    for (int i = 0; i < WORKOUT_COUNT; i++) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(workouts[i].name);
    }
    return;
  }

  // ===== STATUS =====
  if (cmd == "STATUS") {
    Serial.print("STATE:");
    Serial.println(systemState == IDLE ? "IDLE" : "RUNNING");
    return;
  }

  // ===== STOP =====
  if (cmd == "STOP") {
    if (systemState == RUNNING) {
      stopRequested = true;
      Serial.println("STOP prijat.");
    } else {
      Serial.println("Nic nebezi.");
    }
    return;
  }

  // ===== START X =====
  if (cmd.startsWith("START")) {
    if (systemState == RUNNING) {
      Serial.println("Workout uz bezi.");
      return;
    }

    int index = cmd.substring(5).toInt() - 1;

    if (index < 0 || index >= WORKOUT_COUNT) {
      Serial.println("Neplatny index workoutu.");
      return;
    }

    systemState = RUNNING;
    stopRequested = false;

    runWorkout(workouts[index]);

    systemState = IDLE;
    return;
  }

  //======== HELP =========
  //if (cmd == "HELP") {
    //nepotrebne pro gui
  //}

  // ===== NEZNAMY PRIKAZ =====
  Serial.println("Neznamy prikaz.");
}


void countdown(const char* label, int seconds) {
  Serial.println(label);

  for (int i = seconds; i > 0; i--) {
    Serial.println(i);

    unsigned long start = millis();
    while (millis() - start < 1000) {
      if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.equalsIgnoreCase("x")) {
          stopRequested = true;
          return;
        }
      }
    }

    if (stopRequested) return;
  }
}


void runWorkout(const Workout& w) {
  //if (stopRequested) return;
  stopRequested = false;


  Serial.println();
  Serial.print("Start: ");
  Serial.println(w.name);
  Serial.println("Pro ukonceni napis X + Enter");
  beepStart();

  for (int round = 1; round <= w.rounds; round++) {
    if (stopRequested) break;

    Serial.print("Kolo ");
    Serial.print(round);
    Serial.print(" / ");
    Serial.println(w.rounds);

    countdown("CVIČENÍ", w.workSeconds);
    if (stopRequested) break;
    beepTransition();

    if (round < w.rounds) {
      countdown("PAUZA", w.restSeconds);
      if (stopRequested) break;
      beepTransition();
    }
  }

  if (stopRequested) {
    Serial.println("Workout prerusen uzivatelem.");
  } else {
    Serial.println("HOTOVO!");
    beepEnd();
  }
}


void showMainMenu() {
  Serial.println();
  Serial.println("=== WORKOUT TIMER ===");
  Serial.println("[1] Vybrat workout");
  Serial.println("[2] Napsat vlastni workout (zatim neni hotove)");
  Serial.println("[0] Konec");
  Serial.print("Volba: ");
}

void showWorkoutMenu() {
  Serial.println();
  Serial.println("Vyber workout:");
  for (int i = 0; i < WORKOUT_COUNT; i++) {
    Serial.print("[");
    Serial.print(i + 1);
    Serial.print("] ");
    Serial.println(workouts[i].name);
  }
  Serial.print("Volba: ");
}

void setup() {
  Serial.begin(9600);
  buzzerInit(BUZZER_PIN);

  while (!Serial) {
    ; // cekej na Serial Monitor
  }

  showMainMenu();
}
