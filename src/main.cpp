#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "../include/Plane.hpp"

#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#define _PATH_IMG_ "C:/Program Files/SFML/img/"
#else
// #define _PATH_IMG_ "../img/"
#define _PATH_IMG_ "./img/"
#endif

const std::string path_image(PATH_IMG);

int main(void) {
   /* std::mutex mtx;
    TWR twr1("TWR_1", 10, mtx);
    APP app1("APP_1", 100.0f, &twr1, mtx);
    Plane plane1("Plane_1", 250.0f, &app1, &twr1, mtx);
    plane1.start();
    app1.start();
    twr1.start();
    plane1.stop();*/
    return 0;
}