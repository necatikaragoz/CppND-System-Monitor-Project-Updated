#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

namespace LinuxParser {
static int memData[MIO_LAST];
};

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}


// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


// COMPLETED: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  string key, unit, line;
  int value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    for (int i = 0; i < MIO_LAST; ++i) { 
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value >> unit;
    // MemInfo::mData.push_back(value);
    memData[i] = value;

    }
  }
  float utilization = (memData[MemInfoOrder::MIO_MemTotal] -
                      memData[MemInfoOrder::MIO_MemFree] -
                      memData[MemInfoOrder::MIO_Buffers] -
                      memData[MemInfoOrder::MIO_Cached]) /
                      (memData[MemInfoOrder::MIO_MemTotal] * 1.0);

  return utilization;
}

// COMPLETED: Read and return the system uptime
long LinuxParser::UpTime() { 
//NOTE: this funciton returns a long 
  long systemTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> systemTime;
  }
  //if stream doesn't open then return 0
  return systemTime;
}

// COMPLETED: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  
  vector<string> utilization = CpuUtilization();
  long sum{0};
  for(auto val: utilization) 
  {
    sum += stoi(val);
  }

  return sum;
}

// COMPLETED: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 

  string key, line;
  long int value{0}, activeJiffies{0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    // first 13 element of stat is not for jitties cmajflt values is the last value
    for (int i = 0; i < 13; ++i) {
      linestream >> key;
    }
    // after cmajflt time values now accessible
    for (int i = 0; i < 4; ++i) {
      linestream >> value;
      activeJiffies += value;
    }
    return activeJiffies;  //Sum of the utime, stime cutime cstime  
  }
  return activeJiffies;
}

// COMPLETED: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {

  vector<string> utilization = CpuUtilization();
  long sum{0};
  for(auto val: utilization) 
  {
    sum += stoi(val);
  }
  
  return  sum - stoi(utilization.at(kIdle_)) + stoi(utilization.at(kIOwait_)) ;

}

// COMPLETED: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  
  vector<string> utilization = CpuUtilization();
  return  stoi(utilization.at(kIdle_)) + stoi(utilization.at(kIOwait_)) ;
}

// COMPLETED: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 

  vector<string> utilization;
  string line, cpu;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::stringstream linestream(line);
    string temp;
    linestream >> cpu;
    while (linestream >> temp) {
      utilization.push_back(temp);
    }
  }

  return utilization; 

}

// COMPLETED: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::string KEY_TOTAL_PROCESS {"processes"};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    string line;
    string localKey;
    int value;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      while (linestream >> localKey >> value) {
        if (localKey == KEY_TOTAL_PROCESS) {
          return value;
        }
      }
    }
  }
  return 0;
}

// COMPLETED: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::string KEY_RUNNING_PROCESS {"procs_running"};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    string line;
    string localKey;
    int value;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      while (linestream >> localKey >> value) {
        if (localKey == KEY_RUNNING_PROCESS) {
          return value;
        }
      }
    }
  }
  return 0;
}

std::string GetKeyValue( std::string &path , std::string &key)  {
  string value;
  std::ifstream stream(path);
  
    if (stream.is_open()) {
    string line;
    string localKey;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      while (linestream >> localKey >> value) {
        if (localKey == key) {
          return value;
        }
      }
    }
  }
  return value;
}

// COMPLETED: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  
  //get the commandline path for defined pid
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line{};

  if (filestream.is_open()) {

    std::getline(filestream, line);

    return line;
  }
  // return null string:;
  return line;

}

// COMPLETED: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  
  //get the status path for defined pid
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  
  string key, line;
  int value;
  std::string KEY_RAM_SIZE {"VmSize:"};

  if (filestream.is_open()) {

    while (std::getline(filestream, line)) {

      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        
        if (key == KEY_RAM_SIZE) {
          return to_string(value / 1024);
        }
      }
    }
  }
  return "0";
}

// COMPLETED: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 

  //get the status path for defined pid
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  std::string KEY_UID {"Uid:"};
  string key, line, value{};

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == KEY_UID) {
          return value;
        }
      }
    }
  }
  return value;
}

// COMPLETED: Read and return the user associated with a process
string LinuxParser::User(int pid) { 

  string uid = Uid(pid);
  string line, key, value{};
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> key) {
        if (key == uid) {
          return value;
        }
      }
    }
  }
  return value;
}

// COMPLETED: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 

  string key, line;
  long int value{0};
  int utime  = 14;
  int starttime = 22;
  

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < (utime-1) ; ++i) {
      linestream >> key;
    }
    // utime updated
    linestream >>  utime >> value;

    value = (value + utime) ;/// sysconf(_SC_CLK_TCK);

   // std::cout << "utime = " << utime << " value = " << value << "\n";
    return utime;
  }
  return value;

}

