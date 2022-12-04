#pragma once

#include <string>
#include <vector>
#include <sstream>

class DummyNetwork
{
public:
	void write(std::string message);
	std::string read(int how_much);
private:
	std::stringstream buf;
};