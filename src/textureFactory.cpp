#include "../include/textureFactory.h"
#include "../include/textures.h"
#include "../include/tileCode.h"

void TextureFactory::loadFromFile(int code)
{

    if (code == WALKABLE_GROUND && !walkableTexture.loadFromFile("Images/walkableGround.png")) {
        throw std::runtime_error("Failed to load walkable texture");
    }
    else if (code == DESTRUCTIBLE_BLOCK && !destructibleTexture.loadFromFile("Images/destructibleBlock.png")) {
        throw std::runtime_error("Failed to load destructible texture");
    }
    else if(code == MIRROR_1 && !mirror1Texture.loadFromFile("Images/Mirror1WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 1 texture");
    }
    else if(code == MIRROR_2 && !mirror2Texture.loadFromFile("Images/Mirror2WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 2 texture");
    }
    else if(code == MIRROR_3 && !mirror3Texture.loadFromFile("Images/Mirror3WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 3 texture");
    }
    else if(code == MIRROR_4 && !mirror4Texture.loadFromFile("Images/Mirror4WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 4 texture");
    }
    else if(code == WATER && !waterTileTexture.loadFromFile("Images/waterTile.png")) {
        throw std::runtime_error("Failed to load water tile texture");
    }
    else if(code == WATER && !waterSecondFrameTexture.loadFromFile("Images/waterTileSecondFrame.png")) {
        throw std::runtime_error("Failed to load water tile second frame texture");
	}
    else if(code == MOVABLE_BLOCK && !movableBlockTexture.loadFromFile("Images/MovableBlock.png")) {
        throw std::runtime_error("Failed to load water tile texture");
    }
    else if(code == FLAG && !flagTexture.loadFromFile("Images/flagWithBackground.png")) {
        throw std::runtime_error("Failed to load flag texture");
    }
    else if(code == UNDESTRUCTABLE_BLOCK && !undestructableBlockTex.loadFromFile("Images/undestructibleBlock.png")) {
        throw std::runtime_error("Failed to load undestructible block texture");
    }
    else if(code == ENEMY_TANK_LEFT && !EnemyTank1LeftTexture.loadFromFile("Images/tank1.png")) {
        throw std::runtime_error("Failed to load enemy tank left texture");
    }
    else if(code == ENEMY_TANK_RIGHT && !EnemyTank1RightTexture.loadFromFile("Images/tank2.png")) {
        throw std::runtime_error("Failed to load enemy tank left texture");
    }
    else if(code == ENEMY_TANK_UP && !EnemyTank1UpTexture.loadFromFile("Images/tank3.png")) {
        throw std::runtime_error("Failed to load enemy tank up texture");
    }
    else if(code == ENEMY_TANK_DOWN && !EnemyTank1DownTexture.loadFromFile("Images/tank4.png")) {
        throw std::runtime_error("Failed to load enemy tank down texture");
    }
    else if(code == DESTROYED_TANK_LEFT && !destroyedTankLeftTexture.loadFromFile("Images/destroyedTank1.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    else if(code == DESTROYED_TANK_UP && !destroyedTankUpTexture.loadFromFile("Images/destroyedTank2.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    else if(code == DESTROYED_TANK_DOWN && !destroyedTankDownTexture.loadFromFile("Images/destroyedTank3.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    else if(code == DESTROYED_TANK_RIGHT && !destroyedTankRightTexture.loadFromFile("Images/destroyedTank4.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    else if(code == TRANSPORT_TRACK_LEFT && !transportTrackDownTex.loadFromFile("Images/transportTrack4.png")) {
        throw std::runtime_error("Failed to load transport track texture");
    }
    else if(code == TRANSPORT_TRACK_RIGHT && !transportTrackUpTex.loadFromFile("Images/transportTrack3.png")) {
        throw std::runtime_error("Failed to load transport track 3 texture");
    }
    else if(code == TRANSPORT_TRACK_UP && !transportTrackRightTex.loadFromFile("Images/transportTrack2.png")) {
        throw std::runtime_error("Failed to load transport track 3 texture");
    }
    else if(code == TRANSPORT_TRACK_DOWN && !transportTrackLeftTex.loadFromFile("Images/transportTrack.png")) {
        throw std::runtime_error("Failed to load transport track 3 texture");
    }
    else if(code == TILE_IN_WATER && !tileInWaterTex.loadFromFile("Images/tileInWater.png")) {
        throw std::runtime_error("Failed to load tile in water texture");
    }
    else if(code == ENEMY_TANK_LEFT && !thirdFrameEnemyTank1Texture.loadFromFile("Images/tank1SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    else if(code == ENEMY_TANK_LEFT && !secondFrameEnemyTank1Texture.loadFromFile("Images/tank1ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
    else if(code == ENEMY_TANK_RIGHT && !thirdFrameEnemyTank2Texture.loadFromFile("Images/tank2SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    else if(code == ENEMY_TANK_RIGHT && !secondFrameEnemyTank2Texture.loadFromFile("Images/tank2ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
    else if(code == ENEMY_TANK_UP && !thirdFrameEnemyTank3Texture.loadFromFile("Images/tank3SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    else if(code == ENEMY_TANK_UP && !secondFrameEnemyTank3Texture.loadFromFile("Images/tank3ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
    else if(code == ENEMY_TANK_DOWN && !thirdFrameEnemyTank4Texture.loadFromFile("Images/tank4SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    else if(code == ENEMY_TANK_DOWN && !secondFrameEnemyTank4Texture.loadFromFile("Images/tank4ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
}
