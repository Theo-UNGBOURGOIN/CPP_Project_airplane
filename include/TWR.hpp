#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <map>
#include "Plane.hpp"


struct Position {
    float x_ = 0.;
    float y_ = 0.;
    float altitude_ = 0;
};

class TWR : public Agent {
    /*std::queue<std::string> queuelanding;
    std::queue<std::string> queuetakeoff;
    bool iswayfree;*/
    Position pos_;
    //Journal* Journal; 
    std::vector<Plane*> Parking; 
    const int parkingSize_; 

public: 
    TWR(const std::string name, const int parkingSize);
    void run() override; 
    Position twrGetPos();
    ~TWR(); 
};