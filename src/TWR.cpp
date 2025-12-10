#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "../include/Plane.hpp"
#include "../include/APP.hpp"
#include "../include/TWR.hpp"

TWR::TWR(const std::string name, const int parkingSize, std::mutex& mtx, const int x, const int y) : Agent(name, mtx), parkingSize_(parkingSize) {
	srand(static_cast<unsigned int>(time(nullptr)));
	Position pos;
	pos.x_ = x;
	pos.y_ = y;
	pos_ = pos;
}

void TWR::run() {
	{
		std::lock_guard<std::mutex> lock(mtx_);
		std::cout << "RUN TWR " << name_ << std::endl;
	}

	while (isRunning()) {
		std::vector<Plane*> planesToTakeOff;

		{
			std::lock_guard<std::mutex> lock(mtx_);
			std::vector<Plane*> parkingCopy = Parking;

			for (auto* p : parkingCopy) {
				if (p == nullptr) continue;

				if (p->getState() != statePlane::ONGROUND) {
					continue;
				}

				int nb = rand() % 100;
				if (nb < 5) {
					planesToTakeOff.push_back(p);
				}
			}
		}

		for (auto* p : planesToTakeOff) {
			APP* newTarget = p->askAPPForNewTarget();
			int safety = 0;
			while (newTarget == p->getTarget() && safety < 10) {
				newTarget = p->askAPPForNewTarget();
				++safety;
			}

			if (newTarget != nullptr && newTarget != p->getTarget()) {
				p->setTarget(newTarget);
			}

			{
				std::lock_guard<std::mutex> lock(mtx_);
				std::cout << "Plane " << p->getName() << " new target is " << p->getTarget()->getName() << std::endl;
				std::cout << "Plane " << p->getName() << " is taking off" << std::endl;

				auto it = std::remove(Parking.begin(), Parking.end(), p);
				Parking.erase(it, Parking.end());
			}

			p->setState(statePlane::TAKEOFF);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void TWR::landing(Plane* plane) {
	// si place disponible et avion pas déjà garé
	Position planePos = plane->fgetpos();
	Position twrPos = this->twrGetPos();
	float dx = twrPos.x_ - planePos.x_;
	float dy = twrPos.y_ - planePos.y_;
	float distance = std::sqrt(dx * dx + dy * dy);

	std::lock_guard<std::mutex> lock(mtx_);
	bool hasRoom = parkingSize_ - static_cast<int>(Parking.size()) > 0;
	bool alreadyParked = std::find(Parking.begin(), Parking.end(), plane) != Parking.end();

	if (hasRoom && !alreadyParked && distance < 5.0f) {
		Parking.push_back(plane);
		plane->setState(statePlane::ONGROUND);
		std::cout << "Plane " << plane->getName() << " parked at " << name_ << std::endl;
	}
	else if (!hasRoom && !alreadyParked) {
		// pas de place -> renvoyer l'avion en HOLDING
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
	std::lock_guard<std::mutex> lock(mtx_);
	return parkingSize_ - static_cast<int>(Parking.size()) > 0;
}

bool TWR::isParked(Plane& plane) {
	std::lock_guard<std::mutex> lock(mtx_);
	return std::find(Parking.begin(), Parking.end(), &plane) != Parking.end();
}

