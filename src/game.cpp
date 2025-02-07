#include "../inc/game.hpp"
#include <iostream>

// constructor
Game::Game()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0;
    timeLastSpawn = 0;
    mysteryShipSpawnInterval = GetRandomValue(10, 20);
}
// destructor
Game::~Game()
{
    Alien::UnloadImages();
}

void Game::Draw()
{
    // Draw the spaceship into the screnn
    spaceship.Draw();
    // Draw lasers from lasers vector into the screen
    for(Laser& laser: spaceship.lasers)
    {
        laser.Draw();
    }
    // Draw obstacles
    for(auto& obstacle: obstacles)
    {
        obstacle.Draw();
    }

    // Draw aliens
    for(auto& alien: aliens)
    {
        alien.Draw();
    }

    // Draw alien lasers
    for(auto& laser : alienLasers)
    {
        laser.Draw();
    }

    // Draw mystery ship
    mysteryShip.Draw();
}

void Game::Update()
{
    // Mystery ship spawn logic
    double currentTime = GetTime();
    // If interval of time has passed, spawn new mystery ship
    if(currentTime - timeLastSpawn > mysteryShipSpawnInterval)
    {
        mysteryShip.Spawn();
        timeLastSpawn = GetTime();
        mysteryShipSpawnInterval = GetRandomValue(10, 20);
    }

    // Update all lasers inside spaceship's lasers vector
    for(Laser& laser: spaceship.lasers)
    {
        laser.Update();
    }
    MoveAliens();
    // Shoot laser alien
    AlienShootLaser();
    // Update all alien lasers to move
    for(Laser& laser : alienLasers)
    {
        laser.Update();
    }

    // Delete all inactive lasers (lasers that have exited screen, or have collided)
    DeleteInactiveLasers();
    // Update position of mystery ship
    mysteryShip.Update();
    // Check for all in game collisions
    CheckForCollisions();


}
// Method to handle user's input
void Game::HandleInput()
{
    if(IsKeyDown(KEY_LEFT))
    {
        spaceship.MoveLeft();
    }
    else if(IsKeyDown(KEY_RIGHT))
    {
        spaceship.MoveRight();
    }
    else if(IsKeyDown(KEY_SPACE))
    {
        spaceship.FireLaser();
    }
}

void Game::DeleteInactiveLasers()
{
    for(auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();)
    {
        if(!it->active)
        {
            it = spaceship.lasers.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for(auto it = alienLasers.begin(); it != alienLasers.end();)
    {
        if(!it->active)
        {
            it = alienLasers.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size()*3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth))/5;

    for(int i = 0; i < 4; i++)
    {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 150)}));
    }
    return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    for(int row = 0; row < 5; row++) {
        for(int col = 0; col < 11; col++) {
            // Assign alien type depending on row where alien is created
            int alienType;
            if(row == 0) {
                alienType = 3;
            } else if (row == 1 || row == 2) {
                alienType = 2;
            } else {
                alienType = 1;
            }
            // Figure out position of each Alien. (Also add offset to center Alien grid)
            float x = 75 + col * 55;
            float y = 110 + row * 55;
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }
    return aliens;
}

void Game::MoveAliens()
{
    for(auto& alien: aliens)
    {
        if(alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth())
        {
            aliensDirection = -1;
            MoveDownAliens(4);
        } else if (alien.position.x < 0)
        {
            aliensDirection = 1;
            MoveDownAliens(4);
        }
        
        alien.Update(aliensDirection);
    }
}

void Game::MoveDownAliens(int distance)
{
    for(auto& alien : aliens)
    {
        alien.position.y += distance;
    }
}

void Game::AlienShootLaser()
{
    double currentTime = GetTime();
    if(currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty())
    {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien& alien = aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type - 1].width/2,
                                    alien.position.y + alien.alienImages[alien.type - 1].height}, 6));
        timeLastAlienFired = GetTime();
    }
}

// Check for all in-game collisions
void Game::CheckForCollisions()
{
    // Spaceship lasers
    // Check if spaceship lasers collide with aliens
    for(auto& laser: spaceship.lasers)
    {
        // Use iterators to iterate through all aliens 
        auto it = aliens.begin();
        while(it != aliens.end())
        {
            // Check for collisions between spaceship lasers and aliens
            if(CheckCollisionRecs(it->getRect(), laser.getRect()))
            {
                // if collision is detected, remove alien from aliens vector
                it = aliens.erase(it);
                // deactivate laser, for it to be deleted with DeleteInactiveLasers()
                laser.active = false;
            } else
            {
                // If no collisions, move to next alien to check for collision
                ++it;
            }
        }
        // Check if spaceship lasers collide with obstacle blocks
        // Iterate through each obstacle
        for(auto& obstacle : obstacles)
        {
            // Iterate through all blocks inside obstacle
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end())
            {
                // Check for spaceship laser collision with block inside obstacle
                if(CheckCollisionRecs(it->getRect(), laser.getRect()))
                {
                    // If there is a collision delete block
                    it = obstacle.blocks.erase(it);
                    // If collision, delete laser
                    laser.active = false;
                } else
                {
                    // If no collision, move onto next block of obstacle
                    ++it;
                }
            }
        }
        // Check for spaceship laser collision with mystery ship
        if(CheckCollisionRecs(mysteryShip.getRect(), laser.getRect()))
        {
            mysteryShip.alive = false;
            laser.active = false;
        }
    }

    // Alien lasers collision check
    for(auto& laser: alienLasers)
    {
        if(CheckCollisionRecs(laser.getRect(), spaceship.getRect()))
        {
            laser.active = false;
            std::cout << "Spaceship was hit!" << std::endl;
        }

        for(auto& obstacle : obstacles)
        {
            // Iterate through all blocks inside obstacle
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end())
            {
                // Check for alien laser collision with block inside obstacle
                if(CheckCollisionRecs(it->getRect(), laser.getRect()))
                {
                    // If there is a collision delete block
                    it = obstacle.blocks.erase(it);
                    // If collision, delete laser
                    laser.active = false;
                } else
                {
                    // If no collision, move onto next block of obstacle
                    ++it;
                }
            }
        }
    }

    // Check for Alien collision with Obstacle
    for(auto& alien : aliens)
    {
        for(auto& obstacle: obstacles)
        {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it->getRect(), alien.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                } else
                {
                    ++it;
                }
            }
        }

        if(CheckCollisionRecs(alien.getRect(), spaceship.getRect()))
        {
            std::cout << "Spaceship hit by Alien" << std::endl;
        }
    }
}