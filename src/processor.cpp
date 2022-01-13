#include "processor.h"
#include <cstddef>
#include <string>
#include "linux_parser.h"
using std::stof; 

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    // return stof(LinuxParser::CpuUtilization()[0]);
    float active = LinuxParser::ActiveJiffies();
    float idle = LinuxParser::IdleJiffies();
    return (active) / (active + idle);
 }