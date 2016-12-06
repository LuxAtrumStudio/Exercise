#include "exercise.h"
#include "induco.h"
#include <ctime>
#include <iostream>
#include <math.h>
#include <pessum.h>
int main() {
  pessum::InitializePessumComponents();
  exercise::MainLoop();
  pessum::TerminatePessumComponents();
}
