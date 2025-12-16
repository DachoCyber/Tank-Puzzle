#include "bulletHitInfo.h"
#include "tile.h"
#include "playerTileSignal.h"

#include <math.h>           // For generating random numbers
#include <SFML/Graphics.hpp>

class CustomTile : public Tile {
public:
    CustomTile (TileSignal bulletTileSignal, PlayerTileSignal playerTileSignal) {

        
    }
    virtual ~CustomTile () { };

    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(sprite);
    }
private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Image img;
    void generateUniqueID ();
};