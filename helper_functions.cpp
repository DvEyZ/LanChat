#include "helper_functions.h"

std::string intts(int number, int digits = 0)
{
    if (digits == 0) return std::to_string(number);
    else
    {
        std::ostringstream temp;
        temp << std::setfill('0') << std::setw(digits) << number;
        return temp.str();
    }
}