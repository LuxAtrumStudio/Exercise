#ifndef INDUCO_H_
#define INDUCO_H_
#include <string>
#include <termios.h>
#include <vector>
namespace induco {
// Used to remove terminal echo of key press
extern struct termios t;
// Start time for timer
extern clock_t start;
// Provides options, and returns selected option
int Menu(std::string title, std::vector<std::string> options);
// Gets user input for a double
double GetValue(std::string value);
// Gets user input for a string
std::string SGetValue(std::string value);
// Converts elapsed time double to units of h, m, s, ms, us
std::string DisplayTime(double elapsed = 0, bool unit = false);
// Converts time in s to DATE
std::string DisplayDate(int timesec, bool disptime = false,
                        bool dispyear = false);
// Waits for any user input (Y/N)
bool Pause();
// Displays a line of size length
void Line(int size);
// Displays several line breaks followed by a line and several more line breaks
void Break();
// Toggles terminal echoing
void Echo(bool setting);
// Starts or stops a timmer with accuracy to us
double Timer(bool startb = false);
// Returns terminal width and height in characters
int GetSize(bool height = false);
// Sets the console output color
void SetColor(int color = 7);
}
#endif
