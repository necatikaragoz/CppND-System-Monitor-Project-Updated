#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <array>
#include <vector>
using std::vector;
using std::array;



class Processor {
 public:

enum CPUTimeEnum { 
  IdleTime, 
  ActiveTime, 
  TotalTime,
  LastEnumNo
  };

  Processor();
  float Utilization(); // COMPLETED: See src/processor.cpp
  long TotalJiffies();

 private:
  void CalculateDeltaTime(array<long, CPUTimeEnum::LastEnumNo>&, array<long, CPUTimeEnum::LastEnumNo>&);
  void GetJiffies(array<long, CPUTimeEnum::LastEnumNo>&);
  float CalculateUtilization();
  void UpdateJiffies();


  std::array<long, CPUTimeEnum::LastEnumNo> CurrentJiffies;
  std::array<long, CPUTimeEnum::LastEnumNo> PreviousJiffies;
  std::array<float, CPUTimeEnum::LastEnumNo> DifferenceJiffies;

};

#endif