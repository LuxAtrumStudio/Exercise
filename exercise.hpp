#ifndef EXERCISE_H_
#define EXERCISE_H_
#include <string>
#include <vector>
namespace exercise {
struct EType {
  std::string activitytype, grouptype;
  int activityindex, groupindex;
};
struct Exercise {
  int etypepointer;
  int reps, sets;
};
struct Workout {
  std::vector<Exercise> exercises;
  time_t date;
};
extern std::vector<EType> types;
extern std::vector<Workout> workouts;
extern std::vector<Exercise> goals;
extern std::string filelocation;
void MainLoop();
void DisplayRecord();
void DisplayGraph();
void DisplayWorkout(int pointer);
void LoadTypes();
void LoadWorkouts();
void LoadGoals();
void AddWorkout();
void SaveWorkouts();
bool SortCheck(const EType a, const EType b);
bool CheckStr(std::string str);
}
#endif
