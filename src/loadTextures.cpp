#include "../include/loadTextures.h"

void loadTextures() {
    if (!walkableTexture.loadFromFile("Images/walkableGround.png")) {
        throw std::runtime_error("Failed to load walkable texture");
    }
    if (!destructibleTexture.loadFromFile("Images/destructibleBlock.png")) {
        throw std::runtime_error("Failed to load destructible texture");
    }
    if(!mirror1Texture.loadFromFile("Images/Mirror1WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 1 texture");
    }
    if(!mirror2Texture.loadFromFile("Images/Mirror2WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 2 texture");
    }
    if(!mirror3Texture.loadFromFile("Images/Mirror3WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 3 texture");
    }
    if(!mirror4Texture.loadFromFile("Images/Mirror4WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 4 texture");
    }
    if(!waterTileTexture.loadFromFile("Images/waterTile.png")) {
        throw std::runtime_error("Failed to load water tile texture");
    }
    if(!waterSecondFrameTexture.loadFromFile("Images/waterTileSecondFrame.png")) {
        throw std::runtime_error("Failed to load water tile second frame texture");
	}
    if(!movableBlockTexture.loadFromFile("Images/MovableBlock.png")) {
        throw std::runtime_error("Failed to load water tile texture");
    }
    if(!flagTexture.loadFromFile("Images/flagWithBackground.png")) {
        throw std::runtime_error("Failed to load flag texture");
    }
    if(!undestructableBlockTex.loadFromFile("Images/undestructibleBlock.png")) {
        throw std::runtime_error("Failed to load undestructible block texture");
    }
    if(!EnemyTank1LeftTexture.loadFromFile("Images/tank1.png")) {
        throw std::runtime_error("Failed to load enemy tank left texture");
    }
    if(!EnemyTank1RightTexture.loadFromFile("Images/tank2.png")) {
        throw std::runtime_error("Failed to load enemy tank left texture");
    }
    if(!EnemyTank1UpTexture.loadFromFile("Images/tank3.png")) {
        throw std::runtime_error("Failed to load enemy tank up texture");
    }
    if(!EnemyTank1DownTexture.loadFromFile("Images/tank4.png")) {
        throw std::runtime_error("Failed to load enemy tank down texture");
    }
    if(!destroyedTankLeftTexture.loadFromFile("Images/destroyedTank1.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    if(!destroyedTankUpTexture.loadFromFile("Images/destroyedTank2.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    if(!destroyedTankDownTexture.loadFromFile("Images/destroyedTank3.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    if(!destroyedTankRightTexture.loadFromFile("Images/destroyedTank4.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    if(!transportTrackDownTex.loadFromFile("Images/transportTrack4.png")) {
        throw std::runtime_error("Failed to load transport track texture");
    }
    if(!transportTrackUpTex.loadFromFile("Images/transportTrack3.png")) {
        throw std::runtime_error("Failed to load transport track 3 texture");
    }
    if(!transportTrackRightTex.loadFromFile("Images/transportTrack2.png")) {
        throw std::runtime_error("Failed to load transport track 3 texture");
    }
    if(!transportTrackLeftTex.loadFromFile("Images/transportTrack.png")) {
        throw std::runtime_error("Failed to load transport track 3 texture");
    }
    if(!tileInWaterTex.loadFromFile("Images/tileInWater.png")) {
        throw std::runtime_error("Failed to load tile in water texture");
    }
    if(!thirdFrameEnemyTank1Texture.loadFromFile("Images/tank1SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    if(!secondFrameEnemyTank1Texture.loadFromFile("Images/tank1ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
    if(!thirdFrameEnemyTank2Texture.loadFromFile("Images/tank2SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    if(!secondFrameEnemyTank2Texture.loadFromFile("Images/tank2ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
    if(!thirdFrameEnemyTank3Texture.loadFromFile("Images/tank3SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    if(!secondFrameEnemyTank3Texture.loadFromFile("Images/tank3ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
    if(!thirdFrameEnemyTank4Texture.loadFromFile("Images/tank4SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    if(!secondFrameEnemyTank4Texture.loadFromFile("Images/tank4ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    } 
}