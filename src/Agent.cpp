#include "../include/Agent.hpp"

Agent::Agent(const std::string& name, std::mutex& mtx) : name_(name) {
	running_ = false;
}

Agent::~Agent() {
	stop();  
	if (thread_.joinable()) {
		thread_.join();
	}
}

void Agent::start() {
	if (running_) return; 
	if (thread_.joinable()) {
		thread_.join();
	}
	running_ = true;
	thread_ = std::thread([this] {this->run();});
}

void Agent::stop() {
	running_ = false; 
}

bool Agent::isRunning() const {
	return running_;
}

const std::string& Agent::getName() const {
	return name_;
}