#include "exercise.h"
#include "induco.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <math.h>
#include <pessum.h>
#include <vector>

namespace exercise {
std::vector<WorkOut> goals;
std::vector<WorkOut> workouts;
pessum::luxreader::DataFile goallux;
pessum::luxreader::DataFile workoutlux;
}

void exercise::MainLoop() {
  bool running = true;
  LoadExerciseData();
  std::string input = "";
  while (running == true) {
    DisplayExercise();
    getline(std::cin, input);
    if (input == "q") {
      running = false;
    }
    if (input == "r") {
      AddWorkOut();
    }
    if (input == "a") {
      AddGoal();
    }
    if (input[0] == 'e') {
    }
  }
  SaveExerciseData();
}

void exercise::DisplayExercise() {
  std::sort(workouts.begin(), workouts.end(), SortCheck);
  int width, height, groupsize;
  width = induco::GetSize();
  height = induco::GetSize(true);
  groupsize = width / 6;
  std::cout << "EXERCISE\n";
  std::string labelline;
  induco::Line(width);
  labelline = "TODAY:";
  if (goals.size() == 0) {
    induco::SetColor(2);
    std::cout << "NO EXERCISES FOR TODAY\n";
    induco::SetColor(7);
  }
  for (int i = 0; i < goals.size(); i++) {
    CheckGoal(i);
    std::string line = "";
    line = line + GetActivity(goals[i].activity) + " " +
           std::to_string(goals[i].count);
    if (goals[i].compleated == true) {
      std::cout << "\e[9m";
    }
    induco::SetColor(goals[i].activity);
    std::cout << line;
    if (goals[i].compleated == true) {
      std::cout << "\e[0m";
    }
    std::cout << "\n";
    induco::SetColor(7);
  }
  labelline = "DATE";
  while (labelline.size() < groupsize) {
    labelline += " ";
  }
  labelline += "ACTIVITY";
  while (labelline.size() < groupsize * 2) {
    labelline += " ";
  }
  labelline += "COUNT\n";
  induco::Line(width);
  std::cout << labelline;
  if (workouts.size() == 0) {
    induco::SetColor(4);
    std::cout << "NO EXERCISES RECORDED\n";
    induco::SetColor(7);
  }
  for (int i = 0; i < workouts.size(); i++) {
    std::string line = "";
    line = induco::DisplayDate(workouts[i].date, false, false);
    while (line.size() < groupsize) {
      line += " ";
    }
    line += GetActivity(workouts[i].activity);
    while (line.size() < groupsize * 2) {
      line += " ";
    }
    line += std::to_string(workouts[i].count);
    induco::SetColor(workouts[i].activity);
    std::cout << line;
    std::cout << "\n";
    induco::SetColor(7);
  }
  for (int i = workouts.size() + goals.size(); i < height - 7; i++) {
    std::cout << "\n";
  }
  induco::Line(width);
  std::cout << "Record[r] | Add Goal[a] | Edit[e] | Quit[q]\n";
}

void exercise::LoadExerciseData() {
  goallux = pessum::luxreader::LoadLuxDataFile("goals");
  for (int i = 0; i < goallux.datafilevariables[0].intvectorvalues.size();
       i++) {
    WorkOut newworkout;
    newworkout.activity =
        IntActivity(goallux.datafilevariables[0].intvectorvalues[i]);
    newworkout.count = goallux.datafilevariables[1].intvectorvalues[i];
    goals.push_back(newworkout);
  }
  workoutlux = pessum::luxreader::LoadLuxDataFile("workouts");
  for (int i = 0; i < workoutlux.datafilevariables[0].intvectorvalues.size();
       i++) {
    WorkOut newworkout;
    newworkout.date = workoutlux.datafilevariables[0].intvectorvalues[i];
    newworkout.activity =
        IntActivity(workoutlux.datafilevariables[1].intvectorvalues[i]);
    newworkout.count = workoutlux.datafilevariables[2].intvectorvalues[i];
    workouts.push_back(newworkout);
  }
}

void exercise::SaveExerciseData() {
  pessum::luxreader::SaveLuxDataFile("goals", goallux);
  pessum::luxreader::SaveLuxDataFile("workouts", workoutlux);
}

std::string exercise::GetActivity(Activity activity) {
  if (activity == SITUP) {
    return ("SITUP");
  } else if (activity == PUSHUP) {
    return ("PUSHUP");
  } else if (activity == CALFRAISE) {
    return ("CALF RAISE");
  } else {
    return ("NULL");
  }
}

exercise::Activity exercise::IntActivity(int index) {
  if (index == 1) {
    return (SITUP);
  } else if (index == 2) {
    return (PUSHUP);
  } else if (index == 3) {
    return (CALFRAISE);
  } else {
    return (NONE);
  }
}

void exercise::AddWorkOut() {
  int activity;
  time_t currenttime;
  int count;
  activity = induco::GetValue("Activity [1-3]");
  count = induco::GetValue("Count");
  WorkOut newworkout;
  time(&currenttime);
  newworkout.date = currenttime;
  newworkout.activity = IntActivity(activity);
  newworkout.count = count;
  workouts.push_back(newworkout);
  workoutlux.datafilevariables[0].intvectorvalues.push_back(newworkout.date);
  workoutlux.datafilevariables[1].intvectorvalues.push_back(
      newworkout.activity);
  workoutlux.datafilevariables[2].intvectorvalues.push_back(newworkout.count);
}

void exercise::AddGoal() {
  int activity;
  int count;
  activity = induco::GetValue("Activity [1-3]");
  count = induco::GetValue("Count");
  WorkOut newworkout;
  newworkout.activity = IntActivity(activity);
  newworkout.count = count;
  goals.push_back(newworkout);
  goallux.datafilevariables[0].intvectorvalues.push_back(newworkout.activity);
  goallux.datafilevariables[1].intvectorvalues.push_back(newworkout.count);
}

void exercise::EditWorkOut(int pointer) {}

bool exercise::CheckGoal(int pointer) {
  time_t currenttime;
  struct tm *current;
  struct tm *workout;
  time(&currenttime);
  current = gmtime(&currenttime);
  int day = current->tm_yday;
  for (int i = 0; i < workouts.size(); i++) {
    workout = gmtime(&workouts[i].date);
    int workoutday = workout->tm_yday;
    if (day == workoutday) {
      if (workouts[i].activity == goals[pointer].activity &&
          workouts[i].count == goals[pointer].count) {
        goals[pointer].compleated = true;
        return (true);
      }
    }
    if (workout->tm_yday < current->tm_yday) {
      return (false);
    }
  }
}

bool exercise::SortCheck(WorkOut a, WorkOut b) { return (a.date > b.date); }
