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
	
};

void CCR::addPlane(APP& app, Plane& plane) {

};

void CCR::deletePlane(APP& app, Plane& plane) {

};