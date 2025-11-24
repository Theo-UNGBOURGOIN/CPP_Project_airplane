#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <mutex>

// #include <atomic>
// #include <queue>

struct Position {
	float x_ = 0.f;
	float y_ = 0.f;
	float altitude_ = 0.f;
};

class Agent {
public: 
	Agent(const std::string& name, std::mutex& mtx);
	virtual ~Agent(); 
	void start();       // lance le thread
	void stop();        // arrête la boucle
	bool isRunning() const;

	virtual void run() = 0;  // boucle interne des agents

	// void envoyerMessage(const std::string& msg);
	// bool recevoirMessage(std::string& msg);

	const std::string& getName() const;

protected:
	std::string name_;
	std::thread thread_;
	std::atomic<bool> running_;
	std::mutex mtx_; 
};
