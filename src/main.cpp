#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "../include/Plane.hpp"

using namespace sf;
constexpr unsigned WINDOW_SIZE_X = 1500, WINDOW_SIZE_Y = 1500;

#ifdef _MSC_VER
// #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#define _PATH_IMG_ "C:/SFML_3.0.2/img/"
#else
#define _PATH_IMG_ "./img/"
#endif

void initWindow(std::vector<APP*>& apps, std::vector<Plane*>& planes) {
    RenderWindow window(VideoMode({ WINDOW_SIZE_X, WINDOW_SIZE_Y }), "Air Traffic Control");
    window.setFramerateLimit(120);

    // Charger la carte
    Texture backgroundImage;
    if (!backgroundImage.loadFromFile(std::string(_PATH_IMG_) + "carte-relief-France.jpg")) {
        std::cerr << "Erreur chargement carte" << std::endl;
        return;
    }
    Sprite backgroundSprite(backgroundImage);
    backgroundSprite.scale({0.9f,0.9f}); 
    // Charger la texture de l'avion
    Texture planeTexture;
    if (!planeTexture.loadFromFile(std::string(_PATH_IMG_) + "plane.png")) {
        std::cerr << "Erreur chargement avion" << std::endl;
        return;
    }

    // Charger la texture de l'APP
    Texture appTexture;
    if (!appTexture.loadFromFile(std::string(_PATH_IMG_) + "airport.png")) {
        std::cerr << "Erreur chargement APP" << std::endl;
        return;
    }

    Texture appRedTexture;
    if (!appRedTexture.loadFromFile(std::string(_PATH_IMG_) + "aairportRed.png")) {
        std::cerr << "Erreur chargement APP" << std::endl;
        return;
    }

    // Créer les sprites pour les APPs
    std::vector<Sprite> appShapes;
    for (auto* appPtr : apps) {
        Sprite appSprite(appTexture);
        appSprite.setScale({ 0.2f, 0.2f });
        Vector2u texSize = appTexture.getSize();
        appSprite.setOrigin({ texSize.x / 2.0f, texSize.y / 2.0f });
        appShapes.push_back(appSprite);
    }

    // Créer les sprites pour tous les avions
    std::vector<Sprite> planeShapes;
    for (auto* planePtr : planes) {
        Sprite planeSprite(planeTexture);
        planeSprite.setScale({ 0.1f, 0.1f }); 
        // Centrer l'origine du sprite
        Vector2u texSize = planeTexture.getSize();
        planeSprite.setOrigin({ texSize.x / 2.0f, texSize.y / 2.0f });
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
            // Vérifier si le parking a de la place et changer la texture
            if (apps[i]->getTwr()->placeInParking()) {
                appShapes[i].setTexture(appTexture);  // Vert = place disponible
            }
            else {
                appShapes[i].setTexture(appRedTexture);  // Rouge = plein
            }

            float appX = apps[i]->getPos().x_;
            float appY = apps[i]->getPos().y_;
            appShapes[i].setPosition({ appX, appY });
            window.draw(appShapes[i]);
        }

        // Mettre à jour et dessiner tous les avions
        for (size_t i = 0; i < planeShapes.size(); ++i) {
            if (planes[i]->getTarget()->getTwr()->isParked(*planes[i])) {
                continue;  // Passer à l'avion suivant sans le dessiner
            }
            float planeX = planes[i]->fgetpos().x_;
            float planeY = planes[i]->fgetpos().y_;
            planeShapes[i].setPosition({ planeX, planeY });
            
            Position direction = planes[i]->getTrajectory();
            float angle = std::atan2(direction.y_, direction.x_) * 180.f / 3.14159265f;
            planeShapes[i].setRotation(sf::degrees(angle + 90.f));
            window.draw(planeShapes[i]);
        }

        window.display();
    }
}

int main(void) {
    srand(time(NULL));
    std::mutex mtx;
    std::cout << "Mutex created" << std::endl;

    CCR CCR("GLOBAL", mtx);
    std::cout << "CCR CREATED" << std::endl;

    TWR twrBordeaux("BORDEAUX", 2, mtx, 360.0f, 855.0f);  
    TWR twrParis("PARIS", 2, mtx, 675.0f, 315.0f);  
    TWR twrMarseille("MARSEILLE", 2, mtx, 900.0f, 1013.0f);
    TWR twrLille("LILLE", 2, mtx, 729.0f, 108.0f);
    TWR twrBonifacio("BONIFACIO", 2, mtx, 1300.0f, 1240.0f); 
    std::cout << "TWRs created" << std::endl;

    APP appBordeaux("APP_BORDEAUX", 60.0f, &twrBordeaux, &CCR, mtx);
    APP appParis("APP_PARIS", 60.0f, &twrParis, &CCR, mtx);
	APP appMarseille("APP_MARSEILLE", 60.0f, &twrMarseille, &CCR, mtx);
	APP appLille("APP_LILLE", 60.0f, &twrLille, &CCR, mtx);
	APP appBonifacio("APP_BONIFACIO", 60.0f, &twrBonifacio, &CCR, mtx);
    std::cout << "APPs created" << std::endl;

    Plane planeAFR10("AFR10", 35, &appBordeaux, &twrMarseille, mtx);
    Plane planeAFR50("AFR50", 35, &appParis, &twrBordeaux, mtx);
    Plane planeA380("AIM90", 35, &appBordeaux, &twrParis, mtx);
	Plane planeDLH20("DLH20", 35, &appBonifacio, &twrLille, mtx);
    Plane planeAZE20("AZE20", 35, &appBordeaux, &twrLille, mtx);
    std::cout << "Planes created" << std::endl;

    CCR.addAPP(appBordeaux);
    CCR.addAPP(appParis);
	CCR.addAPP(appMarseille);
    CCR.addAPP(appLille);
    CCR.addAPP(appBonifacio); 
    CCR.addPlane(planeAFR10);
    CCR.addPlane(planeAFR50);
    CCR.addPlane(planeA380);
	CCR.addPlane(planeAZE20);
    
    std::cout << "CCR configured" << std::endl;

    twrBordeaux.start();
    twrParis.start();
	twrMarseille.start();
    twrLille.start();
    twrBonifacio.start(); 
    appBordeaux.start();
    appParis.start();
	appMarseille.start();
    appLille.start();
    appBonifacio.start();
    CCR.start();
    planeAFR10.start();
    planeAFR50.start();
    planeA380.start();
    planeDLH20.start();
    planeAZE20.start(); 

    std::vector<APP*> apps = { &appBordeaux, &appParis, &appMarseille, &appLille, &appBonifacio};
    std::vector<Plane*> planes = { &planeAFR10, &planeAFR50, &planeA380, &planeDLH20, &planeAZE20 };

    initWindow(apps, planes);

	planeAFR10.stop();
	planeAFR50.stop();
	planeA380.stop();
    planeDLH20.stop();
    return 0;
}