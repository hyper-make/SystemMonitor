#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int p)
    : pid(p),
      user(LinuxParser::User(p)),
      command(LinuxParser::Command(p)),
      cpuUtilization(LinuxParser::CpuUtilization(p)),
      ram(LinuxParser::Ram(p)),
      upTime(LinuxParser::UpTime(p)){};

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUtilization; }

// TODO: Return the command that generated this process
string Process::Command() { return command; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return upTime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  float x = a.ram.empty() ? 0 : std::stof(a.ram);
  float y = ram.empty() ? 0 : std::stof(ram);
  
  return x < y;
}
