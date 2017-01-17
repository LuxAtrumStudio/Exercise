#include "exercise.hpp"
#include <appareo.h>
#include <ctime>
#include <iostream>
#include <math.h>
#include <pessum.h>
int main() {
  appareo::InitializeAppareo();
  exercise::MainLoop();
  appareo::TerminateAppareo();
}
