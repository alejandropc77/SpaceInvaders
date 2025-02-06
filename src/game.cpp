#include "../inc/game.hpp"

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


    DeleteInactiveLasers();
    mysteryShip.Update();


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