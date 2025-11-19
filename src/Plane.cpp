#include <iostream>
#include <cmath>
#include "../include/Plane.hpp"
#include "../include/APP.hpp"
#include "../include/TWR.hpp"

// void requestlanding 
// void takeof 

Plane::Plane(const std::string& name, const float& speed, APP* target, TWR* spawn, std::mutex& mtx) : Agent(name, mtx), speed_(speed) {
	Position pos;
	Position trajectory;
	pos.altitude_ = 0;
	pos.x_ = spawn->twrGetPos().x_;
	pos.y_ = spawn->twrGetPos().y_;
	trajectory_.x_ = target->getPos().x_ - pos.x_;
	trajectory_.x_ = target->getPos().y_ - pos.x_;
	float norme = sqrt(trajectory_.x_ * trajectory_.x_ + trajectory_.y_ * trajectory_.y_);
	trajectory_.x_ = trajectory_.x_ / norme; 
	trajectory_.y_ = trajectory_.y_ / norme; 
};

void Plane::run() {
	std::cout << "RUN PLANE" << std::endl;
};



Position Plane::fgetpos() {
	return pos_;
};

