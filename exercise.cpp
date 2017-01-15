#include <appareo.h>
#include <math.h>
#include <pessum.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>
#include "exercise.h"

using namespace appareo::curse::out;

namespace exercise {
std::vector<Exercise> goals;
std::vector<Workout> workouts;
pessum::luxreader::DataFile goallux;
pessum::luxreader::DataFile workoutlux;
}

void exercise::MainLoop() {
  bool running = true;
  LoadExerciseData();
  // GoalBackUp();
  appareo::curse::InitializeWindow();
  appareo::curse::windows[0].CreateWindow(
      "Goals", -1, ceil(goals.size() / 3) + 3, 0, 0, true, true);
  appareo::curse::InitializeWindow();
  appareo::curse::windows[1].CreateWindow(
      "Workouts", -1, appareo::curse::scrheight - ceil(goals.size() / 3) - 6, 0,
      ceil(goals.size() / 3) + 3, true, true);
  std::string input = "";
  while (running == true) {
    DisplayExercise();
    appareo::curse::Field newfield;
    newfield.name = "In";
    newfield.type = 4;
    std::vector<appareo::curse::Field> formfields = {newfield};
    formfields = appareo::curse::NewForm(formfields, "Input", -1, 3, 0,
                                         appareo::curse::scrheight - 3);
    input = formfields[0].sval;
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
  width = appareo::curse::windows[0].width - 2;
  height = appareo::curse::windows[0].height - 2;
  groupsize = width / 6;
  std::string labelline;
  BindWindow("Goals");
  if (goals.size() == 0) {
    SetAtt({GREEN_TEXT});
    PrintZ("NO EXERCISE TODAY", 5);
    SetAtt();
  }
  int perrow = 3, row = 1;
  for (int i = 0; i < goals.size(); i) {
    for (int j = 0; j < perrow; j++) {
      std::vector<Attributes> atts;
      CheckGoal(i);
      std::string line = "";
      line = line + GetActivity(goals[i].activity) + " " +
             std::to_string(goals[i].count);
      if (goals[i].compleated == true) {
        atts.push_back(STANDOUT);
      }
      // induco::SetColor(goals[i].activity);
      if (goals[i].activity == 0) {
        atts.push_back(RED_TEXT);
      }
      if (goals[i].activity == 1) {
        atts.push_back(GREEN_TEXT);
      }
      if (goals[i].activity == 2) {
        atts.push_back(YELLOW_TEXT);
      }
      if (goals[i].activity == 3) {
        atts.push_back(BLUE_TEXT);
      }
      if (goals[i].activity == 4) {
        atts.push_back(MAGENTA_TEXT);
      }
      if (goals[i].activity == 5) {
        atts.push_back(CYAN_TEXT);
      }
      SetAtt(atts);
      Print(line, row,
            (j * width / perrow) + ((width / perrow) - line.size()) / 2);
      SetAtt();
      i++;
      if (i >= goals.size()) {
        break;
      }
    }
    row++;
  }
  BindWindow("Workouts");
  width = appareo::curse::windows[1].width;
  height = appareo::curse::windows[1].height;
  labelline = "DATE";
  while (labelline.size() < groupsize) {
    labelline += " ";
  }
  labelline += "ACTIVITIES";
  Print(labelline, 1, 1);
  if (workouts.size() == 0) {
    SetAtt({RED_TEXT});
    PrintZ("NO EXERCISES RECORDED", 5);
    SetAtt();
  }
  for (int i = 0; i < workouts.size() && i < height - 4; i++) {
    Print(appareo::induco::DisplayDate(workouts[i].date, false, false), i + 2,
          1);
    int pos = groupsize;
    for (int j = 0; j < workouts[i].activities.size(); j++) {
      std::string line = std::to_string(workouts[i].counts[j]) + "x" +
                         GetActivity(workouts[i].activities[j]);

      if (workouts[i].activities[j] == 0) {
        SetAtt({RED_TEXT});
      }
      if (workouts[i].activities[j] == 1) {
        SetAtt({GREEN_TEXT});
      }
      if (workouts[i].activities[j] == 2) {
        SetAtt({YELLOW_TEXT});
      }
      if (workouts[i].activities[j] == 3) {
        SetAtt({BLUE_TEXT});
      }
      if (workouts[i].activities[j] == 4) {
        SetAtt({MAGENTA_TEXT});
      }
      if (workouts[i].activities[j] == 5) {
        SetAtt({CYAN_TEXT});
      }
      Print(line, i + 2, pos + 1);
      pos += ((groupsize * 5) / workouts[i].activities.size());
      SetAtt();
    }
  }
  PrintZ("Record[r] | Add Goal[a] | Edit[e] | Quit[q]", 7);
}

void exercise::LoadExerciseData() {
  goallux = pessum::luxreader::LoadLuxDataFile("/home/arden/bin/files/goals");
  for (int i = 0; i < goallux.datafilevariables[0].intvectorvalues.size();
       i++) {
    Exercise newworkout;
    newworkout.activity =
        IntActivity(goallux.datafilevariables[0].intvectorvalues[i]);
    newworkout.count = goallux.datafilevariables[1].intvectorvalues[i];
    newworkout.compleated = false;
    goals.push_back(newworkout);
  }
  workoutlux =
      pessum::luxreader::LoadLuxDataFile("/home/arden/bin/files/workouts");
  int activitycounter = 0;
  for (int i = 0; i < workoutlux.datafilevariables[0].intvectorvalues.size();
       i++) {
    Workout newworkout;
    newworkout.date = workoutlux.datafilevariables[0].intvectorvalues[i];
    for (int j = activitycounter;
         j <
         activitycounter + workoutlux.datafilevariables[1].intvectorvalues[i];
         j++) {
      newworkout.activities.push_back(
          IntActivity(workoutlux.datafilevariables[2].intvectorvalues[j]));
      newworkout.counts.push_back(
          workoutlux.datafilevariables[3].intvectorvalues[j]);
    }
    activitycounter += workoutlux.datafilevariables[1].intvectorvalues[i];
    workouts.push_back(newworkout);
  }
}

void exercise::SaveExerciseData() {
  pessum::luxreader::SaveLuxDataFile("/home/arden/bin/files/goals", goallux);
  pessum::luxreader::SaveLuxDataFile("/home/arden/bin/files/workouts",
                                     workoutlux);
}

std::string exercise::GetActivity(Activity activity) {
  if (activity == SITUP) {
    return ("SITUP");
  } else if (activity == PUSHUP) {
    return ("PUSHUP");
  } else if (activity == CALFRAISE) {
    return ("CALF RAISE");
  } else if (activity == SQUAT) {
    return ("SQUAT");
  } else {
    return ("NULL");
  }
}

int exercise::StrActivityInt(std::string input) {
  if (input == "situp") {
    return (1);
  } else if (input == "pushup") {
    return (2);
  } else if (input == "calf raise") {
    return (3);
  } else if (input == "squat") {
    return (4);
  } else {
    return (0);
  }
}

exercise::Activity exercise::IntActivity(int index) {
  if (index == 1) {
    return (SITUP);
  } else if (index == 2) {
    return (PUSHUP);
  } else if (index == 3) {
    return (CALFRAISE);
  } else if (index == 4) {
    return (SQUAT);
  } else {
    return (NONE);
  }
}

void exercise::AddWorkOut() {
  std::vector<appareo::curse::Field> formfields;
  appareo::curse::Field newfield;
  newfield.name = "Activity Count";
  newfield.type = 1;
  newfield.sval = "4";
  formfields.push_back(newfield);
  formfields = appareo::curse::NewForm(formfields, "New Workout",
                                       appareo::curse::scrwidth / 2, 3);
  int activity;
  time_t currenttime;
  int count, activitycount;

  activitycount = formfields[0].ival;
  formfields.clear();
  for (int i = 0; i < activitycount; i++) {
    appareo::curse::Field field;
    field.name = "Activity " + std::to_string(i);
    field.type = 4;
    formfields.push_back(field);
    field.name = "Count " + std::to_string(i);
    field.type = 1;
    formfields.push_back(field);
  }
  formfields = appareo::curse::NewForm(formfields, "New Workout",
                                       appareo::curse::scrwidth / 2,
                                       appareo::curse::scrheight / 2);
  Workout newworkout;
  time(&currenttime);
  newworkout.date = currenttime;
  for (int i = 0; i < activitycount * 2; i += 2) {
    newworkout.activities.push_back(
        IntActivity(StrActivityInt(formfields[i].sval)));
    newworkout.counts.push_back(formfields[i + 1].ival);
  }
  workouts.push_back(newworkout);
  workoutlux.datafilevariables[0].intvectorvalues.push_back(newworkout.date);
  workoutlux.datafilevariables[1].intvectorvalues.push_back(
      newworkout.activities.size());
  for (int i = 0; i < activitycount; i++) {
    workoutlux.datafilevariables[2].intvectorvalues.push_back(
        newworkout.activities[i]);
    workoutlux.datafilevariables[3].intvectorvalues.push_back(
        newworkout.counts[i]);
  }
}

void exercise::AddGoal() {
  std::vector<appareo::curse::Field> formfields;
  appareo::curse::Field newfield;
  newfield.name = "Activity";
  newfield.type = 4;
  formfields.push_back(newfield);
  newfield.name = "Count";
  newfield.type = 1;
  formfields.push_back(newfield);
  formfields = appareo::curse::NewForm(formfields, "New Goal",
                                       appareo::curse::scrwidth / 2, 4);
  int activity;
  int count;
  activity = StrActivityInt(formfields[0].sval);
  count = formfields[1].ival;
  Exercise newworkout;
  newworkout.activity = IntActivity(activity);
  newworkout.count = count;
  goals.push_back(newworkout);
  goallux.datafilevariables[0].intvectorvalues.push_back(newworkout.activity);
  goallux.datafilevariables[1].intvectorvalues.push_back(newworkout.count);
}

void exercise::EditWorkOut(int pointer) {}

bool exercise::CheckGoal(int pointer) {
  time_t currenttime;
  struct tm current;
  struct tm workout;
  time(&currenttime);
  current = *localtime(&currenttime);
  for (int i = 0; i < workouts.size(); i++) {
    workout = *localtime(&workouts[i].date);
    if (workout.tm_yday == current.tm_yday) {
      for (int j = 0; j < workouts[i].activities.size(); j++) {
        if (workouts[i].activities[j] == goals[pointer].activity &&
            workouts[i].counts[j] == goals[pointer].count) {
          goals[pointer].compleated = true;
          return (true);
        }
      }
    }
    if (workout.tm_yday < current.tm_yday) {
      return (false);
    }
  }
  return (false);
}

bool exercise::SortCheck(Workout a, Workout b) { return (a.date > b.date); }

void exercise::GoalBackUp() {}
