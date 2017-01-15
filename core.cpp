#include "exercise.h"
#include "induco.h"
#include <ctime>
#include <iostream>
#include <math.h>
#include <appareo.h>
#include <pessum.h>
int main() {
  appareo::InitializeAppareo();
  exercise::MainLoop();
  appareo::TerminateAppareo();
}
