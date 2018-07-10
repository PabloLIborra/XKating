#include "PauseState.h"

void PauseState::init() {
    //Bind all managers that are going to be used
    eventManager  = &EventManager::getInstance();
    inputManager  = &InputManager::getInstance();
    renderManager = &RenderManager::getInstance();
    objectManager = &ObjectManager::getInstance();
    audioManager = &AudioManager::getInstance();

    Game::getInstance().setAccumulatedTime(0);
}

void PauseState::update(float &accumulatedTime) {
    //Update input manager
    inputManager->update();
    
    //Update audio manager
    audioManager->update();

    //Event manager has to be the last to be updated
    eventManager->update();

    //Sets if the game keeps running or not
    Game::getInstance().setStay(objectManager->getGameRunning());
}

void PauseState::draw() {
    renderManager->draw();
}

void PauseState::close() {
    
}