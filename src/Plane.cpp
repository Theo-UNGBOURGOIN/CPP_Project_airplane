#include <iostream>
#include <cmath>
#include "../include/Plane.hpp"
#include "../include/APP.hpp"
#include "../include/TWR.hpp"
#include <iomanip>

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
    state_ = statePlane::TAKEOFF;
    float currentSpeed = 0.;
    float currentAltitude = -350.; // avoid starting altitude to 350
	Position holdingCenter_ = target_->getPos();
	float holdingAngle = atan2(pos_.y_ - holdingCenter_.y_, pos_.x_ - holdingCenter_.x_);  // Commence le holding à l'angle où l'avion arrive

    while (isRunning()) {
        if (state_ == statePlane::TAKEOFF && currentSpeed < speed_) {
            currentSpeed += 1.0f;
            currentAltitude += 350.0f;
            pos_.altitude_ = currentAltitude;
            if (currentSpeed == speed_) {
                state_ = statePlane::FLYING;
            }
        }
        if (state_ == statePlane::FLYING) {
            speed_ = currentSpeed;
            pos_.altitude_ = currentAltitude;
            Position currentPos = fgetpos();
            float dx = target_->getPos().x_ - currentPos.x_;
            float dy = target_->getPos().y_ - currentPos.y_;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < target_->getRadius()) {
                std::cout << "Plane " << name_ << " is close to target, requesting landing..." << std::endl;
                state_ = statePlane::LANDING;
                
            }
        }
        if (state_ == statePlane::HOLDING) {
            float range = target_->getRadius() - 1.0f;

            pos_.x_ = holdingCenter_.x_ + range * std::cos(holdingAngle) ;
            pos_.y_ = holdingCenter_.y_ + range * std::sin(holdingAngle) ;

            holdingAngle += 0.05f;

            if (holdingAngle > 2 * M_PI) {
                holdingAngle -= 2 * M_PI;
            }

            float nextX = holdingCenter_.x_ + range * std::cos(holdingAngle + 0.05f);
            float nextY = holdingCenter_.y_ + range * std::sin(holdingAngle + 0.05f);
            trajectory_.x_ = nextX - pos_.x_;
            trajectory_.y_ = nextY - pos_.y_;
            float norm = sqrt(trajectory_.x_ * trajectory_.x_ + trajectory_.y_ * trajectory_.y_);
            if (norm > 0) {
                trajectory_.x_ /= norm;
                trajectory_.y_ /= norm;
            }
        }
        if (state_ == statePlane::LANDING) {
            target_->getTwr()->landing(this);
            if (currentAltitude >= 0.0f) {
                currentAltitude -= 350.0f;
                pos_.altitude_ = currentAltitude;
            }
            Position twrPos = target_->getTwr()->twrGetPos();
            float dx = twrPos.x_ - pos_.x_;
            float dy = twrPos.y_ - pos_.y_;
            float distanceToTwr = sqrt(dx * dx + dy * dy);
            if (distanceToTwr > 1.0f) {
                currentSpeed = std::max(0.0f, currentSpeed - (50.0f * (1 / distanceToTwr)));
            }
            else {
                currentSpeed = 0.0f;
                
            }
        }

        // Ne pas appliquer le mouvement en holding (déjà fait dans le if)
        if (state_ != statePlane::HOLDING) {
            pos_.x_ += trajectory_.x_ * currentSpeed * 0.3f;
            pos_.y_ += trajectory_.y_ * currentSpeed * 0.3f;
        }

        std::ostringstream os;
        os << "Plane: " << name_
            << " | X: " << (*this).fgetpos().x_
            << " | Y: " << (*this).fgetpos().y_
            << " | Speed: " << currentSpeed
            << " | Altitude: " << (*this).fgetpos().altitude_ << std::endl;
        {
            std::lock_guard<std::mutex> lock(globalCoutMutex);
            std::cout << os.str();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
    }
    {
        std::lock_guard<std::mutex> lock(globalCoutMutex);
        std::cout << "Plane " << name_ << " LANDED" << std::endl;
    }
    state_ = statePlane::ONGROUND;
}


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

