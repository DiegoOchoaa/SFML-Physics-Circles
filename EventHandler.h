#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "particle.h"

class EventHandler {
public:
    bool pressed = false;
    bool right_pressed = false;
    bool spacePressed = false;

    void handleEvents(sf::RenderWindow& window, std::vector<Particle>& particles, float particle_size, float width, float height);
};

#endif // EVENT_HANDLER_H
