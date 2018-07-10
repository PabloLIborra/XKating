#include "MultiSelectionState.h"

//==============================================
// Additional functions
//==============================================
void multiLoadAnimations();
void multiAddStateChange(EventData eData);

void MultiSelectionState::init() 
{

    if(!initialized) 
    {
        //Bind all managers that are going to be used
        eventManager   = &EventManager::getInstance();
        inputManager   = &InputManager::getInstance();
        renderManager  = &RenderManager::getInstance();
        objectManager  = &ObjectManager::getInstance();
        audioManager   = &AudioManager::getInstance();
        networkManager = &NetworkManager::getInstance();

        eventManager->addListener(EventListener {EventType::Key_Pressed, multiAddStateChange});

        initialized = true;

        cameraPositions[4] = glm::vec3(10,-27,0);
        cameraPositions[5] = glm::vec3(0,-30,0);

        currPlayer = 0;
    }

    eventManager->addEvent(Event {Game_PlayerSelection});

    Game::getInstance().setAccumulatedTime(0);

    GlobalVariables::getInstance().setIgnoreInput(true);

    renderManager->getRenderFacade()->setCameraTarget(cameraPositions[4], cameraPositions[5]);
    
    ongoing = false;
    GlobalVariables::getInstance().setFixedPlayer(false);
}

void MultiSelectionState::update(float &accumulatedTime) 
{
    int selectedPlayer = GlobalVariables::getInstance().getSelectedPlayer(); 

    if(GlobalVariables::getInstance().getFixedPlayer()) {

        if(!load){
            EventManager::getInstance().addEvent(Event {EventType::Game_LoadingScreen});
            load = true;
        }
        else if (!GlobalVariables::getInstance().getGameLoaded()) {

            eventManager->addEvent(Event {EventType::GameObject_Delete, EventData {60001}});
            eventManager->addEvent(Event {EventType::GameObject_Delete, EventData {60000}});
            eventManager->addEvent(Event {EventType::GameObject_Delete, EventData {60002}});
            eventManager->addEvent(Event {EventType::GameObject_Delete, EventData {60003}});
            eventManager->addEvent(Event {EventType::GameObject_Delete, EventData {60004}});
            eventManager->addEvent(Event {EventType::GameObject_Delete, EventData {60005}});
    
            eventManager->update();
    
            std::cout << "Empieza a borrar" << std::endl;
    
            IRenderFacade* f = renderManager->getRenderFacade();
    
            f->deleteAnimation("Punk/CharSelect/punkAnimation_000");
            f->deleteAnimation("Crocodile/CharSelect/cocodrilaBonesSelect_000");
            f->deleteAnimation("Cyborg/CharSelect/CyborgFINALAnimation_000");
            f->deleteAnimation("Witch/CharSelect/bruja_000");
    
            std::cout << "Acaba de borrar" << std::endl;

            //Load map
            Game::getInstance().loadMap();

            networkManager->update();

            //Add animations to the characters
            multiLoadAnimations();

            renderManager->getRenderFacade()->changeAnimation(25000, 0);

            if(objectManager->getObject(25001) != nullptr)
                renderManager->getRenderFacade()->changeAnimation(25001, 0);

            if(objectManager->getObject(25002) != nullptr)
                renderManager->getRenderFacade()->changeAnimation(25002, 0);

            if(objectManager->getObject(25003) != nullptr)
                renderManager->getRenderFacade()->changeAnimation(25003, 0);

            GlobalVariables::getInstance().setGameLoaded(true);
            GlobalVariables::getInstance().setIgnoreInput(false);

        }
        else if(load && GlobalVariables::getInstance().getGameLoaded()){
            eventManager->addEvent(Event {EventType::Game_LoadingScreen});
            GlobalVariables::getInstance().setIgnoreInput(false);
        }

    }

    if(!ongoing && currPlayer != selectedPlayer) {
        ongoing = true;
        currTime = maxTime;
        GlobalVariables::getInstance().setSelecting(true);
        if(currPlayer == 0 && selectedPlayer == 3) {
            cameraPositions[0] = glm::vec3(10,-27,0);
            cameraPositions[1] = glm::vec3(0,-30,0);
            cameraPositions[2] = glm::vec3(10,-27,-18);
            cameraPositions[3] = glm::vec3(0,-30,-18);
        }
        else if (currPlayer == 3 && selectedPlayer == 0) {
            cameraPositions[0] = glm::vec3(10,-27,-18);
            cameraPositions[1] = glm::vec3(0,-30,-18);
            cameraPositions[2] = glm::vec3(10,-27,0);
            cameraPositions[3] = glm::vec3(0,-30,0); 
        }
        else if (currPlayer > selectedPlayer) {
            cameraPositions[0] = cameraPositions[4];
            cameraPositions[1] = cameraPositions[5];
            cameraPositions[2] = glm::vec3(10,-27,cameraPositions[4].z + 6);
            cameraPositions[3] = glm::vec3(0,-30,cameraPositions[5].z + 6);
        }
        else if (currPlayer < selectedPlayer) {
            cameraPositions[0] = cameraPositions[4];
            cameraPositions[1] = cameraPositions[5];
            cameraPositions[2] = glm::vec3(10,-27,cameraPositions[4].z - 6);
            cameraPositions[3] = glm::vec3(0,-30,cameraPositions[5].z - 6);
        }
    }

    if(ongoing) {
        cameraPositions[4] = cameraPositions[0] + ((maxTime-currTime) * (cameraPositions[2]-cameraPositions[0]))/maxTime;
        cameraPositions[5] = cameraPositions[1] + ((maxTime-currTime) * (cameraPositions[3]-cameraPositions[1]))/maxTime;

        renderManager->getRenderFacade()->setCameraTarget(cameraPositions[4], cameraPositions[5]);
        
        currTime -= accumulatedTime;

        if(currTime < 0) {
            ongoing = false;
            currPlayer = GlobalVariables::getInstance().getSelectedPlayer();
            GlobalVariables::getInstance().setSelecting(false);
        }
    }

    //Update input manager
    inputManager->update();
    
    //Update audio manager
    audioManager->update();
    renderManager->getRenderFacade()->updateAnimations(accumulatedTime);

    //Event manager has to be the last to be updated
    eventManager->update();

    //Sets if the game keeps running or not
    Game::getInstance().setStay(objectManager->getGameRunning());
}

void MultiSelectionState::draw() 
{
    renderManager->draw();
}

void MultiSelectionState::close() 
{
    
}

void multiLoadAnimations() 
{

    IRenderFacade* f = RenderManager::getInstance().getRenderFacade();

    //f->deleteAnimation("Punk/CharSelect/punkAnimation_000");
    //f->deleteAnimation("Crocodile/CharSelect/cocodrilaBonesSelect_000");
    //f->deleteAnimation("Cyborg/CharSelect/CyborgFINALAnimation_000");
    //f->deleteAnimation("Witch/CharSelect/bruja_000");

    std::cout << "Loading animations" << std::endl;

    f->addAnimation(00, "Punk/Idle/punkAnimation_000", 62, "Punk/DefaultMaterial_Base_Color.png");
    f->addAnimation(01, "Punk/Correr/punkAnimation_000", 70, "Punk/DefaultMaterial_Base_Color.png");
    f->addAnimation(02, "Punk/Choque/punkAnimation_000", 38, "Punk/DefaultMaterial_Base_Color.png");
    f->addAnimation(03, "Punk/LanzarItem/punkAnimation_000", 37, "Punk/DefaultMaterial_Base_Color.png");
    f->addAnimation(04, "Punk/LanzarItemMovimiento/punkAnimation_000", 38, "Punk/DefaultMaterial_Base_Color.png");
    f->addAnimation(05, "Punk/DerrapeDCHA/punkAnimation_000", 81, "Punk/DefaultMaterial_Base_Color.png");
    f->addAnimation(06, "Punk/DerrapeIZQ/punkAnimation_000", 78, "Punk/DefaultMaterial_Base_Color.png");

    f->addAnimation(10, "Crocodile/Idle/cocodrilaBonesSelect_000", 83, "Crocodile/DefaultMaterial_Base_Color.png");
    f->addAnimation(11, "Crocodile/Correr/cocodrilaBonesSelect_000", 20, "Crocodile/DefaultMaterial_Base_Color.png");
    f->addAnimation(12, "Crocodile/Choque/cocodrilaBonesSelect_000", 0, "Crocodile/DefaultMaterial_Base_Color.png");
    f->addAnimation(13, "Crocodile/LanzarItem/cocodrilaBonesSelect_000", 56, "Crocodile/DefaultMaterial_Base_Color.png");
    f->addAnimation(14, "Crocodile/LanzarItemMovimiento/cocodrilaBonesSelect_000", 40, "Crocodile/DefaultMaterial_Base_Color.png");
    f->addAnimation(15, "Crocodile/DerrapeDCHA/cocodrilaBonesSelect_000", 81, "Crocodile/DefaultMaterial_Base_Color.png");
    f->addAnimation(16, "Crocodile/DerrapeIZQ/cocodrilaBonesSelect_000", 78, "Crocodile/DefaultMaterial_Base_Color.png");

    f->addAnimation(20, "Cyborg/Idle/CyborgFINALAnimation_000", 82, "Cyborg/DefaultMaterial_Base_Color.png");
    f->addAnimation(21, "Cyborg/Correr/CyborgFINALAnimation_000", 28, "Cyborg/DefaultMaterial_Base_Color.png");
    f->addAnimation(22, "Cyborg/Choque/CyborgFINALAnimation_000", 38, "Cyborg/DefaultMaterial_Base_Color.png");
    f->addAnimation(23, "Cyborg/LanzarItem/CyborgFINALAnimation_000", 45, "Cyborg/DefaultMaterial_Base_Color.png");
    f->addAnimation(24, "Cyborg/LanzarItemMovimiento/CyborgFINALAnimation_000", 39, "Cyborg/DefaultMaterial_Base_Color.png");
    f->addAnimation(25, "Cyborg/DerrapeDCHA/CyborgFINALAnimation_000", 81, "Cyborg/DefaultMaterial_Base_Color.png");
    f->addAnimation(26, "Cyborg/DerrapeIZQ/CyborgFINALAnimation_000", 68, "Cyborg/DefaultMaterial_Base_Color.png");

    f->addAnimation(30, "Witch/Idle/bruja_000", 53, "Witch/DefaultMaterial_Base_Color.png");
    f->addAnimation(31, "Witch/Correr/bruja_000", 23, "Witch/DefaultMaterial_Base_Color.png");
    f->addAnimation(32, "Witch/Choque/bruja_000", 42, "Witch/DefaultMaterial_Base_Color.png");
    f->addAnimation(33, "Witch/LanzarItem/bruja_000", 56, "Witch/DefaultMaterial_Base_Color.png");
    f->addAnimation(34, "Witch/LanzarItemMovimiento/bruja_000", 39, "Witch/DefaultMaterial_Base_Color.png");
    f->addAnimation(35, "Witch/DerrapeDCHA/bruja_000", 81, "Witch/DefaultMaterial_Base_Color.png");
    f->addAnimation(36, "Witch/DerrapeIZQ/bruja_000", 78, "Witch/DefaultMaterial_Base_Color.png");

    std::cout << "Llega al final" << std::endl;
    
}


void multiAddStateChange(EventData eData) 
{

    if(GlobalVariables::getInstance().getGameState() == IGameState::stateType::MULTISELECTION && GlobalVariables::getInstance().getGameLoaded()){
        //Change state
        Game::getInstance().setState(IGameState::stateType::MULTIPREMATCH);
    }
}