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

void TWR::landing(Plane* plane) {
	// check si il y a de la place dans le parking et si l'avion n'est pas déjà garé
	// si oui stop plane 
	if (parkingSize_ - Parking.size() > 0 && std::find(Parking.begin(), Parking.end(), plane) == Parking.end()) {
		Parking.push_back(plane);
		plane->stop();
		std::cout << "Landing" << std::endl; 
		return; 
	}

}

Position TWR::twrGetPos() {
	return pos_;
}

std::vector<Plane*>& TWR::getParking() { 
	return Parking;
}

bool TWR::placeInParking() {
	// Vérifier si le parking n'est pas plein
	if (parkingSize_ - Parking.size() > 0) {
		return true;
	} else {
		return false;
	}
}


bool TWR::isParked(Plane& plane) {
	return std::find(Parking.begin(), Parking.end(), &plane) != Parking.end();
}