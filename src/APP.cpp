#include <iostream>
#include <cmath>
#include <vector>
#include "../include/Plane.hpp"
#include "../include/APP.hpp"
#include "../include/TWR.hpp"


APP::APP(const std::string name, const float& range, TWR* twr, CCR* ccr, std::mutex& mtx) : Agent(name, mtx), range_(range), twr_(twr), ccr_(ccr) {
	pos_ = twr->twrGetPos();
};

void APP::run() {
	std::cout << "RUN APP" << std::endl;
}; 


float APP::getRadius() {
	return range_;
}

Position APP::getPos() {
	return pos_;
	// return twr_->twrGetPos();
}

bool APP::isPlaneInRange(Plane& plane){
	if(std::find(planeInRange_.begin(), planeInRange_.end(), &plane) != planeInRange_.end()) {
		// l'avion est dans la liste
		return true;
	}
	else{
		return false; 
	}
};

void APP::addPlane(Plane* plane) {
	if (std::find(planeInRange_.begin(), planeInRange_.end(), plane) == planeInRange_.end()) {
		planeInRange_.push_back(plane); 
	}
};

void APP::delPlane(Plane* plane) {
	if (auto itr = std::find(planeInRange_.begin(), planeInRange_.end(), plane); itr != planeInRange_.end()) {
		planeInRange_.erase(itr);
	}
};

TWR* APP::getTwr() {
	return twr_;
}

std::vector<Plane*>& APP::getWhosInRange() {
	return planeInRange_;
}


APP* APP::askForNewTarget() {
	return ccr_->newTarget();
}