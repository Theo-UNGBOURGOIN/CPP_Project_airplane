#include <iostream>
#include <cmath>
#include <vector>
#include "../include/Plane.hpp"
#include "../include/APP.hpp"
#include "../include/TWR.hpp"

TWR::TWR(const std::string name, const int parkingSize, std::mutex& mtx, const int x, const int y) : Agent(name, mtx), parkingSize_(parkingSize) {
	srand(time(nullptr));
	Position pos;
	pos.x_ = x;
	pos.y_ = y; 
	pos_ = pos;
}

void TWR::run() {
	std::cout << "RUN TWR" << std::endl;
}

Position TWR::twrGetPos() {
	return pos_;
}