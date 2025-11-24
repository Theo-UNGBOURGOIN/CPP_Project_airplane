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
	for(v in app_){
		for (p in plane_) {
			if (){

			}
			// et if plane is in range (verifier que les coordonnées sont dans le cerlce ) alors handover 
			// pas besoin de verifier si c'est la bonne target on verifie dans tout les cas et on donne 
			// (c'est pour le au cas ou il y un probleme)
			// else on delete
		}
	}
}

void CCR::handoverToAPP(APP& app, Plane& plane) {
	
};

void CCR::addPlane(Plane& plane) {

};

void CCR::deletePlane(Plane& plane) {

};