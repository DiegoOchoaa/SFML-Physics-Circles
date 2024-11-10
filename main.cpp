#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "particle.h"
#include "EventHandler.h" // Include the new EventHandler class
#include <thread>
#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <iostream>

class Grid
{
public:
    Grid(float width, float height, float cellsize)
        : width(width), height(height), cellsize(cellsize * 2)
    {
        gridWidth = static_cast<int>(std::ceil(width / cellsize));
        gridHeight = static_cast<int>(std::ceil(height / cellsize));
    }

    void drawGrid(sf::RenderWindow &window)
    {
        sf::VertexArray grid(sf::Lines);

        // Draw vertical grid lines
        for (int i = 0; i <= gridWidth; ++i)
        {
            float x = i * cellsize;
            grid.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color(255, 255, 255, 30)));
            grid.append(sf::Vertex(sf::Vector2f(x, gridHeight * cellsize), sf::Color(255, 255, 255, 30)));
        }

        // Draw horizontal grid lines
        for (int i = 0; i <= gridHeight; ++i)
        {
            float y = i * cellsize;
            grid.append(sf::Vertex(sf::Vector2f(0, y), sf::Color(255, 255, 255, 30)));
            grid.append(sf::Vertex(sf::Vector2f(width, y), sf::Color(255, 255, 255, 30)));
        }

        window.draw(grid);
    }

    void sortParticles(std::vector<Particle> &particles)
    {
        treeGrid.resize(gridWidth);
        for (int i = 0; i < gridWidth; ++i)
        {
            treeGrid[i].resize(gridHeight);
        }

        for (int i = 0; i < gridWidth; ++i)
        {
            for (int j = 0; j < gridHeight; ++j)
            {
                treeGrid[i][j].clear();
            }
        }

        for (int i = 0; i < particles.size(); ++i)
        {
            int gridx = particles[i].x / cellsize;
            int gridy = particles[i].y / cellsize;

            if (gridx >= 0 && gridx < gridWidth && gridy >= 0 && gridy < gridHeight)
            {
                treeGrid[gridx][gridy].push_back(&particles[i]);
            }
        }
    }

    void ResolveCollisionsInQuadrant(int startX, int endX, int startY, int endY, std::vector<Particle> &particles, float dampening)
    {
        for (int i = 0; i < particles.size(); ++i)
        {
            particles[i].wallCollisions(width, height, dampening);
        }
        for (int x = startX; x < endX; ++x)
        {
            for (int y = startY; y < endY; ++y)
            {
                std::vector<Particle *> nearbyParticles;

                // Check the current cell and its 8 neighboring cells
                for (int dx = -1; dx <= 1; ++dx)
                {
                    for (int dy = -1; dy <= 1; ++dy)
                    {
                        int neighborX = x + dx;
                        int neighborY = y + dy;

                        if (neighborX >= 0 && neighborX < gridWidth && neighborY >= 0 && neighborY < gridHeight)
                        {
                            // Access the vector of particles in the neighboring cell
                            std::vector<Particle *> &neighborParticles = treeGrid[neighborX][neighborY];
                            // Loop through each particle in the neighboring cell and add it to nearbyParticles
                            for (Particle *p : neighborParticles)
                            {
                                nearbyParticles.push_back(p); // Add each particle to the nearbyParticles vector
                            }
                        }
                    }
                }

                solveCollisions(nearbyParticles);
            }
        }
    }

    void ResolveAllCollisions(std::vector<Particle> &particles, float dampening)
    {
         for (int i = 0; i < 8; ++i){
        // Create threads for each quadrant
        std::thread topLeftThread(&Grid::ResolveCollisionsInQuadrant, this, 0, gridWidth / 2, 0, gridHeight / 2, std::ref(particles), dampening);
        std::thread topRightThread(&Grid::ResolveCollisionsInQuadrant, this, gridWidth / 2, gridWidth, 0, gridHeight / 2, std::ref(particles), dampening);
        std::thread bottomLeftThread(&Grid::ResolveCollisionsInQuadrant, this, 0, gridWidth / 2, gridHeight / 2, gridHeight, std::ref(particles), dampening);
        std::thread bottomRightThread(&Grid::ResolveCollisionsInQuadrant, this, gridWidth / 2, gridWidth, gridHeight / 2, gridHeight, std::ref(particles), dampening);

        // Join threads
        topLeftThread.join();
        topRightThread.join();
        bottomLeftThread.join();
        bottomRightThread.join();
         }
         
    }

private:
    float width, height, cellsize;
    int gridWidth, gridHeight;
    std::vector<std::vector<std::vector<Particle *>>> treeGrid;
};

void resolveParticles(float wall_damping, std::vector<Particle> &particles, Grid &tree)
{
    tree.sortParticles(particles);
    tree.ResolveAllCollisions(particles, wall_damping);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(2560, 1664), "Quad Tree Particles");
    sf::Vector2u windowSize = window.getSize();
    sf::CircleShape shape(1.f);
    sf::Clock clock;               // Start a clock to measure time elapsed for FPS
    window.setFramerateLimit(120); // Set the window to run at 60 FPS
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    std::vector<Particle> particles;

    float gravity = 0.7f;
    const float wall_damping = 0.2f;
    int particle_size = 15;
    EventHandler eventHandler;
    Grid Tree(windowSize.x, windowSize.y, particle_size);

    for (int n = 0; n < 500; ++n)
    {
        float size = rand() % particle_size;
        if (size < 5)
        {
            size = 5;
        }
        size = particle_size;
        particles.emplace_back(rand() % windowSize.x, rand() % windowSize.y, 0, 0, size);
    }

    while (window.isOpen())
    {
        eventHandler.handleEvents(window, particles, particle_size, windowSize.x, windowSize.y);
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2u windowSize = window.getSize();

        // applyGiregionatGravity(particles, gravity, windowSize.x, windowSize.y, 1.5f);
        applyGravity(particles, gravity);

        for (int i = 0; i < 1; ++i)
        {
            resolveParticles(wall_damping, particles, Tree);
        }

        window.clear();

        // Tree.drawGrid(window);

        for (int i = 0; i < particles.size(); ++i)
        {
            particles[i].drawParticle(window, shape);
        }

        // std::cout << "  Particles: " << particles.size() << std::endl;

        window.display();
    }

    return 0;
}