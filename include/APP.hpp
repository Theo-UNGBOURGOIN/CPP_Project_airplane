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
class Journal;
struct Position;

class APP : public Agent {
private:
	Position pos_;
    // Journal* journal; 
    std::vector<Plane*> planeInRange_; 
    TWR* twr_; 
    const float range_;  
public: 
    APP(const std::string name, const float& range, TWR* twr, std::mutex& mtx);
    void run() override;
    Position getPos();
    void receivePlane(Plane* plane);    // appelé par CCR pour handover l'avion si il est dans la range et si c'est le bon aéropoprt 
    
    
};