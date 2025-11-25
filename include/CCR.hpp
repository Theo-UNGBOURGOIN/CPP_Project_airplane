#pragma once 
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include "Plane.hpp"
#include "Agent.hpp"

class CCR : public Agent {
private:
	std::vector<APP*> app_;
	std::vector<TWR*> twr_;
	std::vector<Plane*> plane_; 
public:
	CCR(const std::string& name, std::mutex& mtx);
	void run() override;
	void handoverToAPP(APP* app, Plane* plane);
	void addPlane(Plane& plane); 
	void addAPP(APP& app); 
};

//run handover add app add plane