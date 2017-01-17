#ifndef EXERCISE_H_
#define EXERCISE_H_
#include <pessum.h>
#include <vector>
namespace exercise {
enum Activity {
  NONE = 0,
  SITUP = 1,
  PUSHUP = 2,
  CALFRAISE = 3,
  SQUAT = 4,
  PLANK = 5
};
struct Exercise {
  time_t date;
  Activity activity;
  int count;
  bool compleated;
};
struct Workout {
  time_t date;
  std::vector<Activity> activities;
  std::vector<int> counts;
};
extern std::vector<Exercise> goals;
extern std::vector<Workout> workouts;
extern pessum::luxreader::DataFile goallux;
extern pessum::luxreader::DataFile workoutlux;
void MainLoop();
void DisplayExercise();
void LoadExerciseData();
void SaveExerciseData();
std::string GetActivity(Activity activity);
int StrActivityInt(std::string input);
Activity IntActivity(int index);
void AddWorkOut();
void AddGoal();
void EditWorkOut(int pointer);
bool CheckGoal(int pointer);
bool SortCheck(Workout a, Workout b);
void GoalBackUp();
}
#endif
