#include "exercise.hpp"
#include <algorithm>
#include <appareo.h>
#include <pessum.h>
#include <string>
#include <vector>

namespace exercise {
std::vector<EType> types;
std::vector<Workout> workouts;
std::vector<Exercise> goals;
std::string filelocation = "/home/ardenrasmussen/bin/files/";
}

using namespace appareo::curse::out;

void exercise::MainLoop() {
  LoadTypes();
  LoadWorkouts();
  appareo::curse::InitializeWindow();
  appareo::curse::InitializeWindow();
  appareo::curse::windows[0].CreateWindow("Exercise", -1, 5, 0, 0, true, true);
  appareo::curse::windows[1].CreateWindow(
      "Record", -1, appareo::curse::scrheight - 8, 0, 5, true, true);
  bool running = true;
  int displaytype = 1;
  while (running == true) {
    if (displaytype == 1) {
      DisplayRecord();
    }
    appareo::curse::Field newfield;
    newfield.name = "In";
    newfield.type = 4;
    std::vector<appareo::curse::Field> formfields = {newfield};
    formfields = appareo::curse::NewForm(formfields, "Input", -1, 3, 0,
                                         appareo::curse::scrheight - 3);
    std::string input = formfields[0].sval;
    if (input == "q") {
      running = false;
    }
    if (input == "a") {
      AddWorkout();
    }
    if (input[0] == 'v') {
      input.erase(input.begin(), input.begin() + 2);
      if (CheckStr(input) == true) {
        DisplayWorkout(stoi(input) - 1);
      }
    }
  }
  SaveWorkouts();
}

void exercise::DisplayRecord() {
  int width, height, groupsize;
  width = appareo::curse::windows[1].width - 2;
  height = appareo::curse::windows[1].height - 2;
  BindWindow("Record");
  groupsize = width / 10;
  std::string labelline;
  labelline = "INDEX";
  while (labelline.size() >= groupsize * 2) {
    labelline.pop_back();
  }
  while (labelline.size() < groupsize * 2) {
    labelline += " ";
  }
  labelline += "DATE";
  while (labelline.size() >= groupsize * 5) {
    labelline.pop_back();
  }
  while (labelline.size() < groupsize * 5) {
    labelline += " ";
  }
  labelline += "GROUPS";
  while (labelline.size() >= groupsize * 10) {
    labelline.pop_back();
  }
  while (labelline.size() < groupsize * 10) {
    labelline += " ";
  }
  Print(labelline, 1, 1);
  if (workouts.size() == 0) {
    SetAtt({RED_TEXT, UNDERLINE});
    PrintZ("NO WORKOUTS RECORDED", 5);
    SetAtt();
  }
  for (int i = 0; i < workouts.size() && i < height - 6; i++) {
    std::string line = "";
    line += std::to_string(i + 1) + ". ";
    while (line.size() >= groupsize * 2) {
      line.pop_back();
    }
    while (line.size() < groupsize * 2) {
      line += " ";
    }
    line += appareo::induco::DisplayDate(workouts[i].date, false, true);
    while (line.size() >= groupsize * 5) {
      line.pop_back();
    }
    while (line.size() < groupsize * 5) {
      line += " ";
    }
    Print(line, i + 3, 1);
    for (int j = 0; j < workouts[i].exercises.size(); j++) {
      if (types[workouts[i].exercises[j].etypepointer].groupindex == 0) {
        SetAtt({BLUE_BACK});
      }
      if (types[workouts[i].exercises[j].etypepointer].groupindex == 1) {
        SetAtt({GREEN_BACK});
      }
      if (types[workouts[i].exercises[j].etypepointer].groupindex == 2) {
        SetAtt({CYAN_BACK});
      }
      if (types[workouts[i].exercises[j].etypepointer].groupindex == 3) {
        SetAtt({MAGENTA_BACK});
      }
      Print(" ", i + 3,
            (groupsize * 5) +
                types[workouts[i].exercises[j].etypepointer].groupindex);
    }
    SetAtt();
  }
  PrintZ("Record[r] | Edit[e] | ToggleDisplay[d] | ToggleUnits[u] | Quit[q]",
         7);
}

void exercise::DisplayGraph() {}

void exercise::DisplayWorkout(int pointer) {
  int window = appareo::curse::windows.size();
  Workout workout = workouts[pointer];
  appareo::curse::InitializeWindow();
  appareo::curse::windows[window].CreateWindow(
      appareo::induco::DisplayDate(workout.date, false, true),
      appareo::curse::scrwidth / 2, appareo::curse::scrheight / 2, -1, -1, true,
      true);
  for (int i = 0; i < workout.exercises.size(); i++) {
    if (types[workout.exercises[i].etypepointer].groupindex == 0) {
      SetAtt({BLUE_TEXT}, window);
    }
    if (types[workout.exercises[i].etypepointer].groupindex == 1) {
      SetAtt({GREEN_TEXT}, window);
    }
    if (types[workout.exercises[i].etypepointer].groupindex == 2) {
      SetAtt({CYAN_TEXT}, window);
    }
    if (types[workout.exercises[i].etypepointer].groupindex == 3) {
      SetAtt({MAGENTA_TEXT}, window);
    }
    Print(std::to_string(workout.exercises[i].sets) + "x" +
              std::to_string(workout.exercises[i].reps) + " " +
              types[workout.exercises[i].etypepointer].activitytype,
          i + 1, 1, window);
  }
  getch();
  appareo::curse::windows[window].TerminateWindow();
  appareo::curse::windows.erase(appareo::curse::windows.begin() + window);
}

void exercise::LoadTypes() {
  std::vector<std::string> luxtypes =
      pessum::luxreader::LoadLuxListFile(filelocation + "types");
  std::string group = "";
  std::string exercise = "";
  int groupindex = -1;
  int exerciseindex = -1;
  for (int i = 0; i < luxtypes.size(); i++) {
    if (luxtypes[i][0] == '>') {
      luxtypes[i].erase(luxtypes[i].begin());
      group = luxtypes[i];
      groupindex++;
    } else {
      exercise = luxtypes[i];
      exerciseindex++;
      EType newtype;
      newtype.activitytype = exercise;
      newtype.activityindex = exerciseindex;
      newtype.grouptype = group;
      newtype.groupindex = groupindex;
      types.push_back(newtype);
    }
  }
  std::sort(types.begin(), types.end(), SortCheck);
}

void exercise::LoadWorkouts() {
  pessum::luxreader::DataFile lux =
      pessum::luxreader::LoadLuxDataFile(filelocation + "workouts");
  int exerciseindex = 0;
  for (int i = 0; i < lux.datafilevariables[0].intvectorvalues.size(); i++) {
    Workout newworkout;
    newworkout.date = lux.datafilevariables[0].intvectorvalues[i];
    for (int j = exerciseindex;
         j < exerciseindex + (lux.datafilevariables[1].intvectorvalues[i] * 3);
         j++) {
      Exercise newexercise;
      newexercise.etypepointer = lux.datafilevariables[2].intvectorvalues[j];
      j++;
      newexercise.reps = lux.datafilevariables[2].intvectorvalues[j];
      j++;
      newexercise.sets = lux.datafilevariables[2].intvectorvalues[j];
      newworkout.exercises.push_back(newexercise);
    }
    workouts.push_back(newworkout);
    exerciseindex += (lux.datafilevariables[1].intvectorvalues[i] * 3);
  }
}

void exercise::LoadGoals() {}

void exercise::AddWorkout() {
  std::vector<appareo::curse::Field> formfields;
  appareo::curse::Field newfield;
  newfield.name = "Exercise Count";
  newfield.type = 1;
  newfield.sval = "5";
  formfields.push_back(newfield);
  formfields = appareo::curse::NewForm(formfields, "New Workout",
                                       appareo::curse::scrwidth / 2, 3);
  time_t currenttime;
  int activitycount;

  activitycount = formfields[0].ival;
  std::vector<std::string> options;
  for (int i = 0; i < types.size(); i++) {
    options.push_back(types[i].activitytype);
  }
  Workout newworkout;
  time(&currenttime);
  newworkout.date = currenttime;
  for (int i = 0; i < activitycount; i++) {
    formfields.clear();
    appareo::curse::Field field;
    field.name = "Exercise Type";
    field.type = 6;
    field.options = options;
    formfields.push_back(field);
    field.name = "Reps/Seconds";
    field.type = 1;
    field.sval = "1";
    formfields.push_back(field);
    field.name = "Sets";
    field.type = 1;
    field.sval = "1";
    formfields.push_back(field);
    formfields = appareo::curse::NewForm(
        formfields, "Exercise " + std::to_string(i + 1),
        appareo::curse::scrwidth / 2, appareo::curse::scrheight / 2);
    Exercise newexercise;
    newexercise.etypepointer = formfields[0].ival;
    newexercise.reps = formfields[1].ival;
    newexercise.sets = formfields[2].ival;
    newworkout.exercises.push_back(newexercise);
  }
  workouts.push_back(newworkout);
}

void exercise::SaveWorkouts() {
  pessum::luxreader::DataFile lux;
  lux.datafilename = "workouts";
  pessum::luxreader::Variable newvar;
  for (int i = 0; i < 3; i++) {
    lux.datafilevariables.push_back(newvar);
  }
  lux.datafilevariables[0].variablename = "date";
  lux.datafilevariables[0].variabletype = "vector";
  lux.datafilevariables[0].variabledefinitiontype = "int";
  lux.datafilevariables[1].variablename = "count";
  lux.datafilevariables[1].variabletype = "vector";
  lux.datafilevariables[1].variabledefinitiontype = "int";
  lux.datafilevariables[2].variablename = "data";
  lux.datafilevariables[2].variabletype = "vector";
  lux.datafilevariables[2].variabledefinitiontype = "int";
  for (int i = 0; i < workouts.size(); i++) {
    lux.datafilevariables[0].intvectorvalues.push_back(workouts[i].date);
    lux.datafilevariables[1].intvectorvalues.push_back(
        workouts[i].exercises.size());
    for (int j = 0; j < workouts[i].exercises.size(); j++) {
      lux.datafilevariables[2].intvectorvalues.push_back(
          workouts[i].exercises[j].etypepointer);
      lux.datafilevariables[2].intvectorvalues.push_back(
          workouts[i].exercises[j].reps);
      lux.datafilevariables[2].intvectorvalues.push_back(
          workouts[i].exercises[j].sets);
    }
  }
  pessum::luxreader::SaveLuxDataFile(filelocation + "workouts", lux);
}

bool exercise::SortCheck(const EType a, const EType b) {
  if (a.activitytype < b.activitytype) {
    return (true);
  }
  return (false);
}

bool exercise::CheckStr(std::string str) {
  for (int i = 0; i < str.size(); i++) {
    if (int(str[i]) < 48 || int(str[i]) > 57) {
      return (false);
    }
  }
  return (true);
}
