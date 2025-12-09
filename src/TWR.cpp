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
	// Vérifie si le parking est vide ou non
	//     Si avions, 20% de chance de repartir toutes les 5 secondes
	//              Si il repart, on "reset" l'avion en changeant sa trajectoire et sa target aléatoirement 
	//				et on remet speed, altitude et pos avec les bonnes valeurs
	//				on remet l'etat en tAKE OFF
	while (isRunning()) {
		std::vector<Plane*> ParkingCopy = Parking;
		for(auto p : Parking){
			if (p != nullptr) {
				int nb = rand() % 200 ; 
				if (nb < 5) {
					p->setTarget(p->askAPPForNewTarget());
					while (p->askAPPForNewTarget() == p->getTarget()) {
						p->setTarget(p->askAPPForNewTarget());
					}
					{
						std::lock_guard<std::mutex> lock(mtx_);
						std::cout << "Plane " << p->getName() << " new target is " << p->getTarget()->getName() << std::endl;
						std::cout << "Plane " << p->getName() << " is taking off" << std::endl;
					}
					std::this_thread::sleep_for(std::chrono::seconds(5));
					// p->start();
					p->setState(statePlane::TAKEOFF);
					Parking.erase(std::find(Parking.begin(), Parking.end(), p));
				}
			}
		}
		
	}
}

void TWR::landing(Plane* plane) {
    if (placeInParking() && std::find(Parking.begin(), Parking.end(), plane) == Parking.end()) {
        // Vérifier que l'avion est très proche de la TWR
        Position planePos = plane->fgetpos();
        Position twrPos = this->twrGetPos();
        float dx = twrPos.x_ - planePos.x_;
        float dy = twrPos.y_ - planePos.y_;
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < 5.0f) {
            Parking.push_back(plane);
            // plane->stop();
            std::cout << "Landing" << std::endl;
        }
    }
    else {
        plane->setState(statePlane::HOLDING);
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

