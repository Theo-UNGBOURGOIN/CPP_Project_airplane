#pragma once 
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include "Plane.hpp"
#include "Agent.hpp"

class CCR : public Agent {
private:
	std::vector<APP*> app_;
	std::vector<TWR*> twr_;
public:
};