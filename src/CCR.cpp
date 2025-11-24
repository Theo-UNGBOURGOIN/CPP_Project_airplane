#include <iostream>
#include <cmath>
#include <vector>
#include "../include/Plane.hpp"
#include "../include/APP.hpp"
#include "../include/TWR.hpp"
#include "../include/CCR.hpp"

CCR::CCR(const std::string& name, std::mutex& mtx) : Agent(name, mtx) {

};

void CCR::run() {
	while(true){
		for (auto a : app_) {
			for ( auto p : plane_) {
				if( pow( p->fgetpos().x_ - a->getPos().x_ , 2 ) + pow(p->fgetpos().y_ - a->getPos().y_, 2) < a->getRadius()) {
					handoverToAPP(a, p);
				}
				else {
					deletePlane(a, p);
				}
			}
			
		}
	}
}

void CCR::handoverToAPP(APP& app, Plane& plane) {
	// ajouter l'avion à l'app si pas déjà fait dans le vecteur avion de l'app
	if (std::find(app.planeInRange_.begin(), app.planeInRange_.end(), &plane) != app.planeInRange_.end()) {
		// l'avion est déjà dans la liste
		return;
	}
	app.planeInRange_.push_back(&plane);
};

void CCR::addPlane(Plane& plane) {
	// avec ::find pour vérifier si l'avion est déjà dans la liste de CCR
	// si pas dans la liste on l'ajoute
	if( std::find(plane_.begin(), plane_.end(), &plane) != plane_.end()) {
		// l'avion est déjà dans la liste
		return;
	}
	plane_.push_back(&plane);
	
};

void CCR::deletePlane(APP& app, Plane& plane) {
	// avec ::find pour vérifier si l'avion est dans la liste
	// si dans la liste on le supprime
	if( std::find(app.planeInRange_.begin(), app.planeInRange_.end(), &plane) == app.planeInRange_.end()) {
		// l'avion n'est pas dans la liste
		return;
	}
	app.planeInRange_.erase(std::remove(app.planeInRange_.begin(), app.planeInRange_.end(), &plane), app.planeInRange_.end());
};