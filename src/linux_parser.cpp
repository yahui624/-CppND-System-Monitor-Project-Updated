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
using std::cout;

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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  // (total_memory - free_memory)/total_memory
  string line, total, free, temp;

  vector<long> memory{}; // store memory data for total, free, buffer, cache 
 
  std::ifstream filestream(kProcDirectory+kMeminfoFilename); 
  if (filestream.is_open()) {
    std::getline(filestream, line); 
    std::istringstream linestream(line); 
    linestream >>temp >> total; 
    // cout << "IN MemoryUtilization, key is: " << total;
    std::getline(filestream, line); 
    std::istringstream linestream1(line); 
    linestream1 >>temp >> free; 
  }
  return (stof(total) - stof(free))/stof(total); 

}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {  
  string line; 
  long up_time{0};
  std::ifstream filestream(kProcDirectory+kUptimeFilename); 
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line); 
    linestream >> up_time; 
  } 
  return up_time; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long total, val; 
  string line, cpu; 
  std::ifstream filestream(kProcDirectory+kStatFilename); 
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line); 
    linestream >> cpu;
    while(linestream >> val) {
      total += val; 
    } 
  } 
  return total; 

}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line, key;
  long val, active; 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line); 
    for (int i=0; i<13; ++i ) {
      linestream >> key; 
    }
    for (int i=0; i<4; ++i) {
      linestream >> val; 
      active += val;
    }
  } 
  return active;
 }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long val, user, nice, system, IRO, softIRO, steal; 
  vector<string> cpu_data = CpuUtilization();

  user = stol(cpu_data.at(CPUStates::kUser_));
  nice = stol(cpu_data.at(CPUStates::kNice_));
  system = stol(cpu_data.at(CPUStates::kSystem_));
  IRO = stol(cpu_data.at(CPUStates::kIRQ_));
  softIRO = stol(cpu_data.at(CPUStates::kSoftIRQ_));
  steal = stol(cpu_data.at(CPUStates::kSteal_));

  return user + nice + system + IRO + softIRO + steal; 

 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long idle, wait; 
  vector<string> cpu_data = CpuUtilization(); 
  
  idle = stol(cpu_data.at(CPUStates::kUser_));
  wait = stol(cpu_data.at(CPUStates::kIOwait_));

  return idle + wait; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, cpu, val;
  vector<string> Jiffies{}; 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line); 
    linestream >> cpu;
    while (linestream >> val) {
      Jiffies.push_back(val);
    }
  }
  return Jiffies; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int val;
  string line, key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){ 
      std::stringstream linestream(line);
      while (linestream >> key >> val){
        if (key == "processes") {
          return val;
        }
      }
    }
 }
 return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int val;
  string line, key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){ 
      std::stringstream linestream(line);
      while (linestream >> key >> val){
        if (key == "procs_running") {
          return val;
        }
      }
    }
 }
 return 0; 
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string cmd; 
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (stream.is_open()){
    std::getline(stream, cmd);
  }
  return cmd; 
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  float val{0.0};
  string line, key;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){ 
      std::stringstream linestream(line);
      while (linestream >> key >> val){
        if (key == "VmSize:") {
          return std::to_string(val/1000); // KB to MB
        }
      }
    }
 }
 return std::to_string(val/1000); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, val{};
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){ 
      std::stringstream linestream(line);
      while (linestream >> key >> val){
        if (key == "Uid:") {
          return val; 
        }
      }
    }
 }
 return val; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = Uid(pid); 
  string line, key, val{};
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){ 
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::stringstream linestream(line);
      while (linestream >> key >> val){
        if (key == uid) {
          return val; 
        }
      }
    }
 }
 return val; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, key;
  long val {0}; 
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i=0; i<21; ++i ) {
      linestream >> key;
    }
    linestream >> val;
    return val/sysconf(_SC_CLK_TCK); 
 }
 return val; 


 }


float LinuxParser::CpuUtilization(int pid) {
  string line, val; 
  long total_time, utime, stime, cutime, cstime, starttime;  
  float cpu{0.0}; 
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= 22; ++ i) {
      linestream >> val;
      // cout << " I M STUCK HERE " << val;
      if (i==14) {
        utime = stoi(val);
        // cout << "UTIME: " << utime;
      }
      if (i==15) {
        stime = stoi(val);
      }
      if (i==16) {
        cutime = stoi(val);
      }
      if (i==17) {
        cstime = stoi(val);
      }
      if (i==22) {
        starttime = stoi(val);
      }
      
    }
    total_time = utime + stime + cutime + cstime; 
    long upTime = LinuxParser::UpTime();
    long sec = upTime - starttime/sysconf(_SC_CLK_TCK);
    cpu = (total_time/sysconf(_SC_CLK_TCK)*1.0)/sec; 
  }
  return cpu;
}