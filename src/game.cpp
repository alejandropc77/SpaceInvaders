#include "../inc/game.hpp"

// constructor
Game::Game()
{
    obstacles = CreateObstacles();
}
// destructor
Game::~Game()
{

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
}

void Game::Update()
{
    // Update all lasers inside spaceship's lasers vector
    for(Laser& laser: spaceship.lasers)
    {
        laser.Update();
    }
    DeleteInactiveLasers();
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