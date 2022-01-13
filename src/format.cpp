#include <string>
#include <iomanip>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    std::stringstream output; 
    int HH = seconds / 3600; 
    seconds %= 3600; 
    int MM = seconds / 60; 
    int SS = seconds % 60; 
    
    output << std::setw(2) << std::setfill('0') << HH << ":" << std::setw(2) << std::setfill('0') << MM << ":"  << std::setw(2) << std::setfill('0') << SS;

    return output.str();
}