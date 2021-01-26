#include <string>

#include "format.h"
#include <dirent.h>
#include <unistd.h>

using std::string;


// COMPLETED: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 

  const int secondsInMinutes = 60;
  const int secondsInHours = 3600;

  const long tHours = seconds / secondsInHours;
  const int tMinutes = (seconds % secondsInHours) / secondsInMinutes;
  const int tSeconds = seconds % secondsInMinutes;

  return CompleteFormat(tHours) + ":" + CompleteFormat(tMinutes) + ":" + CompleteFormat(tSeconds);
}

// string value should be double character
string Format::CompleteFormat(const long val) {
  if (val < 10)
    return "0" + std::to_string(val);
  else
    return std::to_string(val);
}