#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include "Plane.hpp"

class TWR {
    std::queue<std::string> fileAtterrissage;
    std::queue<std::string> fileDecollage;
    bool pisteLibre;
    std::mutex mtx;
};