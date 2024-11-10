// Particle.cpp
#include "particle.h"

// Constructor for Particle
Particle::Particle(float x, float y, float vx, float vy, float size, sf::Color color)
    : x(x), y(y), vx(vx), vy(vy), size(size), color(color) {}

// Function definitions
sf::Vector2f getDirection(sf::Vector2f pointA, sf::Vector2f pointB)
{
    sf::Vector2f direction = pointB - pointA;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
    {
        direction /= length;
    }
    return direction;
}

float distance(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

bool check_collision(const Particle &a, const Particle &b)
{
    float dist = distance(a.x, a.y, b.x, b.y);
    return dist <= (a.size + b.size);
}

void applyGravity(std::vector<Particle> &particles, float gravity)
{
    for (int i = 0; i < particles.size(); ++i)
    {
        // Apply gravity to vertical velocity
        particles[i].vy += gravity;

        // Update position based on velocity
        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;
    }
}
void applyGiregionatGravity(std::vector<Particle> &particles, float gravity, float width, float height, float force)
{
    for (int i = 0; i < particles.size(); ++i)
    {
        float dist = distance(width / 2, height / 2, particles[i].x, particles[i].y);
        const sf::Vector2f dir = sf::Vector2f(width / 2, height / 2) - sf::Vector2f(particles[i].x, particles[i].y);

        particles[i].vx -= dir.x * 0.01f * force; // Reduced multiplier for smoother motion
        particles[i].vy -= dir.y * 0.01f * force; // Reduced multiplier for smoother motion


        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;
    }
}

void solveCollisions(std::vector<Particle *> &particles)
{
    const float minMoveThreshold = 0.1f; // Minimum movement required to resolve a collision
    for (int thi = 0; thi < particles.size(); ++thi)
    {
        for (int other = 0; other < particles.size(); ++other)
        {
            if (thi != other)
            {
                Particle *part1 = particles[thi];   // Pointer to first particle
                Particle *part2 = particles[other]; // Pointer to second particle

                const sf::Vector2f collision_axis = sf::Vector2f(part1->x, part1->y) - sf::Vector2f(part2->x, part2->y);
                float dist = distance(part1->x, part1->y, part2->x, part2->y);
                float overlap_thresh = part1->size + part2->size;

                if (dist < overlap_thresh)
                {
                    // Normalize the collision axis
                    const sf::Vector2f n = collision_axis / dist;
                    const float delta = overlap_thresh - dist;

                    // Only apply a correction if the overlap is significant enough
                    if (delta > minMoveThreshold)
                    {
                        // Apply a smaller collision correction factor for damping
                        const sf::Vector2f add = .3f * delta * n;

                        part2->x -= add.x;
                        part2->y -= add.y;
                        part1->x += add.x;
                        part1->y += add.y;

                        // Optionally, apply velocity damping after collision resolution
                        part1->vx *= 0.999f; // Damp velocity for part1
                        part1->vy *= 0.999f;
                        part2->vx *= 0.999f; // Damp velocity for part2
                        part2->vy *= 0.999f;
                    }
                }
            }
        }
    }
}
