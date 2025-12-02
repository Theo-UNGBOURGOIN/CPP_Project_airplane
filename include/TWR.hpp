#pragma once 
#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <map>
#include "Plane.hpp"
#include "Agent.hpp"


struct Position;

class TWR : public Agent {

private:
    std::queue<std::string> queuelanding;
    std::queue<std::string> queuetakeoff;
    // bool iswayfree;
    Position pos_;
    //Journal* Journal; 
    std::vector<Plane*> Parking;
    const int parkingSize_ = 2; 

public: 
	TWR(const std::string name, const int parkingSize, std::mutex& mtx, const int x, const int y); 
    void run() override; 
    void landing(Plane* plane);
    Position twrGetPos();
    std::vector<Plane*>& getParking();
    bool isParked(Plane& plane); 
	bool placeInParking();
};