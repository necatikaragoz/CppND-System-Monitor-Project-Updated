#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:

  Process(const int pid);

  int Pid();                               // COMPLETED: See src/process.cpp
  std::string User();                      // COMPLETED: See src/process.cpp
  std::string Command();                   // COMPLETED: See src/process.cpp
  float CpuUtilization();                  // COMPLETED: See src/process.cpp
  std::string Ram();                       // COMPLETED: See src/process.cpp
  long int UpTime();                       // COMPLETED: See src/process.cpp
  bool operator<(Process const& a) const;  // COMPLETED: See src/process.cpp

  // COMPLETED: Declare any necessary private members
 private:
    float mCpuUtil;
    int   mPid;
    std::string mCommand;
    std::string mUser;
    long mUpTime;
    float mUtilization;
    std::string mRam;
  

};

#endif