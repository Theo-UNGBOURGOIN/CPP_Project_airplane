#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include "Agent.hpp"

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using namespace std::chrono_literals;

struct Position {
	float x_ = 0.;
	float y_ = 0.;
	float altitude_ = 0;
};

class Plane;
class APP; 
class TWR; 
class Journal;

void update_plane(Plane& plane, bool& stop_thread);

class Plane : public Agent {
	Journal *journal_; 
	APP* app_ = nullptr; 
	TWR* twr_ = nullptr; 
	Position pos_; 
	Position trajectory_;

	// float radius_ = 0;       A vérifier si les avions tournent en rond ou pas 
	float speed_ = 0;
	/*float fuel = 0;
	float conso = 0;*/  // TDDO: A FAIRE PLUS TARD QUAND AJOUT DE CARBURANT

	// high_resolution_clock::time_point initial_clock_;
public:
	Plane(Position pos, const float& speed, const float& fuel, const& target);
	void run() override; 
	// request landing 
	// take of 
	void updatePos();
	// set APP et tower
	Position fgetpos(); 

	~Plane();
};