#include <string>
# include <iomanip>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    
    long HH = seconds/3600;
    seconds %= 60; 
    long MM = seconds/60;
    seconds %= 60; 

    // Save the output to certain format 
    std::stringstream output;
    output << std::setw[2] << std::setfill('0') << HH << ":" << std::setw[2] << std::setfill('0') << MM << ":" << std::setw[2] << std::setfill("0") << seconds; 

    return output.str();
                                                         
}