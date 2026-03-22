#include <SFML/Graphics.hpp>

class GameFrameClock {
public:
    sf::Clock gameClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    const sf::Time timePerFrame = sf::seconds(1.f/60.f);

    sf::Time updateFramesWaterTile = sf::seconds(0.5);
    sf::Time updateFramesEnemyTank = sf::seconds(0.33);

    sf::Clock waterFramesClock;
    sf::Clock enemyTankFramesClock;
};