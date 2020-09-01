#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int hours, minutes;
  std::string fhours, fminutes, fseconds;
  std::string time;
  minutes = (int) ((seconds - (seconds %  60)) / 60);
  seconds -= minutes * 60;
  hours = ((minutes - (minutes %  60)) / 60);
  minutes -= hours * 60;
  
  if (seconds < 10){fseconds = "0" + std::to_string(seconds);}
  else {fseconds = std::to_string(seconds);}
  
  if (minutes < 10){fminutes = "0" + std::to_string(minutes);}
  else {fminutes = std::to_string(minutes);}
  
  if (hours < 10){fhours = "0" + std::to_string(hours);}
  else {fhours = std::to_string(hours);}
  
  time = fhours + ":" + fminutes + ":" + fseconds;
  return time; 
}
