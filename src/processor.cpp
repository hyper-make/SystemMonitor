#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  double CPU;
  
  std::string cpu, userstr, nicestr, systemstr, idlestr, iowaitstr, irqstr, softirqstr, stealstr;
  int user, nice, system, idle, iowait, irq, softirq, steal;
  
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if (stream.is_open()){
    
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> userstr >> nicestr >> systemstr >> idlestr >> iowaitstr >> irqstr >> softirqstr >> stealstr;
    
    user = std::stoi(userstr);
    nice = std::stoi(nicestr);
    system = std::stoi(systemstr);
    idle = std::stoi(idlestr);
    iowait = std::stoi(iowaitstr);
    irq = std::stoi(irqstr);
    softirq = std::stoi(softirqstr);
    steal = std::stoi(stealstr);
    
    int PrevIdle = previdle + previowait;
    int Idle = idle + iowait;

    int PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    int NonIdle = user + nice + system + irq + softirq + steal;

    int PrevTotal = PrevIdle + PrevNonIdle;
    int Total = Idle + NonIdle;

    int totald = Total - PrevTotal;
    int idled = Idle - PrevIdle;

    CPU = (totald - idled)/totald;
    
    prevuser = user;
    prevnice = nice;
    prevsystem = system;
    previdle = idle;
    previowait = iowait;
    previrq = irq;
    prevsoftirq = softirq;
    prevsteal = steal; 
  }
  return CPU;
}
