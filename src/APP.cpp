#include <iostream>
#include <cmath>
#include <vector>
#include "../include/Plane.hpp"
#include "../include/APP.hpp"
#include "../include/TWR.hpp"

APP::APP(const std::string name, const float& range, TWR* twr, std::mutex& mtx) : Agent(name, mtx), range_(range), twr_(twr) {

};


void APP::run() {
	std::cout << "RUN APP" << std::endl;
}



Position APP::getPos() {
	return pos_;
}

void receivePlane(Plane* plane) {

}