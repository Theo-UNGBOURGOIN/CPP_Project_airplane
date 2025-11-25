#include <iostream>
#include <cmath>
#include "../include/Plane.hpp"
#include "../include/APP.hpp"
#include "../include/TWR.hpp"

// void requestlanding 
// void takeof 

Plane::Plane(const std::string& name, const float& speed, APP* target, TWR* spawn, std::mutex& mtx) : Agent(name, mtx), speed_(speed) {
	Position pos;
	pos.altitude_ = 0; // altitude de départ
	pos.x_ = spawn->twrGetPos().x_;
	pos.y_ = spawn->twrGetPos().y_;
	Position trajectory;
	trajectory.x_ = target->getPos().x_ - pos.x_;
	trajectory.y_ = target->getPos().y_ - pos.y_;
	float norme = sqrt(trajectory.x_ * trajectory.x_ + trajectory.y_ * trajectory.y_);
	trajectory.x_ = trajectory.x_ / norme;
	trajectory.y_ = trajectory.y_ / norme;
	pos_ = pos;
	trajectory_ = trajectory;
};

void Plane::run() {
	std::cout << "RUN PLANE" << std::endl;
	while (isRunning()) {
		pos_.x_ += trajectory_.x_ * speed_ * 0.1f;
		pos_.y_ += trajectory_.y_ * speed_ * 0.1f;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	std::cout << "PLANE LANDED" << std::endl;
};

void Plane::requestlanding(APP *target) {
	// if dans la zone du bon app alors tu request landing a app 
	// sinon tu continues d'avance
	// fais le code 

	if (pow(pos_.x_ - target->getPos().x_, 2) + pow(pos_.y_ - target->getPos().y_, 2) < 50) { 
		//target->receivePlane(this);
	}
};

void Plane::takeof(TWR* spawn) {
	
};

Position Plane::fgetpos() {
	return pos_;
};

