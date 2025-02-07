#include "../inc/alien.hpp"

Texture2D Alien::alienImages[3] = {};

Alien::Alien(int type, Vector2 position)
{
    this->type = type;
    this->position = position;
    // If image of this type of alien has not been loaded, load it
    if(alienImages[type - 1].id == 0)
    {
        // Load image according to type of alien
        switch(type)
        {
            case 1:
                alienImages[0] = LoadTexture("../graphics/alien_1.png");
                break;
            case 2:
                alienImages[1] = LoadTexture("../graphics/alien_2.png");
                break;
            case 3:
                alienImages[2] = LoadTexture("../graphics/alien_3.png");
                break;
            default:
                alienImages[0] = LoadTexture("../graphics/alien_1.png");
                break;           
        }
    }
}

// Draw texture(image) into the screen
void Alien::Draw()
{
    DrawTextureV(alienImages[type - 1], position, WHITE);
}

// Return type of this alien
int Alien::GetType()
{
    return type;
}

// Unload all alien images
void Alien::UnloadImages()
{
    for(int i = 0; i < 4; i++)
    {
        UnloadTexture(alienImages[i]);
    }
}

// Update position of alien, using the correct direction
void Alien::Update(int direction)
{
    position.x += direction;
}

// Return the rectangle that covers the alien image
Rectangle Alien::getRect()
{
    return {position.x, position.y, 
            float(alienImages[type - 1].width), 
            float(alienImages[type - 1].height)};
}