#include "../include/inputManager.h"

void InputManager::setPressedKey(sf::Keyboard::Key pK)
{
    if ( pK != sf::Keyboard::Up   && pK != sf::Keyboard::Down  &&
         pK != sf::Keyboard::Left && pK != sf::Keyboard::Right &&
         pK != sf::Keyboard::Space )
    {

    }
    pressedKey = pK;
}

sf::Keyboard::Key InputManager::getPressedKey() const
{
    return pressedKey;
}

bool InputManager::getIsBlocked()
{
    return blocked;
}

void InputManager::blockInput() 
{    
    blocked = true;
}

void InputManager::unblockInput() 
{    
    blocked = false;
}

InputManager :: InputManager () : pressedKey(sf::Keyboard::Unknown)
{
}

InputManager::~InputManager()
{
}