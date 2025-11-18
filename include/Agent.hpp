#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <mutex>

class Agent {
public: 
	Agent(const std::string& name);
	virtual ~Agent(); 
	void start();       // lance le thread
	void stop();        // arrête la boucle
	bool isRunning() const;

	virtual void run() = 0;  // boucle interne des agents

	const std::string& getName() const;

protected:
	std::string name_;
	std::thread thread_;
	bool running_ = false;
	std::mutex mtx_; 
};
