#include "EventHandler.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "particle.h"

bool pressed = false;
bool right_pressed = false;
bool spacePressed = false; // Variable to track spacebar press state
bool r_pressed = false;
bool top = false;

void EventHandler::handleEvents(sf::RenderWindow &window, std::vector<Particle> &particles, float particle_size, float width, float height)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space && !spacePressed)
            {
                spacePressed = true;
            }
            if (event.key.code == sf::Keyboard::R)
            {
                r_pressed = true;
            }
            if (event.key.code == sf::Keyboard::Up)
            {
                top = true;
            }
        }
        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::Space)
            {
                spacePressed = false;
            }
            if (event.key.code == sf::Keyboard::R)
            {
                r_pressed = false;
            }
            if (event.key.code == sf::Keyboard::Up)
            {
                top = false;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                pressed = true;
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                right_pressed = true;
            }
        }
        if (event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                pressed = false;
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                right_pressed = false;
            }
        }
    }
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (right_pressed)
    {
        for (int n = 0; n < 5; ++n)
        {
            float s = rand() % int(particle_size);
            if (s < 5)
            {
                s = 5;
            }
            s = particle_size;

            particles.emplace_back(mousePos.x, mousePos.y, 15 - rand() % 30, 15 - rand() % 30, s, sf::Color(100 + rand() % 156, 2, rand() % 21));
        }
    }
    if (true)
    {
        for (int i = 0; i < particles.size(); ++i)
        {
            float dist = distance(mousePos.x, mousePos.y, particles[i].x, particles[i].y);
            const sf::Vector2f dir = sf::Vector2f(mousePos.x, mousePos.y) - sf::Vector2f(particles[i].x, particles[i].y);

            if (dist < 300)
            {
                // Apply smoother force
                float force = 5000.f / (dist * dist + 1.f); // Limit force magnitude using distance
                float maxForce = 1.f;                       // Limit max force to avoid over-pulling
                force = std::min(force, maxForce);
                float forcee = (sqrt(dist)) / 10;

                if (pressed)
                {
                    particles[i].vx += dir.x * 0.01f * forcee; // Reduced multiplier for smoother motion
                    particles[i].vy += dir.y * 0.01f * forcee; // Reduced multiplier for smoother motion
                }
                if (spacePressed)
                {
                    particles[i].vx -= dir.x * force * 0.4f; // Reduced multiplier for smoother motion
                    particles[i].vy -= dir.y * force * 0.4f; // Reduced multiplier for smoother motion
                }
            }
        }
    }
    if (r_pressed)
    {
        particles.clear();
    }
    if (top)
    {
        for (int i = 0; i < particles.size(); ++i)
        {
            float dist = distance(width / 2, height / 2, particles[i].x, particles[i].y);
            const sf::Vector2f dir = sf::Vector2f(width / 2, height / 2) - sf::Vector2f(particles[i].x, particles[i].y);

            particles[i].vx += dir.x * 0.01f * 0.5; // Reduced multiplier for smoother motion
            particles[i].vy += dir.y * 0.01f * 0.5; // Reduced multiplier for smoother motion

            particles[i].x += particles[i].vx;
            particles[i].y += particles[i].vy;
        }
    }
}
