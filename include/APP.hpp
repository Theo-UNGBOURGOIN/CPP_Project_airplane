#pragma once
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include "Plane.hpp"
#include "Agent.hpp"

class Plane;
class APP;
class TWR;
class CCR;
class Journal;
struct Position;

class APP : public Agent {
private:
	Position pos_;
    // Journal* journal; 
    std::vector<Plane*> planeInRange_; 
    TWR* twr_;
    const float range_;
    CCR* ccr_; 
public: 
    APP(const std::string name, const float& range, TWR* twr, CCR* ccr, std::mutex& mtx);
    void run() override;
	float getRadius();
    Position getPos();
    bool isPlaneInRange(Plane& plane);
    void addPlane(Plane* plane); 
    void delPlane(Plane* plane); 
    TWR* getTwr();
	std::vector<Plane*>& getWhosInRange();
    APP* askForNewTarget(); 
};