#include "ClientLobbyState.h"

//==============================================================
// MAIN FUNCTIONS
//==============================================================

void ClientLobbyState::init() {

    //Initialize managers
    networkManager = &NetworkManager::getInstance();
    objectManager  = &ObjectManager::getInstance();
    renderManager  = &RenderManager::getInstance();
    inputManager   = &InputManager::getInstance();
    eventManager   = &EventManager::getInstance();

    //Initialize image
    text1 = renderManager->createText(std::string("... Conectando ..."), glm::vec2(500,500), 255, 0, 0, 255, glm::vec2(300,300), std::string("media/font/Razor_m.png"));

    //Initialize lobby
    /*if(!networkManager->getConnected())
    {
        networkManager->initLobby();
    }*/
}

void ClientLobbyState::update(float &accumulatedTime) {
    //Check input
    inputManager->update();

    //Update network
    /*networkManager->updateLobby();
    if(networkManager->getStarted())
    {
        //Close
        close();

        //Send drawing event
        EventManager::getInstance().addEvent(Event {EventType::Game_PlayerSelection});

        //Change game to online match
        GlobalVariables::getInstance().setOnline(true);
        Game::getInstance().setState(IGameState::stateType::MULTISELECTION);
    }*/

    //Update event
    eventManager->update();


    //See if game is still running
    Game::getInstance().setStay(objectManager->getGameRunning());
}

void ClientLobbyState::draw() {
    //Update GUI only
    renderManager->draw();
}

void ClientLobbyState::close() {
    //Delete connecting text
    renderManager->cleanVI();
}