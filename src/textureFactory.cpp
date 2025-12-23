#include "../include/textureFactory.h"
#include "../include/textures.h"

void TextureFactory::loadFromFile(int code)
{

    if (code == 1 && !walkableTexture.loadFromFile("Images/walkableGround.png")) {
        throw std::runtime_error("Failed to load walkable texture");
    }
    else if (code == 3 && !destructibleTexture.loadFromFile("Images/destructibleBlock.png")) {
        throw std::runtime_error("Failed to load destructible texture");
    }
    else if(code == 4 && !mirror1Texture.loadFromFile("Images/Mirror1WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 1 texture");
    }
    else if(code == 5 && !mirror2Texture.loadFromFile("Images/Mirror2WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 2 texture");
    }
    else if(code == 6 && !mirror3Texture.loadFromFile("Images/Mirror3WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 3 texture");
    }
    else if(code == 7 && !mirror4Texture.loadFromFile("Images/Mirror4WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 4 texture");
    }
    else if(code == 8 && !waterTileTexture.loadFromFile("Images/waterTile.png")) {
        throw std::runtime_error("Failed to load water tile texture");
    }
    else if(code == 8 && !waterSecondFrameTexture.loadFromFile("Images/waterTileSecondFrame.png")) {
        throw std::runtime_error("Failed to load water tile second frame texture");
	}
    else if(code == 9 && !movableBlockTexture.loadFromFile("Images/MovableBlock.png")) {
        throw std::runtime_error("Failed to load water tile texture");
    }
    else if(code == 10 && !flagTexture.loadFromFile("Images/flagWithBackground.png")) {
        throw std::runtime_error("Failed to load flag texture");
    }
    else if(code == 11 && !undestructableBlockTex.loadFromFile("Images/undestructibleBlock.png")) {
        throw std::runtime_error("Failed to load undestructible block texture");
    }
    else if(code == 12 && !EnemyTank1LeftTexture.loadFromFile("Images/tank1.png")) {
        throw std::runtime_error("Failed to load enemy tank left texture");
    }
    else if(code == 13 && !EnemyTank1RightTexture.loadFromFile("Images/tank2.png")) {
        throw std::runtime_error("Failed to load enemy tank left texture");
    }
    else if(code == 14 && !EnemyTank1UpTexture.loadFromFile("Images/tank3.png")) {
        throw std::runtime_error("Failed to load enemy tank up texture");
    }
    else if(code == 15 && !EnemyTank1DownTexture.loadFromFile("Images/tank4.png")) {
        throw std::runtime_error("Failed to load enemy tank down texture");
    }
    else if(code == 16 && !destroyedTankLeftTexture.loadFromFile("Images/destroyedTank1.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    else if(code == 17 && !destroyedTankUpTexture.loadFromFile("Images/destroyedTank2.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    else if(code == 18 && !destroyedTankDownTexture.loadFromFile("Images/destroyedTank3.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    else if(code == 19 && !destroyedTankRightTexture.loadFromFile("Images/destroyedTank4.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    else if(code == 20 && !transportTrackDownTex.loadFromFile("Images/transportTrack4.png")) {
        throw std::runtime_error("Failed to load transport track texture");
    }
    else if(code == 21 && !transportTrackUpTex.loadFromFile("Images/transportTrack3.png")) {
        throw std::runtime_error("Failed to load transport track 3 texture");
    }
    else if(code == 22 && !transportTrackRightTex.loadFromFile("Images/transportTrack2.png")) {
        throw std::runtime_error("Failed to load transport track 3 texture");
    }
    else if(code == 23 && !transportTrackLeftTex.loadFromFile("Images/transportTrack.png")) {
        throw std::runtime_error("Failed to load transport track 3 texture");
    }
    else if(code == 50 && !tileInWaterTex.loadFromFile("Images/tileInWater.png")) {
        throw std::runtime_error("Failed to load tile in water texture");
    }
    else if(code == 12 && !thirdFrameEnemyTank1Texture.loadFromFile("Images/tank1SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    else if(code == 12 && !secondFrameEnemyTank1Texture.loadFromFile("Images/tank1ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
    else if(code == 13 && !thirdFrameEnemyTank2Texture.loadFromFile("Images/tank2SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    else if(code == 13 && !secondFrameEnemyTank2Texture.loadFromFile("Images/tank2ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
    else if(code == 14 && !thirdFrameEnemyTank3Texture.loadFromFile("Images/tank3SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    else if(code == 14 && !secondFrameEnemyTank3Texture.loadFromFile("Images/tank3ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
    else if(code == 15 && !thirdFrameEnemyTank4Texture.loadFromFile("Images/tank4SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    else if(code == 15 && !secondFrameEnemyTank4Texture.loadFromFile("Images/tank4ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
}