#include <iostream>
#include <cmath>
#include "../include/Plane.hpp"
#include "../include/APP.hpp"
#include "../include/TWR.hpp"

// void requestlanding 
// void takeof 

std::mutex globalCoutMutex;

Plane::Plane(const std::string& name, const float& speed, APP* target, TWR* spawn, std::mutex& mtx) : target_(target), Agent(name, mtx), speed_(speed) {
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
	{
		std::lock_guard<std::mutex> lock(globalCoutMutex);
		std::cout << "RUN PLANE" << std::endl;
	}
	while (isRunning()) {
		pos_.x_ += trajectory_.x_ * speed_ * 0.1f;
		pos_.y_ += trajectory_.y_ * speed_ * 0.1f;
		std::ostringstream os;
		os << "Pos: " << name_ << "   " << (*this).fgetpos().x_ << "   " << (*this).fgetpos().y_ << "   " << (*this).fgetpos().altitude_ << std::endl;
		{
			std::lock_guard<std::mutex> lock(globalCoutMutex);
			std::cout << os.str();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		Position currentPos = fgetpos();
		float dx = target_->getPos().x_ - currentPos.x_;
		float dy = target_->getPos().y_ - currentPos.y_;
		float distance = sqrt(dx * dx + dy * dy);

		if (distance < target_->getRadius()) {
			std::cout << "Plane " << name_ << " is close to target, requesting landing..." << std::endl;
			target_->getTwr()->landing(this);  
			break;  
		}
	}

	{
		std::lock_guard<std::mutex> lock(globalCoutMutex);
		std::cout << "Plane " << name_ << " LANDED" << std::endl;
	}
};

void Plane::requestlanding(APP *target) {
    if (target->isPlaneInRange(*this)) {
        target->getTwr()->landing(this);
    }
};

void Plane::takeof(TWR* spawn) {
	
};

Position Plane::fgetpos() {
	return pos_;
};

Position Plane::getTrajectory() {
	return trajectory_;
};

