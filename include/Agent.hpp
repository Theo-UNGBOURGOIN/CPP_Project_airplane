// Agent.hpp
#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include "Journal.hpp"

class Agent {
protected:
    std::string id;
    std::thread threadAgent;
    std::atomic<bool> actif;
    Journal* journal;

    std::mutex mtxMessages;
    std::queue<std::string> messages;
    std::condition_variable cvMessages;

public:
    Agent(const std::string& id, Journal* j) : id(id), actif(true), journal(j) {}
    virtual ~Agent() {
        if (threadAgent.joinable()) threadAgent.join();
    }

    virtual void run() = 0; // chaque agent a sa propre logique

    void start() {
        threadAgent = std::thread(&Agent::run, this);
    }

    void stop() { actif = false; }

    void envoyerMessage(const std::string& msg) {
        std::unique_lock<std::mutex> lock(mtxMessages);
        messages.push(msg);
        cvMessages.notify_one();
    }

    bool recevoirMessage(std::string& out) {
        std::unique_lock<std::mutex> lock(mtxMessages);
        if (messages.empty()) return false;
        out = messages.front();
        messages.pop();
        return true;
    }

    std::string getId() const { return id; }
};
