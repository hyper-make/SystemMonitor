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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string key, mem;
  float memTotal, memFree, buffers;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> mem){
        if (key == "MemTotal"){
          memTotal = stof(mem);
        }
        if (key == "MemFree"){
          memFree = stof(mem);
        }
        if (key == "Buffers"){
          buffers = stof(mem);
        }
      }
    }
  }
  return (1.0 - (memFree / (memTotal - buffers)));
}

//Read and return the system uptime
long LinuxParser::UpTime() { 
  string seconds;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> seconds;
  }
  return std::stol(seconds);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long sec = LinuxParser::UpTime();
  long jiffie = sec * sysconf(_SC_CLK_TCK);
  return jiffie;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string value;
  long jiffies;
  vector<string>statsList;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream>>value) {
      statsList.push_back(value);
    } 
    jiffies = ( std::stol(statsList[13]) + std::stol(statsList[14]) + std::stol(statsList[15]) + std::stol(statsList[16])) / sysconf(_SC_CLK_TCK);
  }
  return jiffies; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line;
  long sum;
  vector<string>values;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0 ; i < 11; i++){
      linestream >> values[i];
    }
    for (int i = 1 ; i < 11; i++){
      sum = std::stol(values[i]);
    }
  }
  return sum; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line;
  long sum;
  std::ifstream stream(kProcDirectory + kStatFilename);
  vector<string>values;
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0 ; i < 6; i++){
      linestream >> values[i];
    }
    sum = std::stol(values[4]) + std::stol(values[5]);
  }
  return sum; 
}

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) { 
  float uptime = (float) UpTime();
  float cpu;
  vector<string> values;
  string value;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value){
      values.push_back(value);
    }
    
    int utime = std::stoi(values[13]);
    int stime = std::stoi(values[14]);
    int cutime = std::stoi(values[15]);
    int cstime = std::stoi(values[16]);
    int startTime = std::stoi(values[21]);
    
    int totalTime = utime + stime + cutime + cstime;
    float seconds = uptime - ((float) startTime / sysconf(_SC_CLK_TCK));
    cpu = ((float) totalTime / sysconf(_SC_CLK_TCK)) / seconds;
  }
  return cpu; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, value;
  string line;
  int proc;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "processes"){
          proc = std::stoi(value);
        }
      }
    }
  }
  return proc; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, value;
  string line;
  int proc;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "procs_running"){
          proc = std::stoi(value);
        }
      }
    }
  }
  return proc; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  string cmdline;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);
  }
  return line; 
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  float mem;
  string key, value;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatusFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::replace(line.begin(), line.end(),':',' ');
    std::istringstream linestream(line);
    while (linestream >> key >> value){
      if (key == "VmSize"){
        mem = std::stof(value);
      }
    }
  }
  
  return std::to_string(mem / 1000); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string uid;
  string key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::replace(line.begin(), line.end(),':',' ');
    std::istringstream linestream(line);
    while (linestream >> key >> value){
      if (key == "Uid"){
        uid = value;
      }
    }
  }
  return uid; 
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string line;
  string user, value;
  string name;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':',' ');
      std::replace(line.begin(), line.end(), 'x',' ');
      std::istringstream linestream(line);
      while (linestream >> user >> value){
        if (value==uid){
          name = user;
        }
      }
    }
  } 
  return name;
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  string temp;
  long output;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i=0; i < 22; i++) {
      linestream >> temp;
    }
    long output = stof(temp) / sysconf(_SC_CLK_TCK);
  }
  return output;
}
