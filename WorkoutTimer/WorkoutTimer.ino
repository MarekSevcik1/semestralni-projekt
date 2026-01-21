#include "workouts.h"
#include <string.h>

Workout workouts[MAX_WORKOUTS] = {
  { "Tabata Classic", 20, 10, 8 },
  { "Silovy zaklad", 40, 20, 6 },
  { "HIIT kratky", 30, 15, 6 },
  { "Kardio", 60, 30, 4 },
  { "Stretch", 45, 15, 6 }
};

int WORKOUT_COUNT = 5;

//=====ADD=========
bool addWorkout(const char* name, int work, int rest, int rounds) {
  if (WORKOUT_COUNT >= MAX_WORKOUTS) return false;

  strncpy(workouts[WORKOUT_COUNT].name, name, NAME_LEN - 1);
  workouts[WORKOUT_COUNT].name[NAME_LEN - 1] = '\0';

  workouts[WORKOUT_COUNT].workSeconds = work;
  workouts[WORKOUT_COUNT].restSeconds = rest;
  workouts[WORKOUT_COUNT].rounds = rounds;

  WORKOUT_COUNT++;
  return true;
}

// ====== DELETE ======
bool deleteWorkout(int index) {
  if (index < 0 || index >= WORKOUT_COUNT) return false;

  for (int i = index; i < WORKOUT_COUNT - 1; i++) {
    workouts[i] = workouts[i + 1];
  }

  WORKOUT_COUNT--;
  return true;
}

// ====== RENAME ======
bool renameWorkout(int index, const char* newName) {
  if (index < 0 || index >= WORKOUT_COUNT) return false;

  strncpy(workouts[index].name, newName, NAME_LEN - 1);
  workouts[index].name[NAME_LEN - 1] = '\0';
  return true;
}
