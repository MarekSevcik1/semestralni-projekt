#include <Arduino.h>
#include "buzzer.h"
#include "workouts.h"
#include "lcd.h"

const int BUZZER_PIN = 10;

// Definice pinů pro tlačítka
const int PIN_NEXT  = 2;
const int PIN_START = 3;
const int PIN_STOP  = 4;

// ================== STATE =====================
enum SystemState { IDLE, RUNNING, STOPPING };
SystemState systemState = IDLE;
bool stopRequested = false;

int selectedWorkoutIndex = 0; // Který workout v seznamu je právě vybraný

// ======= DEKLARACE ======
void processCommand(String cmd);
void runWorkout(const Workout& w);
void emitState();
void emitWorkouts();
void countdownPhase(const char* phase, int seconds);
void checkButtons(); // Nová funkce pro čtení tlačítek

// ================== SETUP =====================
void setup() {
  Serial.begin(9600);
  buzzerInit(BUZZER_PIN);

  // Nastavení tlačítek s vnitřním pull-up rezistorem
  pinMode(PIN_NEXT, INPUT_PULLUP);
  pinMode(PIN_START, INPUT_PULLUP);
  pinMode(PIN_STOP, INPUT_PULLUP);

  lcdInit();
  
  // Po startu ukážeme první workout v seznamu
  lcdShowWorkout(workouts[selectedWorkoutIndex].name, 0, workouts[selectedWorkoutIndex].rounds);
  lcdShowPhase("READY", 0); 
  
  emitState();
}

// ================== LOOP ======================
void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    processCommand(cmd);
  }

  checkButtons();
}

// ================== TLAČÍTKA ======================
void checkButtons() {
  // Tlačítko NEXT (Listování)
  if (digitalRead(PIN_NEXT) == LOW) {
    if (systemState == IDLE) {
      delay(50); // Debounce
      selectedWorkoutIndex++;
      if (selectedWorkoutIndex >= WORKOUT_COUNT) {
        selectedWorkoutIndex = 0;
      }
      
      // Aktualizace displeje při výběru
      lcdShowWorkout(workouts[selectedWorkoutIndex].name, 0, workouts[selectedWorkoutIndex].rounds);
      lcdShowPhase("SELECT", 0);
      
      Serial.print("Selected: ");
      Serial.println(workouts[selectedWorkoutIndex].name);
      
      while(digitalRead(PIN_NEXT) == LOW); // Čekání na uvolnění
    }
  }

  // Tlačítko START
  if (digitalRead(PIN_START) == LOW) {
    if (systemState == IDLE && WORKOUT_COUNT > 0) {
      delay(50);
      stopRequested = false;
      systemState = RUNNING;
      
      runWorkout(workouts[selectedWorkoutIndex]);
      
      systemState = IDLE;
      // Po skončení ukážeme opět výběr
      lcdShowWorkout(workouts[selectedWorkoutIndex].name, 0, workouts[selectedWorkoutIndex].rounds);
      lcdShowPhase("READY", 0);
      
      while(digitalRead(PIN_START) == LOW);
    }
  }

  // Tlačítko STOP (pouze v IDLE pro reset nebo informaci)
  if (digitalRead(PIN_STOP) == LOW) {
    if (systemState == RUNNING) {
      stopRequested = true;
      systemState = STOPPING;
    }
    while(digitalRead(PIN_STOP) == LOW);
  }
}

// ================== HLAVNI CORE ======================
void processCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  if (cmd.length() == 0) return;

  // ----- STATUS -----
  if (cmd.startsWith("STATUS")) {
    emitState();
    return;
  }

  // ----- LIST -----
  if (cmd.startsWith("LIST")) {
    emitWorkouts();
    return;
  }

  // ----- STOP -----
  // Opraveno z "RUN" na "STOP", aby to dávalo smysl
  if (cmd.startsWith("STOP")) {
    if (systemState == RUNNING) {
      stopRequested = true;
      systemState = STOPPING;
      Serial.println("STOPPING...");
    }
    return;
  }

  // ----- ADD -----
  if (cmd.startsWith("ADD ")) {
    if (systemState != IDLE) return;

    String data = cmd.substring(4);
    int p1 = data.indexOf(',');
    int p2 = data.indexOf(',', p1 + 1);
    int p3 = data.indexOf(',', p2 + 1);

    if (p1 < 0 || p2 < 0 || p3 < 0) {
      Serial.println("ERR FORMAT");
      return;
    }

    String name = data.substring(0, p1);
    int work = data.substring(p1 + 1, p2).toInt();
    int rest = data.substring(p2 + 1, p3).toInt();
    int rounds = data.substring(p3 + 1).toInt();

    if (addWorkout(name.c_str(), work, rest, rounds)) {
      Serial.println("ADDED");
    } else {
      Serial.println("ERR FULL");
    }
    return;
  }

  // ----- START X -----
  if (cmd.startsWith("START")) {
    if (systemState != IDLE) return;

    int index = cmd.substring(5).toInt() - 1;
    if (index < 0 || index >= WORKOUT_COUNT) {
      Serial.println("ERR INDEX");
      return;
    }

    stopRequested = false;
    systemState = RUNNING;
    
    Serial.print("STARTED ");
    Serial.println(index + 1);

    runWorkout(workouts[index]);

    systemState = IDLE;
    emitState();
    return;
  }

  //======= DELETE =============
  if (cmd.startsWith("DELETE")) {
    int index = cmd.substring(6).toInt() - 1;
    if (deleteWorkout(index)) {
      Serial.println("DELETED");
    } else {
      Serial.println("ERROR");
    }
    return;
  }

  //======== RENAME ========
  if (cmd.startsWith("RENAME")) {
    int firstSpace = cmd.indexOf(' ');
    int secondSpace = cmd.indexOf(' ', firstSpace + 1);
    if (secondSpace == -1) return;

    int index = cmd.substring(firstSpace + 1, secondSpace).toInt() - 1;
    String newName = cmd.substring(secondSpace + 1);

    char buffer[NAME_LEN];
    newName.toCharArray(buffer, NAME_LEN);

    if (renameWorkout(index, buffer)) {
      Serial.println("RENAMED");
    } else {
      Serial.println("ERROR");
    }
    return;
  }
}

// ============= EMIT ==============
void emitState() {
  Serial.print("STATE ");
  if (systemState == IDLE) {
    Serial.println("IDLE");
    lcdShowIdle();
  }
  else if (systemState == RUNNING) Serial.println("RUNNING");
  else if (systemState == STOPPING) Serial.println("STOPPING");
}

void emitWorkouts() {
  for (int i = 0; i < WORKOUT_COUNT; i++) {
    Serial.print("WORKOUT ");
    Serial.print(i + 1);
    Serial.print(" ");
    Serial.println(workouts[i].name);
  }
  Serial.println("END");
}

// ================== RUN WORKOUT ===================
void runWorkout(const Workout& w) {
  lcdShowRunning();
  delay(1000); // Krátká pauza pro zobrazení stavu RUNNING

  beepStart(); 
  lcdRoundsTotal = w.rounds;

  for (int round = 1; round <= w.rounds; round++) {
    if (stopRequested) break;

    lcdRound = round;
    lcdShowWorkout(w.name, round, w.rounds);

    Serial.print("ROUND ");
    Serial.print(round);
    Serial.print("/");
    Serial.println(w.rounds);

    countdownPhase("WORK", w.workSeconds);
    if (stopRequested) break;

    if (round < w.rounds) {
      countdownPhase("REST", w.restSeconds);
      if (stopRequested) break;
    }
  }

  if (stopRequested) {
    Serial.println("STOPPED");
    lcdShowStopped();
  } else {
    Serial.println("DONE");
    beepEnd();
    lcdShowDone();
  }
  
  delay(2000); // Necháme výsledek chvíli na displeji
}

//============ COUNTDOWN =========
void countdownPhase(const char* phase, int seconds) {
  Serial.print("PHASE ");
  Serial.print(phase);
  Serial.print(" ");
  Serial.println(seconds);
  
  lcdPhase = phase;
  beepTransition(); 
  
  for (int i = seconds; i >= 0; i--) { // i >= 0 aby se ukázala i nula
    if (stopRequested) return;

    lcdSeconds = i;
    Serial.print("TICK ");
    Serial.println(i);
    
    // Voláme zobrazení fáze
    lcdShowPhase(phase, i);

    if (i <= 3 && i > 0) {
      beepTransition();
    }

    // Čekání 1 sekundu s kontrolou sériové linky
    unsigned long startMs = millis();
    while (millis() - startMs < 1000) {
      // Kontrola Serial STOPU
      if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim(); cmd.toUpperCase();
        if (cmd == "STOP") { stopRequested = true; systemState = STOPPING; return; }
      }
      
      // Kontrola fyzického STOP tlačítka
      if (digitalRead(PIN_STOP) == LOW) {
        stopRequested = true;
        systemState = STOPPING;
        return; 
      }
    }
  }
}
