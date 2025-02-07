#include "../inc/laser.hpp"
#include <iostream>

// constructor
Laser::Laser(Vector2 position, int speed)
{
    this->position = position;
    this->speed = speed;
    active = true;
}

// draw method. draws yellow rectangle to screen
void Laser::Draw()
{
    if(active)
        DrawRectangle(position.x, position.y, 4, 15, {243, 216, 63, 255});
}

// update method. updates position of laser to create movement
void Laser::Update()
{
    this->position.y += this->speed;
    if(active)
    {
        if(position.y > GetScreenHeight() || position.y < 0)
        {
            active = false;
        }
            
    }
}

Rectangle Laser::getRect()
{
    Rectangle rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.width = 4;
    rect.height = 15;
    return rect;
}
