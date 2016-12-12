#ifndef EXERCISE_H_
#define EXERCISE_H_
#include <pessum.h>
#include <vector>
namespace exercise {
enum Activity { NONE = 0, SITUP = 1, PUSHUP = 2, CALFRAISE = 3 };
struct WorkOut {
  time_t date;
  Activity activity;
  int count;
  bool compleated;
};
extern std::vector<WorkOut> goals;
extern std::vector<WorkOut> workouts;
extern pessum::luxreader::DataFile goallux;
extern pessum::luxreader::DataFile workoutlux;
void MainLoop();
void DisplayExercise();
void LoadExerciseData();
void SaveExerciseData();
std::string GetActivity(Activity activity);
Activity IntActivity(int index);
void AddWorkOut();
void AddGoal();
void EditWorkOut(int pointer);
bool CheckGoal(int pointer);
bool SortCheck(WorkOut a, WorkOut b);
void GoalBackUp();
}
#endif
