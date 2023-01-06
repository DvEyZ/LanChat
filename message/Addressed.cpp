#include "Addressed.h"

Addressed::Addressed(std::vector <std::string> r)
    :receivers(r)
{}

std::vector <std::string> Addressed::getReceivers()
{
    return receivers;
}