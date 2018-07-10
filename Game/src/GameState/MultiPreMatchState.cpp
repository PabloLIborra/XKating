#include "MultiPreMatchState.h"

void MultiPreMatchState::init() {

    //Bind all managers that are going to be used
    eventManager  = &EventManager::getInstance();
    inputManager  = &InputManager::getInstance();
    renderManager = &RenderManager::getInstance();
    objectManager = &ObjectManager::getInstance();
    audioManager = &AudioManager::getInstance();

    Game::getInstance().setAccumulatedTime(0);
    GlobalVariables::getInstance().setIgnoreInput(true);
    
}

void MultiPreMatchState::update(float &accumulatedTime) {

    if(stateCounter == -1) {
        stateCounter = 0;
        currTime = maxTime;

        //initial camera movement
        cameraPositions[0] = glm::vec3(30,10,0);
        cameraPositions[1] = glm::vec3(0,0,0);
        cameraPositions[2] = glm::vec3(-30,10,0);
        cameraPositions[3] = glm::vec3(-50,10,0);
    }
    else if (stateCounter == 0 && currTime > 0){

        cameraPositions[4] = cameraPositions[0] + ((maxTime-currTime) * (cameraPositions[2]-cameraPositions[0]))/maxTime;
        cameraPositions[5] = cameraPositions[1] + ((maxTime-currTime) * (cameraPositions[3]-cameraPositions[1]))/maxTime;

        RenderManager::getInstance().getRenderFacade()->setCameraTarget(cameraPositions[4], cameraPositions[5]);
        
        currTime -= accumulatedTime;
    }
    else if (stateCounter == 0 && currTime <= 0){

        stateCounter = 1;
        currTime = maxTime;

        //initial camera movement
        cameraPositions[0] = glm::vec3(-0,100,0);
        cameraPositions[1] = glm::vec3(-0,100,0);
        cameraPositions[2] = glm::vec3(-0,150,50);
        cameraPositions[3] = glm::vec3(-0,100,80);
    }
    else if (stateCounter == 1 && currTime > 0){

        cameraPositions[4] = cameraPositions[0] + ((maxTime-currTime) * (cameraPositions[2]-cameraPositions[0]))/maxTime;
        cameraPositions[5] = cameraPositions[1] + ((maxTime-currTime) * (cameraPositions[3]-cameraPositions[1]))/maxTime;

        RenderManager::getInstance().getRenderFacade()->setCameraTarget(cameraPositions[4], cameraPositions[5]);
        
        currTime -= accumulatedTime;
    }
    else if (stateCounter == 1 && currTime <= 0){

        stateCounter = 2;
        currTime = maxTime;

        //initial camera movement
        cameraPositions[0] = glm::vec3(-120,50,-0);
        cameraPositions[1] = glm::vec3(-100,50,-0);
        cameraPositions[2] = glm::vec3(-100,30,-0);
        cameraPositions[3] = glm::vec3(-100,30,-0);
    }
    else if (stateCounter == 2 && currTime > 0){

        cameraPositions[4] = cameraPositions[0] + ((maxTime-currTime) * (cameraPositions[2]-cameraPositions[0]))/maxTime;
        cameraPositions[5] = cameraPositions[1] + ((maxTime-currTime) * (cameraPositions[3]-cameraPositions[1]))/maxTime;

        RenderManager::getInstance().getRenderFacade()->setCameraTarget(cameraPositions[4], cameraPositions[5]);
        
        currTime -= accumulatedTime;
    }
    else if (stateCounter == 2 && currTime <= 0){

        stateCounter = 3;
        currTime = maxTime - 0.01;

        //initial camera movement
        cameraPositions[0] = glm::vec3(95,12.6469,-20);
        cameraPositions[1] = glm::vec3(65,13.7496,-20);
        cameraPositions[2] = glm::vec3(65,12.6469,-20);
        cameraPositions[3] = glm::vec3(35,13.7496,-20);

        //Set event for countdown start
        EventManager::getInstance().addEvent(Event {EventType::Match_Countdown});
        GlobalVariables::getInstance().setCountdown(3);
    }
    else if (stateCounter == 3 && currTime > 0){

        GlobalVariables::getInstance().setCountdown((int)currTime);

        cameraPositions[4] = cameraPositions[0] + ((maxTime-currTime) * (cameraPositions[2]-cameraPositions[0]))/maxTime;
        cameraPositions[5] = cameraPositions[1] + ((maxTime-currTime) * (cameraPositions[3]-cameraPositions[1]))/maxTime;

        RenderManager::getInstance().getRenderFacade()->setCameraTarget(cameraPositions[4], cameraPositions[5]);
        
        currTime -= accumulatedTime;
    }
    else if (stateCounter == 3 && currTime <= 0){

        stateCounter = -1;

        GlobalVariables::getInstance().setIgnoreInput(false);
        Game::getInstance().setState(IGameState::stateType::MULTIMATCH);
        
    }

    //Update input manager
    inputManager->update();
    
    //Update audio manager
    audioManager->update();

    //Event manager has to be the last to be updated
    eventManager->update();

    //Sets if the game keeps running or not
    Game::getInstance().setStay(objectManager->getGameRunning());
}

void MultiPreMatchState::draw() {
    renderManager->draw();
}

void MultiPreMatchState::close() {
    
}