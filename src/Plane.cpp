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

//void Plane::run() {
//	{
//		std::lock_guard<std::mutex> lock(globalCoutMutex);
//		std::cout << "RUN PLANE" << std::endl;
//	}
//	while (isRunning()) {
//		pos_.x_ += trajectory_.x_ * speed_ * 0.1f;
//		pos_.y_ += trajectory_.y_ * speed_ * 0.1f;
//		std::ostringstream os;
//		os << "Pos: " << name_ << "   " << (*this).fgetpos().x_ << "   " << (*this).fgetpos().y_ << "   " << (*this).fgetpos().altitude_ << std::endl;
//		{
//			std::lock_guard<std::mutex> lock(globalCoutMutex);
//			std::cout << os.str();
//		}
//		std::this_thread::sleep_for(std::chrono::milliseconds(100));
//		
//		Position currentPos = fgetpos();
//		float dx = target_->getPos().x_ - currentPos.x_;
//		float dy = target_->getPos().y_ - currentPos.y_;
//		float distance = sqrt(dx * dx + dy * dy);
//
//		if (distance < target_->getRadius()) {
//			std::cout << "Plane " << name_ << " is close to target, requesting landing..." << std::endl;
//			target_->getTwr()->landing(this);  
//			break;  
//		}
//	}
//
//	{
//		std::lock_guard<std::mutex> lock(globalCoutMutex);
//		std::cout << "Plane " << name_ << " LANDED" << std::endl;
//	}
//};

void Plane::run() {
    {
        std::lock_guard<std::mutex> lock(globalCoutMutex);
        std::cout << "RUN PLANE" << std::endl;
    }
    state_ = statePlane::TAKEOFF;
    float currentSpeed = 0.;
    while (isRunning()) {
        if (state_ == statePlane::TAKEOFF && currentSpeed < speed_) {
            currentSpeed += 1.0f;
            if (currentSpeed == speed_) {
                state_ = statePlane::FLYING;
            }
        }
        if (state_ == statePlane::FLYING) {
            currentSpeed = speed_;
        }
        if (state_ == statePlane::LANDING) {
            Position twrPos = target_->getTwr()->twrGetPos();
            float dx = twrPos.x_ - pos_.x_;
            float dy = twrPos.y_ - pos_.y_;
            float distanceToTwr = sqrt(dx * dx + dy * dy);
            if (distanceToTwr > 1.0f) {
                currentSpeed = std::max(0.0f, currentSpeed - (5.0f * (1.0f / distanceToTwr)));
            }
            else {
                currentSpeed = 0.0f;
            }
        }
        pos_.x_ += trajectory_.x_ * currentSpeed * 0.3f;
        pos_.y_ += trajectory_.y_ * currentSpeed * 0.3f;
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
            state_ = statePlane::LANDING;
            target_->getTwr()->landing(this);

            break;
        }
    }
    {
        std::lock_guard<std::mutex> lock(globalCoutMutex);
        std::cout << "Plane " << name_ << " LANDED" << std::endl;
    }
    state_ = statePlane::ONGROUND;
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

statePlane Plane::getState() {
	return state_;
}