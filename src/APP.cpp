#include <iostream>
#include <cmath>
#include <vector>
#include "../include/Plane.hpp"
#include "../include/APP.hpp"
#include "../include/TWR.hpp"

APP::APP(const std::string name, Position pos, const float& range, TWR* twr) : Agent(name), pos_(pos), range_(range), twr_(twr) {

};

Position APP::getPos() {

};


