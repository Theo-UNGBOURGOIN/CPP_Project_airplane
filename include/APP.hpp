#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include "Plane.hpp"

struct Position {
    float x_ = 0.;
    float y_ = 0.;
    float altitude_ = 0;
};

class Plane;
class APP;
class TWR;
class Journal;


class APP : public Agent {
private:
    Position pos_;
    // Journal* journal; 
    std::vector<Plane*> planeInRange_; 
    TWR* twr_; 
    const float range_;  
public: 
    APP(const std::string name, Position pos, const float& range, TWR* twr);
    void run() override;
    Position getPos();
    void receivePlane(Plane* plane);    // appelé par CCR pour handover l'avion si il est dans la range et si c'est le bon aéropoprt 
    ~APP();
    
};