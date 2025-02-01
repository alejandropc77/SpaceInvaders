#include "../inc/spaceship.hpp"

Spaceship::Spaceship()
{
    // Load image or "texture" from graphics folder
    image = LoadTexture("../graphics/spaceshipGame.png");
    // Place spaceship in the middle of screen in the x axis
    position.x = (GetScreenWidth() - image.width)/2 + 70;
    // Place spaceship at bottom of the screen
    position.y = GetScreenHeight() - 60;
    lastFireTime = 0;
}

Spaceship::~Spaceship()
{
    UnloadTexture(image);
}

void Spaceship::Draw()
{
    // Draw spaceship img with 0.3 scaling (make it smaller)
    DrawTextureEx(image, position, 0, 0.3, WHITE);
}

void Spaceship::MoveLeft()
{
    // Move spaceship 5 pixels to the left
    this->position.x -= 5;
    // If position is past the beginning of the screen, reset to beginning of screen
    if(this->position.x < 0)
    {
        this->position.x = 0;
    }
}

void Spaceship::MoveRight()
{
    // Move spaceship 5 pixels to the right
    this->position.x += 5;
    // If position is past the end of the screen, reset to end of screen
    if(this->position.x > 690)
    {
        this->position.x = 690;
    }
}
// 
void Spaceship::FireLaser()
{
    if(GetTime() - lastFireTime >= 0.25)
    {
        // Add new fired laser to "lasers" vector
        lasers.push_back(Laser({this->position.x + 33, this->position.y}, -6));
        lastFireTime = GetTime();
    }

}