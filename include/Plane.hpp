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

class Plane;

void update_plane(Plane& plane, bool& stop_thread);

class Plane {
	std::thread t_;
	std::string nom;
	float x_ = 0.;
	float y_ = 0.;
	float trajectoire_ = 0;
	// float radius_ = 0;       A vérifier si les avions tournent en rond ou pas 
	float speed_ = 0;
	float altitude_ = 0.;
	float carburant = 0;
	float consommation = 0;
	bool stop_thread_ = false;
	high_resolution_clock::time_point initial_clock_;
public:
	Plane(const float& angle, const float& altitude, const float& speed);

	void update();
	float get_x();
	float get_y();
	float get_traj();

	~Plane();
};