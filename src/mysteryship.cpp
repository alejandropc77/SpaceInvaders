#include "../inc/mysteryship.hpp"

MysteryShip::MysteryShip()
{
    // Load image and set alive boolean to false for now
    image = LoadTexture("../graphics/mystery.png");
    alive = false;
}

MysteryShip::~MysteryShip()
{
    UnloadTexture(image);
}

void MysteryShip::Spawn()
{
    // Spawn 90 pixels below top of screen
    position.y = 90;
    // Choose at random if ship will spawn from left or right of screen
    int side = GetRandomValue(0, 1);

    if(side == 0)
    {
        position.x = 0;
        speed = 3;
    }
    else
    {
        position.x = GetScreenWidth() - image.width;
        speed = -3;
    }
    alive = true;
}

void MysteryShip::Update()
{
    // If mystery ship is alive, move it
    if(alive)
    {
        position.x += speed;
        // If ship leaves the screen, set alive boolean to false (make it dissapear)
        if(position.x > GetScreenWidth() - image.width || position.x < 0)
        {
            alive = false;
        }
    }
}

void MysteryShip::Draw()
{
    if(alive)
    {
        DrawTextureV(image, position, WHITE);
    }
}

Rectangle MysteryShip::getRect()
{
    if(alive)
    {
        return {position.x, position.y, float(image.width), float(image.height)};
    }
    else
    {
        return {position.x, position.y, 0, 0};
    }
    
}