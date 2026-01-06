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

enum class ViewMode {
    FRANCE,
    AIRPORT_VIEW
};

struct ViewTransform {
    float zoomFactor;
    float offsetX;
    float offsetY;

    ViewTransform() : zoomFactor(1.0f), offsetX(0.0f), offsetY(0.0f) {}

    // Transformer les coordonnées du monde vers l'écran
    Vector2f worldToScreen(float worldX, float worldY) const {
        return Vector2f(
            (worldX - offsetX) * zoomFactor + WINDOW_SIZE_X / 2.0f,
            (worldY - offsetY) * zoomFactor + WINDOW_SIZE_Y / 2.0f
        );
    }
};

void drawGlobalView(RenderWindow& window, const Sprite& backgroundSprite, std::vector<Sprite>& appShapes, std::vector<Sprite>& planeShapes, std::vector<APP*>& apps, std::vector<Plane*>& planes, const Texture& appTexture, const Texture& appRedTexture) {
    window.draw(backgroundSprite);

    // Dessiner les APPs
    for (size_t i = 0; i < appShapes.size(); ++i) {
        if (apps[i]->getTwr()->placeInParking()) {
            appShapes[i].setTexture(appTexture);
        }
        else {
            appShapes[i].setTexture(appRedTexture);
        }

        float appX = apps[i]->getPos().x_;
        float appY = apps[i]->getPos().y_;
        appShapes[i].setPosition({ appX, appY });
        window.draw(appShapes[i]);
    }

    // Dessiner les avions
    for (size_t i = 0; i < planeShapes.size(); ++i) {
        if (planes[i]->getTarget()->getTwr()->isParked(*planes[i])) {
            continue;
        }
        float planeX = planes[i]->fgetpos().x_;
        float planeY = planes[i]->fgetpos().y_;
        planeShapes[i].setPosition({ planeX, planeY });

        Position direction = planes[i]->getTrajectory();
        float angle = std::atan2(direction.y_, direction.x_) * 180.f / 3.14159265f;
        planeShapes[i].setRotation(sf::degrees(angle + 90.f));
        window.draw(planeShapes[i]);
    }
}

void drawAirportView(RenderWindow& window, const Sprite& airportBackground, std::vector<Sprite>& planeShapes,
    std::vector<Plane*>& planes, APP* selectedAirport, const ViewTransform& transform) {
    window.draw(airportBackground);

    // Position de l'aéroport dans le monde
    float airportWorldX = selectedAirport->getPos().x_;
    float airportWorldY = selectedAirport->getPos().y_;
    float appRange = selectedAirport->getRadius();

    // Dessiner un cercle pour visualiser le radius de l'aéroport
    CircleShape rangeCircle(appRange * transform.zoomFactor);
    rangeCircle.setOrigin({ appRange * transform.zoomFactor, appRange * transform.zoomFactor });
    rangeCircle.setPosition({ WINDOW_SIZE_X / 2.0f, WINDOW_SIZE_Y / 2.0f });
    rangeCircle.setFillColor(Color::Transparent);
    rangeCircle.setOutlineColor(Color(100, 100, 255, 100));
    rangeCircle.setOutlineThickness(4.0f);
    window.draw(rangeCircle);

    // Dessiner l'aéroport au centre
    CircleShape airportMarker(15.0f);
    airportMarker.setOrigin({ 15.0f, 15.0f });
    airportMarker.setPosition({ WINDOW_SIZE_X / 2.0f, WINDOW_SIZE_Y / 2.0f });
    airportMarker.setFillColor(Color(255, 100, 100));
    airportMarker.setOutlineColor(Color::White);
    airportMarker.setOutlineThickness(2.0f);
    window.draw(airportMarker);

    // Afficher uniquement les avions dans le range de l'APP 
    for (size_t i = 0; i < planes.size(); ++i) {
        if (planes[i]->getTarget()->getTwr()->isParked(*planes[i])) {
            continue;
        }

        float planeWorldX = planes[i]->fgetpos().x_;
        float planeWorldY = planes[i]->fgetpos().y_;

        // Calculer la distance entre l'avion et l'aéroport
        float dx = planeWorldX - airportWorldX;
        float dy = planeWorldY - airportWorldY;
        float distance = std::sqrt(dx * dx + dy * dy);

        // Afficher seulement si dans le range 
        if (distance <= appRange * 2) {
            // Transformer les coordonnées monde vers écran
            Vector2f screenPos = transform.worldToScreen(planeWorldX, planeWorldY);
            planeShapes[i].setPosition(screenPos);
            Position direction = planes[i]->getTrajectory();
            float angle = std::atan2(direction.y_, direction.x_) * 180.f / 3.14159265f;
            planeShapes[i].setRotation(sf::degrees(angle + 90.f));
            planeShapes[i].setScale({ 0.2f, 0.2f });
            window.draw(planeShapes[i]);
        }
    }
}

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
    backgroundSprite.scale({ 0.9f, 0.9f });

    // Charger la texture du zoom aeroport
    Texture airportBackgroundTexture;
    if (!airportBackgroundTexture.loadFromFile(std::string(_PATH_IMG_) + "airport_view.png")) {
        std::cerr << "Erreur chargement zoom aeroport" << std::endl;
    }
    Sprite airportBackgroundSprite(airportBackgroundTexture);
    Vector2u texSize = airportBackgroundTexture.getSize();
    if (texSize.x > 0 && texSize.y > 0) {
        float scaleX = static_cast<float>(WINDOW_SIZE_X) / texSize.x;
        float scaleY = static_cast<float>(WINDOW_SIZE_Y) / texSize.y;
        airportBackgroundSprite.setScale({ scaleX, scaleY });
    }

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
        std::cerr << "Erreur chargement APP rouge" << std::endl;
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

    ViewMode currentView = ViewMode::FRANCE;
    APP* selectedAirport = nullptr;
    ViewTransform viewTransform;

    while (window.isOpen()) {
        while (const std::optional<Event> event = window.pollEvent()) {
            if (event->is<sf::Event::KeyPressed>()) {
                auto keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    if (currentView == ViewMode::FRANCE) {
                        window.close();
                    }
                    else if (currentView == ViewMode::AIRPORT_VIEW) {
                        currentView = ViewMode::FRANCE;
                        selectedAirport = nullptr;

                        // Réinitialiser l'échelle des avions
                        for (auto& sprite : planeShapes) {
                            sprite.setScale({ 0.1f, 0.1f });
                        }

                    }
                }
            }
            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    Vector2i mousePos = mouseEvent->position;

                    if (currentView == ViewMode::FRANCE) {
                        // Vérifier si on a cliqué sur un aéroport
                        for (size_t i = 0; i < apps.size(); ++i) {
                            FloatRect appBounds = appShapes[i].getGlobalBounds();

                            if (appBounds.contains(static_cast<Vector2f>(mousePos))) {
                                currentView = ViewMode::AIRPORT_VIEW;
                                selectedAirport = apps[i];

                                // Calculer le zoom factor pour que le radius occupe ~80% de la fenêtre
                                float appRange = selectedAirport->getRadius();
                                float targetSize = std::min(WINDOW_SIZE_X, WINDOW_SIZE_Y) * 0.8f;
                                viewTransform.zoomFactor = targetSize / (2.0f * appRange);

                                // Centrer sur l'aéroport
                                viewTransform.offsetX = selectedAirport->getPos().x_;
                                viewTransform.offsetY = selectedAirport->getPos().y_;

                                break;
                            }
                        }
                    }
                }
            }
        }

        window.clear(Color::White);

        if (currentView == ViewMode::FRANCE) {
            drawGlobalView(window, backgroundSprite, appShapes, planeShapes, apps, planes, appTexture, appRedTexture);
        }
        else if (currentView == ViewMode::AIRPORT_VIEW && selectedAirport != nullptr) {
            drawAirportView(window, airportBackgroundSprite, planeShapes, planes, selectedAirport, viewTransform);
        }

        window.display();
    }
}

int main(void) {
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

    Plane planeAFR10("AFR10", 30, &appBordeaux, &twrMarseille, mtx);
    Plane planeAFR50("AFR50", 40, &appParis, &twrBordeaux, mtx);
    Plane planeA380("AIM90", 32, &appBordeaux, &twrParis, mtx);
    Plane planeDLH20("DLH20", 33, &appBonifacio, &twrLille, mtx);
    Plane planeAZE20("AZE20", 41, &appBordeaux, &twrLille, mtx);
    Plane planeUAL30("UAL30", 37, &appBordeaux, &twrBonifacio, mtx);
    Plane planeSWR40("SWR40", 38, &appLille, &twrParis, mtx);
    std::cout << "Planes created" << std::endl;

    CCR.addAPP(appParis);
    CCR.addAPP(appBordeaux);
    CCR.addAPP(appMarseille);
    CCR.addAPP(appLille);
    CCR.addAPP(appBonifacio);
    CCR.addPlane(planeAFR10);
    CCR.addPlane(planeAFR50);
    CCR.addPlane(planeA380);
    CCR.addPlane(planeAZE20);
    CCR.addPlane(planeDLH20);
    CCR.addPlane(planeUAL30);
    CCR.addPlane(planeSWR40);

    std::cout << "CCR configured" << std::endl;

    twrParis.start();
    twrBordeaux.start();
    twrMarseille.start();
    twrLille.start();
    twrBonifacio.start();
    appParis.start();
    appBordeaux.start();
    appMarseille.start();
    appLille.start();
    appBonifacio.start();
    CCR.start();
    planeAFR10.start();
    planeAFR50.start();
    planeA380.start();
    planeDLH20.start();
    planeAZE20.start();
    planeUAL30.start();
    planeSWR40.start();

    std::vector<APP*> apps = { &appParis, &appBordeaux, &appMarseille, &appLille, &appBonifacio };
    std::vector<Plane*> planes = { &planeAFR10, &planeAFR50, &planeA380, &planeDLH20, &planeAZE20, &planeSWR40, &planeUAL30 };

    initWindow(apps, planes);

    planeAFR10.stop();
    planeAFR50.stop();
    planeA380.stop();
    planeDLH20.stop();
    planeAZE20.stop();
    planeUAL30.stop();
    planeSWR40.stop();
    return 0;
}