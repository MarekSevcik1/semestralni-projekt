#ifndef WORKOUTS_H
#define WORKOUTS_H

#define MAX_WORKOUTS 10
#define NAME_LEN 20

struct Workout {
  char name[NAME_LEN];
  int workSeconds;
  int restSeconds;
  int rounds;
};

//aktualni seznam
extern Workout workouts[MAX_WORKOUTS];    //globalni promenna, rozsireni viditelnosti
extern int WORKOUT_COUNT;

//api
bool addWorkout(const char* name, int work, int rest, int rounds);
bool deleteWorkout(int index);
bool renameWorkout(int index, const char* newName);


#endif
