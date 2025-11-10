#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using namespace std::chrono_literals;

class Plane {
protected:
	std::string nom;
	float x_ = 0.;
	float y_ = 0.;
	float angle_ = 0;
	// float radius_ = 0;       A vérifier si les avions tournent en rond ou pas 
	float speed_ = 0;
	float altitude_ = 0.;
public:
	Plane();

	float get_x();
	float get_y();
	float get_angle();

	~Plane();
};