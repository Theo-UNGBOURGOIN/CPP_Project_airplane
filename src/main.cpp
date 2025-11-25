#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "../include/Plane.hpp"


// #ifdef _MSC_VER
// #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
// #define _PATH_IMG_ "C:/Program Files/SFML/img/"
// #else
// // #define _PATH_IMG_ "../img/"
// #define _PATH_IMG_ "./img/"
// #endif
// const std::string path_image(PATH_IMG);

int main(void) {
    std::mutex mtx; 
    std::cout << "Mutex created" << std::endl;
    /*----------------------------------------*/
    CCR CCR("GLOBAL", mtx); 
    std::cout << "CCR CREATED" << std::endl; 
    /*----------------------------------------*/
    TWR twrLille("LILLE", 10, mtx, 1000, 1000);
	TWR twrParis("PARIS", 10, mtx, 0, 0);
    std::cout << "TWRs created" << std::endl;
    /*----------------------------------------*/
    APP appLille("APP1", 50.0f, &twrLille, mtx);
    APP appParis("APP2", 50.0f, &twrParis, mtx);
    std::cout << "APPs created" << std::endl;
    /*----------------------------------------*/
    Plane plane("Plane1", 50, &appParis, &twrLille, mtx);
    std::cout << "Plane created" << std::endl;
    /*----------------------------------------*/

	CCR.addAPP(appLille);
	CCR.addAPP(appParis);
	CCR.addPlane(plane);
	std::cout << "CCR configured" << std::endl;

	twrLille.start();
	twrParis.start();
	appLille.start();
	appParis.start();
	CCR.start();
	plane.start();
    std::cout << "All threads started" << std::endl;


    while (true) {
        std::cout << "Pos Plane1: " << plane.fgetpos().x_ << " " << plane.fgetpos().y_ << " " << plane.fgetpos().altitude_ << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
 }