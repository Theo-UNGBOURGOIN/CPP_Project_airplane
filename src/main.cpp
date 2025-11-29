#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "../include/Plane.hpp"

using namespace sf;
constexpr unsigned WINDOW_SIZE_X = 1000, WINDOW_SIZE_Y = 800;

 #ifdef _MSC_VER
 #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
 #define _PATH_IMG_ "C:/SFML_3.0.2/img/"
 #else
 // #define _PATH_IMG_ "../img/"
 #define _PATH_IMG_ "./img/"
 #endif
 const std::string path_image(PATH_IMG);

void initWindow(std::vector<APP*>& apps, std::vector<Plane*>& planes) {
    RenderWindow window(VideoMode({ WINDOW_SIZE_X, WINDOW_SIZE_Y }), "Air Traffic Control");
    window.setFramerateLimit(60);

    // Charger la carte
    Texture backgroundImage;
    if (!backgroundImage.loadFromFile(std::string(_PATH_IMG_) + "europe2.png")) {
        std::cerr << "Erreur chargement carte" << std::endl;
        return;
    }
    Sprite backgroundSprite(backgroundImage);

    // Charger la texture de l'avion
    Texture planeTexture;
    if (!planeTexture.loadFromFile(std::string(_PATH_IMG_)+"plane.png")) {
        std::cerr << "Erreur chargement avion" << std::endl;
        return;
    }

    // Charger la texture de l'APP
    Texture appTexture;
    if (!appTexture.loadFromFile(std::string(_PATH_IMG_)+"airport.png")) {
        std::cerr << "Erreur chargement APP" << std::endl;
        return;
    }

    // Créer les sprites pour les APPs
    std::vector<Sprite> appShapes;
    for (auto* appPtr : apps) {
        Sprite appSprite(appTexture);
        appSprite.setScale({ 0.1f, 0.1f });
        appShapes.push_back(appSprite);
    }

    // Créer les sprites pour tous les avions
    std::vector<Sprite> planeShapes;
    for (auto* planePtr : planes) {
        Sprite planeSprite(planeTexture);
        planeSprite.setScale({ 0.1f, 0.1f });

        Position direction = planePtr->getTrajectory();  
        float angle = std::atan2(direction.y_, direction.x_) * 180.f / 3.14159265f;
        planeSprite.setRotation(sf::degrees(angle + 90.f)); 
        planeShapes.push_back(planeSprite);
    }

    while (window.isOpen()) {
        while (const std::optional<Event> event = window.pollEvent()) {
            if ((event->is<sf::Event::KeyPressed>() &&
                event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape) ||
                event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(backgroundSprite);

        // Dessiner les APPs 
        for (size_t i = 0; i < appShapes.size(); ++i) {
            appShapes[i].setPosition({ apps[i]->getPos().x_, apps[i]->getPos().y_ });
            window.draw(appShapes[i]);
        }

        // Mettre à jour et dessiner tous les avions
        for (size_t i = 0; i < planeShapes.size(); ++i) {
            planeShapes[i].setPosition({ planes[i]->fgetpos().x_, planes[i]->fgetpos().y_ });
            // ici mettre la bonne rotation de l'avion dirigé vers l'app avec setrotation je crois

            window.draw(planeShapes[i]);
        }

        window.display();
    }
}

int main(void) {
    std::mutex mtx;
    std::cout << "Mutex created" << std::endl;

    CCR CCR("GLOBAL", mtx);
    std::cout << "CCR CREATED" << std::endl;

    TWR twrLille("LILLE", 10, mtx, 1000, 1000);
    TWR twrParis("PARIS", 10, mtx, 2000, 2000);
    std::cout << "TWRs created" << std::endl;

    APP appLille("APP1", 50.0f, &twrLille, mtx);
    APP appParis("APP2", 50.0f, &twrParis, mtx);
    std::cout << "APPs created" << std::endl;

    Plane planeAFR10("AFR10", 50, &appParis, &twrLille, mtx);
    Plane planeAFR50("AFR50", 20, &appParis, &twrLille, mtx);
    std::cout << "Plane created" << std::endl;

    CCR.addAPP(appLille);
    CCR.addAPP(appParis);
    CCR.addPlane(planeAFR10);
    CCR.addPlane(planeAFR50);
    std::cout << "CCR configured" << std::endl;


    twrLille.start();
    twrParis.start();
    appLille.start();
    appParis.start();
    CCR.start();
    planeAFR10.start();
    planeAFR50.start();
    std::cout << "All threads started" << std::endl;

    std::vector<APP*> apps = { &appLille, &appParis };
    std::vector<Plane*> planes = { &planeAFR10, &planeAFR50 };
    initWindow(apps, planes);

    return 0;
}