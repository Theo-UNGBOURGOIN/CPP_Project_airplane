#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include "Agent.hpp"
#include "APP.hpp"

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
	

	// high_resolution_clock::time_point initial_clock_;
public:
	Plane(const std::string& name, const float& speed, APP* target, TWR* spawn);
	void run() override; 
	// request landing 
	// take of ? 
	Position fgetpos(); 
	~Plane();
};