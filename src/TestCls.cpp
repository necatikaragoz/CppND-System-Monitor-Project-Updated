#include "TestCls.h"
#include "ncurses_display.h"
#include "system.h"
#include "iostream"
#include "linux_parser.h"

void Test_LinuxParser()
{
  std::cout << "LinuxParser::TotalProcesses = " << LinuxParser::TotalProcesses() << "\n";
  std::cout << "LinuxParser::RunningProcesses = " << LinuxParser::RunningProcesses() << "\n";
  std::cout << "LinuxParser::MemoryUtilization = " << LinuxParser::MemoryUtilization() << "\n";
  std::cout << "LinuxParser::Jiffies = " << LinuxParser::Jiffies() << "\n";
  std::cout << "LinuxParser::ActiveJiffies = " << LinuxParser::ActiveJiffies() << "\n";
  std::cout << "LinuxParser::IdleJiffies = " << LinuxParser::IdleJiffies() << "\n";
  
}

void Test_PID()
{
    std::cout << "LinuxParser::Ram = " << LinuxParser::Ram(7141) << "\n";
    std::cout << "LinuxParser::Uid = " << LinuxParser::Uid(7141) << "\n";
    std::cout << "LinuxParser::User = " << LinuxParser::User(7141) << "\n";
    std::cout << "LinuxParser::ActiveJiffies = " << LinuxParser::ActiveJiffies(7141) << "\n";
    
    
}


void Test_Process()
{
  System sys;
  sys.Processes();
}





TestCls::TestCls()
{
  Test_LinuxParser();
  Test_PID();
  Test_Process();
}