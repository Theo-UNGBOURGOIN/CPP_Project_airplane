#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include "Agent.hpp"
#include "APP.hpp"
#include "TWR.hpp"
#include "CCR.hpp"

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using namespace std::chrono_literals;

struct Position; 
enum struct statePlane {
    FLYING,
    LANDING,
    TAKEOFF,
    ONGROUND,
    HOLDING,
    AVOIDING
};

class Plane;
class APP; 
class TWR; 
class Journal;

class Plane : public Agent {
private:
    // Journal *journal_; 
    /*APP* app_ = nullptr; 
    TWR* twr_ = nullptr;*/
    Position pos_; 
    Position trajectory_;
    statePlane state_ = statePlane::ONGROUND;
    std::string name;  
    // float radius_ = 0;       A vérifier si les avions tournent en rond ou pas 
    float speed_ = 0;
    // float fuel = 0;   a ajouter plus tard
    // float conso = 0;  
    APP* target_; 
    // high_resolution_clock::time_point initial_clock_;
public:
    Plane(const std::string& name, const float& speed, APP* target, TWR* spawn, std::mutex& mtx);
    void run() override;
	void requestlanding(APP* target); 
    Position fgetpos(); 
    Position getTrajectory(); 
    void setState(statePlane newState) { state_ = newState; }
    statePlane getState();
	APP* getTarget() { return target_; }
    void setTarget(APP* newTarget);
    void updateTrajectory();
    // void resetPosition(float x, float y, float altitude);
    bool checkCollision(Plane& other, float minDistance);
    // ~Plane();
	APP* askAPPForNewTarget();
};