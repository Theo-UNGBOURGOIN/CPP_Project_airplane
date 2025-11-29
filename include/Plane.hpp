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
	void takeof(TWR* spawn);
    Position fgetpos(); 
    Position getTrajectory(); 
    // ~Plane();
};