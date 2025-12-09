#include <iostream>
#include <cmath>
#include "../include/Plane.hpp"
#include "../include/APP.hpp"
#include "../include/TWR.hpp"
#include <iomanip>

std::mutex globalCoutMutex;

Plane::Plane(const std::string& name, const float& speed, APP* target, TWR* spawn, std::mutex& mtx)
    : target_(target), Agent(name, mtx), speed_(speed) {
    Position pos;
    pos.altitude_ = 0; // altitude de départ
    pos.x_ = spawn->twrGetPos().x_;
    pos.y_ = spawn->twrGetPos().y_;

    Position trajectory;
    trajectory.x_ = target->getPos().x_ - pos.x_;
    trajectory.y_ = target->getPos().y_ - pos.y_;
    float norme = std::sqrt(trajectory.x_ * trajectory.x_ + trajectory.y_ * trajectory.y_);
    if (norme > 0.0f) {
        trajectory.x_ /= norme;
        trajectory.y_ /= norme;
    }

    pos_ = pos;
    trajectory_ = trajectory;
};

void Plane::run() {
    {
        std::lock_guard<std::mutex> lock(mtx_);
        std::cout << "RUN PLANE " << name_ << std::endl;
    }
    state_ = statePlane::TAKEOFF;
    float currentSpeed = 0.0f;
    float currentAltitude = -350.0f; // pour éviter de commencer directement haut

    Position holdingCenter_ = target_->getPos();
    float holdingAngle = std::atan2(pos_.y_ - holdingCenter_.y_, pos_.x_ - holdingCenter_.x_);

    while (isRunning()) {
        if (state_ == statePlane::TAKEOFF && currentSpeed < speed_) {
            currentSpeed += 1.0f;
            currentAltitude += 350.0f;
            pos_.altitude_ = currentAltitude;
            if (currentSpeed >= speed_) {
                currentSpeed = speed_;
                state_ = statePlane::FLYING;
            }
        }

        if (state_ == statePlane::FLYING) {
            pos_.altitude_ = currentAltitude;

            Position currentPos = fgetpos();
            float dx = target_->getPos().x_ - currentPos.x_;
            float dy = target_->getPos().y_ - currentPos.y_;
            float distance = std::sqrt(dx * dx + dy * dy);

            // recalcule la trajectoire vers la target
            if (distance > 0.1f) {
                trajectory_.x_ = dx / distance;
                trajectory_.y_ = dy / distance;
            }

            // proche de l'APP : demande d'atterrissage (LANDING)
            if (distance < target_->getRadius()) {
                {
                    std::lock_guard<std::mutex> lock(mtx_);
                    std::cout << "Plane " << name_ << " is close to target " << target_->getName() << ", requesting landing..." << std::endl;
                }
                state_ = statePlane::LANDING;
            }
        }

        if (state_ == statePlane::HOLDING) {
            if (target_->getTwr()->placeInParking()) {
                Position currentPos = fgetpos();
                Position twrPos = target_->getTwr()->twrGetPos();
                float dx = twrPos.x_ - currentPos.x_;
                float dy = twrPos.y_ - currentPos.y_;
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance > 0.1f) {
                    trajectory_.x_ = dx / distance;
                    trajectory_.y_ = dy / distance;
                }

                state_ = statePlane::LANDING;
            } else {
                float range = target_->getRadius() - 1.0f;
                if (range < 1.0f) range = 1.0f;

                pos_.x_ = holdingCenter_.x_ + range * std::cos(holdingAngle);
                pos_.y_ = holdingCenter_.y_ + range * std::sin(holdingAngle);

                holdingAngle += 0.05f;
                if (holdingAngle > 2 * M_PI) {
                    holdingAngle -= 2 * M_PI;
                }

                float nextX = holdingCenter_.x_ + range * std::cos(holdingAngle + 0.05f);
                float nextY = holdingCenter_.y_ + range * std::sin(holdingAngle + 0.05f);
                trajectory_.x_ = nextX - pos_.x_;
                trajectory_.y_ = nextY - pos_.y_;
                float norm = std::sqrt(trajectory_.x_ * trajectory_.x_ + trajectory_.y_ * trajectory_.y_);
                if (norm > 0.0f) {
                    trajectory_.x_ /= norm;
                    trajectory_.y_ /= norm;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        if (state_ == statePlane::LANDING) {
            // descente progressive NON VISIBLE POUR L4INSTANT
            if (currentAltitude > 0.0f) {
                currentAltitude -= 350.0f;
                if (currentAltitude < 0.0f) currentAltitude = 0.0f;
                pos_.altitude_ = currentAltitude;
            }

            Position twrPos = target_->getTwr()->twrGetPos();
            float dx = twrPos.x_ - pos_.x_;
            float dy = twrPos.y_ - pos_.y_;
            float distanceToTwr = std::sqrt(dx * dx + dy * dy);

            if (distanceToTwr > 1.0f) {
                currentSpeed = std::max(0.0f, currentSpeed - (50.0f * (1.0f / distanceToTwr)));
            } else {
                currentSpeed = 0.0f;
            }

            if (distanceToTwr > 0.1f) {
                trajectory_.x_ = dx / distanceToTwr;
                trajectory_.y_ = dy / distanceToTwr;
            }

            target_->getTwr()->landing(this);
        }


        if (state_ != statePlane::HOLDING) {
            pos_.x_ += trajectory_.x_ * currentSpeed * 0.3f;
            pos_.y_ += trajectory_.y_ * currentSpeed * 0.3f;
        }

        std::ostringstream os;
        os << "Plane: " << name_
           << " | X: " << fgetpos().x_
           << " | Y: " << fgetpos().y_
           << " | Speed: " << currentSpeed
           << " | Altitude: " << fgetpos().altitude_ << std::endl;
        {
            std::lock_guard<std::mutex> lock(mtx_);
            std::cout << os.str();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    {
        std::lock_guard<std::mutex> lock(mtx_);
        std::cout << "Plane " << name_ << " THREAD STOPPED" << std::endl;
    }
    state_ = statePlane::ONGROUND;
}

void Plane::requestlanding(APP* target) {
    if (target->isPlaneInRange(*this)) {
        target->getTwr()->landing(this);
    }
};

void Plane::takeof(TWR* /*spawn*/) {
    // non utilisé pour le moment, la TWR remet directement l'état en TAKEOFF
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

APP* Plane::askAPPForNewTarget() {
    return target_->askForNewTarget();
};

void Plane::setTarget(APP* newTarget) {
    target_ = newTarget;
    Position newTrajectory;
    newTrajectory.x_ = target_->getPos().x_ - pos_.x_;
    newTrajectory.y_ = target_->getPos().y_ - pos_.y_;
    float norme = std::sqrt(newTrajectory.x_ * newTrajectory.x_ + newTrajectory.y_ * newTrajectory.y_);
    if (norme > 0.0f) {
        newTrajectory.x_ /= norme;
        newTrajectory.y_ /= norme;
    }
    trajectory_ = newTrajectory;
}