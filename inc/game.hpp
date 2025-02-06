#pragma once
#include "alien.hpp"
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "mysteryship.hpp"


class Game 
{
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();
    private:
        // Lasers member functions
        void DeleteInactiveLasers();
        // Obstacles member functions
        std::vector<Obstacle> CreateObstacles();
        // Aliens member functions
        std::vector<Alien> CreateAliens();
        void MoveAliens();
        void MoveDownAliens(int distance);
        void AlienShootLaser();
        // Spaceship member variables
        Spaceship spaceship;
        // Obstacles member variables
        std::vector<Obstacle> obstacles;
        // Aliens member variables
        std::vector<Alien> aliens;
        int aliensDirection;
        std::vector<Laser> alienLasers;
        constexpr static float alienLaserShootInterval = 0.35;
        float timeLastAlienFired;
        // Mystery Ship member variables
        MysteryShip mysteryShip;
        float mysteryShipSpawnInterval;
        float timeLastSpawn;
};