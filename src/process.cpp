#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


void Process::CalculateCpuUtilization(const long totalJiffies) {
  const float activeJiffies = (float)LinuxParser::ActiveJiffies(mPid);
  mCpuUtil = activeJiffies / totalJiffies;
}


Process::Process(const int pid, const long totalJiffies) : mPid(pid) {
  // COMPLETED: maybe generate all the parser operation in here and in the other funcitons return only the value
  mCommand = LinuxParser::Command(pid);
  mRam = LinuxParser::Ram(pid);
  mUpTime = LinuxParser::UpTime(pid);
  mUser = LinuxParser::User(pid);

  CalculateCpuUtilization(totalJiffies);

}

// COMPLETED: Return this process's ID
int Process::Pid() { 
    return mPid; 
    }

// COMPLETED: Return this process's CPU utilization
float Process::CpuUtilization() { 

float utilTime = (LinuxParser::OperationTime(this->mPid) * 1.0) / this->UpTime() ;
  return utilTime ;
  }

// COMPLETED: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(this->mPid); }

// COMPLETED: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(this->mPid); }

// COMPLETED: Return the user (name) that generated this process
string Process::User() {  return LinuxParser::User(this->mPid);}

// COMPLETED: Return the age of this process (in seconds)
long int Process::UpTime() { 

  long uptime = (LinuxParser::UpTime() - LinuxParser::UpTime(this->mPid) );  
 return  uptime; }

// COMPLETEDDO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return (this->mCpuUtil < a.mCpuUtil) ;
}