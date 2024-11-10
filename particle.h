// Particle.h
#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>
#include <cmath>

// Define the Particle structure
struct Particle
{
    float x = 0;
    float y = 0;
    float vx = 0;
    float vy = 0;
    float size = 0;
    float index;
    sf::Color color;

    // Constructor
    Particle(float x = 0, float y = 0, float vx = 0, float vy = 0, float size = 0, sf::Color color = sf::Color::White);

    void drawParticle(sf::RenderWindow& window, sf::CircleShape shape){
        color = sf::Color(100-vx * 10, 100-vy * 10, 255);
        shape.setPosition(x, y);
        shape.setScale(size, size);
        shape.setFillColor(color);
        window.draw(shape);

    }
    void setIndex(int i){
        index = i;
    }
    bool empty() {
    return size ==0;

    
}

    
    void wallCollisions(float windowWidth, float windowHeight, float wallDamping)
    {
        // collisions
        if (y + size > windowHeight)
        {
            vy *= -wallDamping;      // Reverse and dampen the vertical velocity
            y = windowHeight - size; // Prevent sinking below the boundary
        }
        if (x + size > windowWidth)
        {
            vx *= -wallDamping;     // Reverse and dampen the vertical velocity
            x = windowWidth - size; // Prevent sinking below the boundary
        }
        if (y - size < 0)
        {
            vy *= -wallDamping; // Reverse and dampen the vertical velocity
            y = 0 + size;       // Prevent sinking below the boundary
        }

        if (x - size < 0)
        {
            vx *= -wallDamping; // Reverse and dampen the vertical velocity
            x = 0 + size;       // Prevent sinking below the boundary
        }
    }
};

// Declare functions
sf::Vector2f getDirection(sf::Vector2f pointA, sf::Vector2f pointB);
float distance(float x1, float y1, float x2, float y2);
bool check_collision(const Particle &a, const Particle &b);
void solveCollisions(std::vector<Particle*>& particles);
void applyGravity(std::vector<Particle> &particles, float gravity);
void  applyGiregionatGravity(std::vector<Particle>& particles, float gravity, float width, float height, float force);

#endif // PARTICLE_H
