#include "processor.h"
#include "linux_parser.h"

// Get previous jiffies for initializing the jiffies
Processor::Processor() { 
  GetJiffies(PreviousJiffies); 
  }

void Processor::CalculateDeltaTime(
  array<long, CPUTimeEnum::LastEnumNo>& current,
  array<long, CPUTimeEnum::LastEnumNo>& previous) 
  {
    for (int i = 0; i < CPUTimeEnum::LastEnumNo; ++i) 
    {
      DifferenceJiffies[i] = current[i] - previous[i];
    }
}


float Processor::CalculateUtilization() {
  const float epsilon = 0.0000000001;
  const float util =
      DifferenceJiffies[CPUTimeEnum::ActiveTime] /
      (DifferenceJiffies[CPUTimeEnum::TotalTime] + epsilon);
  return util;
}

void Processor::GetJiffies(array<long, CPUTimeEnum::LastEnumNo>& jiff) {
  jiff[CPUTimeEnum::IdleTime] = LinuxParser::IdleJiffies();
  jiff[CPUTimeEnum::ActiveTime] = LinuxParser::ActiveJiffies();
  jiff[CPUTimeEnum::TotalTime] = LinuxParser::Jiffies();
}

long Processor::TotalJiffies() {
  return CurrentJiffies[CPUTimeEnum::TotalTime];
}

void Processor::UpdateJiffies() {
  for (size_t i = 0; i < PreviousJiffies.size(); ++i) {
    PreviousJiffies[i] = CurrentJiffies[i];
  }
}

// COMPLETED: Return the aggregate CPU utilization
float Processor::Utilization() {
  GetJiffies(CurrentJiffies);
  CalculateDeltaTime(CurrentJiffies, PreviousJiffies);
  const float util = CalculateUtilization();
  CurrentJiffies.swap(PreviousJiffies);
  return util;
}