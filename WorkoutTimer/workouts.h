#ifndef WORKOUTS_H
#define WORKOUTS_H

struct Workout {
  const char* name;
  int workSeconds;
  int restSeconds;
  int rounds;
};

const Workout workouts[] = {
  { "Tabata Classic", 20, 10, 8 },
  { "Silovy zaklad", 40, 20, 6 },
  { "HIIT kratky", 30, 15, 6 },
  {"Kardio", 60, 30, 4},
  {"Stretch", 45, 15, 6}
};

const int WORKOUT_COUNT = sizeof(workouts) / sizeof(workouts[0]);

#endif
