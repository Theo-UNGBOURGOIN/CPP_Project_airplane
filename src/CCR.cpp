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
	while (isRunning()) {
		for (auto a : app_) {
			for (auto p : plane_) {
				if (pow(p->fgetpos().x_ - a->getPos().x_, 2) + pow(p->fgetpos().y_ - a->getPos().y_, 2) < pow(a->getRadius(), 2)) {
					handoverToAPP(a, p); 
					std::cout << "PLANE HANDOVER" << std::endl;
					p->stop();
				}
				else {
				    a->delPlane(p);

				}
			}

		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void CCR::handoverToAPP(APP* app, Plane* plane) {
	if (app->isPlaneInRange(*plane)) {
		app->addPlane(plane);
		return;
	}
	else {
		return;
	}
};


void CCR::addPlane(Plane& plane) {
	// avec ::find pour vérifier si l'avion est déjà dans la liste de CCR
	// si pas dans la liste on l'ajoute
	if (std::find(plane_.begin(), plane_.end(), &plane) != plane_.end()) {
		// l'avion est déjà dans la liste
		return;
	}
	plane_.push_back(&plane);

};

void CCR::addAPP(APP& app) {
	// avec ::find pour vérifier si l'app est déjà dans la liste de CCR
	// si pas dans la liste on l'ajoute
	if (std::find(app_.begin(), app_.end(), &app) != app_.end()) {
		// l'app est déjà dans la liste
		return;
	}
	app_.push_back(&app);

};