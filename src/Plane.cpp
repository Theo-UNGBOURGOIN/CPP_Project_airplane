#include <iostream>
#include <cmath>
#include "../include/Plane.hpp"
#include "../include/TWR.hpp"

// void requestlanding 
// void takeof 

Plane::Plane(const std::string& name, const float& speed, APP* target, TWR* spawn) : Agent(name), speed_(speed) {
	Position pos;
	Position trajectory;
	pos.altitude_ = 0;
	pos.x_ = spawn->twrGetPos().x_;
	pos.y_ = spawn->twrGetPos().y_;
	trajectory_.x_ = target->GetPos().x_ - pos.x_;
	trajectory_.x_ = target->GetPos().y_ - pos.x_;
	double norme = sqrt(trajectory_.x_ * trajectory_.x_ + trajectory_.y_ * trajectory_.y_);
	trajectory_.x_ = trajectory_.x_ / norme; 
	trajectory_.y_ = trajectory_.y_ / norme; 
};

Plane::run() override {

};

Position fgetpos() {

};

void update_plane(Plane& plane, bool& stop_thread);

~Plane() {

}
;