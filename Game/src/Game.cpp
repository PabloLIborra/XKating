#include "Game.h"

//====================================================
//  DELEGATE DECLARATIONS
//====================================================
void setStateEvent(EventData eData);
void changeFullscreen(EventData eData);

//====================================================
//  GAME INITIALIZATION
//====================================================
void Game::init() {
    //Initial data set
    globalVariables = &GlobalVariables::getInstance(); //Initialize global variables bush
    globalVariables->setServer(false);                 //Not the server

    //Say game loop is active
    setStay(true);

    //Set engine to default
    setRenderEngine(1);
    setInputEngine(1);
    GlobalVariables::getInstance().setLanguage(1);

    audioManager    = &AudioManager::getInstance();     //Initialize true audio manager
    eventManager    = &EventManager::getInstance();     //Initilize event manager
    renderManager   = &RenderManager::getInstance();    //First we initialize renderManager, who creates a device and passes this reference to the inputManager
    inputManager    = &InputManager::getInstance();     //Once we've initialized the renderManager, we can do the same with our inputManager
    objectManager   = &ObjectManager::getInstance();    //Initialize Object manager
    physicsManager  = &PhysicsManager::getInstance();   //Initialize physics manager
    waypointManager = &WaypointManager::getInstance();  //Initialize Waypoint Manager 
    aiManager       = &AIManager::getInstance();        //Initialize AI manager
    sensorManager   = &SensorManager::getInstance();    //Initialize Sensor manager
    itemManager     = &ItemManager::getInstance();      //Initialize Item manager
    scoreManager    = &ScoreManager::getInstance();     //Initialize Score Manager
    networkManager  = &NetworkManager::getInstance();   //Initialize Network Manager
    debugManager    = &DebugManager::getInstance();     //Initialize Debug Manager

    //================================================================
    //INITIALIZE ALL MANAGERS
    //================================================================
    //Initialize true audio manager
    audioManager->init();

    //First we initialize renderManager, who creates a device and passes this reference to the inputManager
    renderManager->init(getRenderEngine());

    //Once we've initialized the renderManager, we can do the same with our inputManager
    inputManager->init(getInputEngine());

    //Initalize the rest
    objectManager->init();
    physicsManager->init();
    waypointManager->init();
    aiManager->init();
    sensorManager->init();
    itemManager->init();
    scoreManager->init();
    debugManager->init();

    //Initial state
    setState(IGameState::stateType::INTRO);

    //Change state listener
    EventManager::getInstance().addListener(EventListener {EventType::State_Change, setStateEvent});
    EventManager::getInstance().addListener(EventListener {EventType::Global_ChangeFullscreen, changeFullscreen});
}

//====================================================
//  GAME UPDATE
//====================================================
void Game::update(float dTime) {

}

//====================================================
//  GAME DRAW
//====================================================
void Game::draw() {
}

//====================================================
//  GAME CLOSE
//====================================================
void Game::close() {
    state->close();

    //Close all managers
    physicsManager->close();
    renderManager->close();
    inputManager->close();
    eventManager->close();
    waypointManager->close();
    aiManager->close();
    audioManager->close();
    sensorManager->close();
    itemManager->close();
    scoreManager->close();
    networkManager->close();
    debugManager->close();
}

//====================================================
//  GAME LOOP
//====================================================
//Client side
void Game::Run() {
    //Initialize game
    init();
    
    //Initialize timer
    clock = new Clock();
    clock->init();

    //execute game while staying
    while(stay){
        
        //Measure elapsed time
        elapsedTime = (float)clock->getElapsedTime();
        accumulatedTime += elapsedTime;
        clock->restart();
        
        if(dynamic_cast<MatchState*>(state) != nullptr) 
        {
            //If the state is Match, divide with ratio so we can accelerate or slow down the game
            //Update the game once every maxTime
            accumulatedTime /= ratio;
            state->update(accumulatedTime); 
        }
        else
        {
            //Update the game once every maxTime
            state->update(elapsedTime); 
        }

        //Always draw the game
        state->draw();
        
    }

    close();
}

//===============================================================
// ADDITIONAL FUNCTIONS
//===============================================================

//State setter
void Game::setState(IGameState::stateType type){
        //State changer
        switch(type){
            //////
            // set in global variables the type of game and keep the pointer here
            //////
            case IGameState::stateType::INTRO:
                globalVariables->setGameState(IntroState::getInstance().type);
                state = &IntroState::getInstance();
                break;
            case IGameState::stateType::CLIENTLOBBY:
                globalVariables->setGameState(ClientLobbyState::getInstance().type);
                state = &ClientLobbyState::getInstance();
                break;
            case IGameState::stateType::SELECTION:
                globalVariables->setGameState(SelectionState::getInstance().type);
                state = &SelectionState::getInstance();
                break;
            case IGameState::stateType::PREMATCH:
                globalVariables->setGameState(PreMatchState::getInstance().type);
                state = &PreMatchState::getInstance();
                EventManager::getInstance().addEvent(Event {EventType::Match_Start});
                break;
            case IGameState::stateType::MATCH:
                globalVariables->setGameState(MatchState::getInstance().type);
                state = &MatchState::getInstance();
                EventManager::getInstance().addEvent(Event {EventType::Match_Race_Start});
                break;
            case IGameState::stateType::POSTMATCH:
                globalVariables->setGameState(PostMatchState::getInstance().type);
                state = &PostMatchState::getInstance();
                EventManager::getInstance().addEvent(Event {EventType::Match_Race_End});
                break;
            case IGameState::stateType::MULTIMATCH:
                globalVariables->setGameState(MultiMatchState::getInstance().type);
                state = &MultiMatchState::getInstance();
                EventManager::getInstance().addEvent(Event {EventType::Match_Race_Start});
                break;
            case IGameState::stateType::MULTISELECTION:
                globalVariables->setGameState(MultiSelectionState::getInstance().type);
                state = &MultiSelectionState::getInstance();
                break;
            case IGameState::stateType::MULTIPREMATCH:
                globalVariables->setGameState(MultiPreMatchState::getInstance().type);
                state = &MultiPreMatchState::getInstance();
                EventManager::getInstance().addEvent(Event {EventType::Match_Start});
                break;
            case IGameState::stateType::MULTIPOSTMATCH:
                globalVariables->setGameState(MultiPostMatchState::getInstance().type);
                state = &MultiPostMatchState::getInstance();
                EventManager::getInstance().addEvent(Event {EventType::Match_Race_End});
                break;
            case IGameState::stateType::PAUSE:
                globalVariables->setGameState(PauseState::getInstance().type);
                state = &PauseState::getInstance();
                break;
            default:
                globalVariables->setGameState(IntroState::getInstance().type);
                state = &IntroState::getInstance();
                break;
        }
        //Initialize state here
        state->init();
}

std::vector<std::string> Game::split(const std::string& s, const char& c) {
	std::string buff{""};
	std::vector<std::string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

void Game::loadMap() {

    using namespace rapidxml;

    xml_document<> doc;
    xml_node<> * root_node;

    //Read the file and put it into a char array
    std::ifstream theFile("media/xml/circuit.xml");
	std::string buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);

    // Find our root node
	root_node = doc.first_node("object");

    // Iterate over the gameObjects, reading them
    for (xml_node<> * object = root_node; object; object = object->next_sibling()) {

        uint16_t id;
        GameObject::TransformationData transform;

        //Read ID from XML
        id = (uint16_t) std::stoi(object->first_attribute("id")->value());
        //Read POSITION from XML
        auto strVector = split(object->first_attribute("pos")->value(), ',');
        transform.position = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
        //Read ROTATION from XML
        strVector = split(object->first_attribute("rot")->value(), ',');
        transform.rotation = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
        //Read SCALE from XML
        strVector = split(object->first_attribute("sca")->value(), ',');
        transform.scale = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));

        //Create new OBJECT
        auto obj = ObjectManager::getInstance().createObject(id, transform);

        //Parse components
        xml_node<> * root_component = object->first_node("component");

        for (xml_node<> * component = root_component; component; component = component->next_sibling()){

            //Parse TERRAIN component
            if(strcmp(component->first_attribute("name")->value(),"terrain") == 0){

                xml_node<>* bbox = component->first_node("bbox");

                LAPAL::plane3f terrain;
                //Read P1 from XML
                auto strVector = split(bbox->first_attribute("p1")->value(), ',');
                terrain.p1 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                //Read P2 from XML
                strVector = split(bbox->first_attribute("p2")->value(), ',');
                terrain.p2 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                //Read P3 from XML
                strVector = split(bbox->first_attribute("p3")->value(), ',');
                terrain.p3 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                //Read P4 from XML
                strVector = split(bbox->first_attribute("p4")->value(), ',');
                terrain.p4 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                //Read FRICTION from XML
                terrain.fric = std::stof(bbox->first_attribute("friction")->value());
                //Calculate terrain angles in X and Z
                LAPAL::calculateRotationsXZ(terrain);

                //Create TERRAIN component
                PhysicsManager::getInstance().createTerrainComponent(*obj.get(), terrain);

            }

            //Parse WAYPOINT component
            if(strcmp(component->first_attribute("name")->value(),"waypoint") == 0){
                
                float radius = std::stof(component->first_attribute("radius")->value());
                int level = std::stoi(component->first_attribute("level")->value());

                //Create TERRAIN component
                WaypointManager::getInstance().createWaypointComponent(obj, radius, level);

            }

            //Parse LIGHT component
            if(strcmp(component->first_attribute("name")->value(),"light") == 0){
                
                float radius = std::stof(component->first_attribute("radius")->value());
                
                LightRenderComponent::Type type;

                if(component->first_attribute("type")->value()[0] == 'P')
                    type = LightRenderComponent::Type::Point;
                if(component->first_attribute("type")->value()[0] == 'D')
                    type = LightRenderComponent::Type::Directional;

                //Create LIGHT component
                RenderManager::getInstance().createLightRenderComponent(*obj.get(),type,radius);

            }

            //Parse ITEMBOX component
            if(strcmp(component->first_attribute("name")->value(),"itemBox") == 0){
                
                //Create TERRAIN component
                ItemManager::getInstance().createItemBox(*obj.get());
            }

            //Parse COLLISION component
            if(strcmp(component->first_attribute("name")->value(),"collision") == 0){

                xml_node<>* bbox = component->first_node("bbox");
                LAPAL::plane3f terrain;
                float radius, height;
                bool kinetic;
                CollisionComponent::Type type = CollisionComponent::Type::Default;

                //Parse Terrain if given
                if(bbox != nullptr) {
                    //Read P1 from XML
                    auto strVector = split(bbox->first_attribute("p1")->value(), ',');
                    terrain.p1 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                    //Read P2 from XML
                    strVector = split(bbox->first_attribute("p2")->value(), ',');
                    terrain.p2 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                    //Read P3 from XML
                    strVector = split(bbox->first_attribute("p3")->value(), ',');
                    terrain.p3 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                    //Read P4 from XML
                    strVector = split(bbox->first_attribute("p4")->value(), ',');
                    terrain.p4 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                    //Read FRICTION from XML
                    terrain.fric = std::stof(bbox->first_attribute("friction")->value());
                    //Calculate terrain angles in X and Z
                    LAPAL::calculateRotationsXZ(terrain);
                }

                //Parse Radius if given
                if(component->first_attribute("radius") != nullptr)
                    radius = std::stof(component->first_attribute("radius")->value());

                //Parse Height if given
                if(component->first_attribute("height") != nullptr)
                    height = std::stof(component->first_attribute("height")->value());

                //Parse Kinetic
                if(strcmp(component->first_attribute("kinetic")->value(),"true") == 0)
                    kinetic = true;
                else 
                    kinetic = false;

                //Parse Type
                if(strcmp(component->first_attribute("type")->value(),"ramp") == 0)
                    type = CollisionComponent::Type::Ramp;
                if(strcmp(component->first_attribute("type")->value(),"start") == 0)
                    type = CollisionComponent::Type::StartLine;


                //Create COLLISION component
                if(bbox != nullptr)
                    PhysicsManager::getInstance().createCollisionComponent(*obj.get(), terrain, kinetic, height, type);
                else
                    PhysicsManager::getInstance().createCollisionComponent(*obj.get(), radius, height, kinetic, type);

            }

            //Parse RAMP component
            if(strcmp(component->first_attribute("name")->value(),"ramp") == 0){
                
                float vel = std::stof(component->first_attribute("vel")->value());
                float cTime = std::stof(component->first_attribute("cTime")->value());
                float dTime = std::stof(component->first_attribute("dTime")->value());

                //Create RAMP component
                PhysicsManager::getInstance().createRampComponent(*obj.get(), vel, cTime, dTime);

            }

            //Parse RENDER component
            if(strcmp(component->first_attribute("name")->value(),"render") == 0){

                ObjectRenderComponent::Shape shape;

                if(strcmp(component->first_attribute("type")->value(),"plane") == 0){
                    shape = ObjectRenderComponent::Shape::Plane;
                }
                if(strcmp(component->first_attribute("type")->value(),"mesh") == 0){
                    shape = ObjectRenderComponent::Shape::Mesh;
                }
                if(strcmp(component->first_attribute("type")->value(),"sphere") == 0){
                    shape = ObjectRenderComponent::Shape::Sphere;
                }
                if(strcmp(component->first_attribute("type")->value(),"cube") == 0){
                    shape = ObjectRenderComponent::Shape::Cube;
                }
                
                //Create RENDER component
                RenderManager::getInstance().createObjectRenderComponent(*obj.get(), shape, component->first_attribute("file")->value());

            }
        }
	}

    //Update every thing that has been created
    EventManager::getInstance().update();

    //Loop over terrain components, linking them
    for (xml_node<> * object = root_node; object; object = object->next_sibling()) {

        //Get current object ID
        uint16_t id = (uint16_t) std::stoi(object->first_attribute("id")->value());

        //Parse components
        xml_node<> * root_component = object->first_node("component");

        for (xml_node<> * component = root_component; component; component = component->next_sibling()){

            //Parse TERRAIN components
            if(strcmp(component->first_attribute("name")->value(),"terrain") == 0){

                //Get our component
                std::shared_ptr<TerrainComponent> cmp = ObjectManager::getInstance().getObject(id).get()->getComponent<TerrainComponent>();

                //If not null, connect our component with next object
                uint16_t connectID = (uint16_t) std::stoi(split(component->first_attribute("l0")->value(), ':')[0]);

                if( connectID > 65000 )
                    cmp.get()->setNext(nullptr);
                else 
                    cmp.get()->setNext(ObjectManager::getInstance().getObject(connectID).get()->getComponent<TerrainComponent>());

                //If not null, connect our component with right object
                connectID = (uint16_t) std::stoi(split(component->first_attribute("l1")->value(), ':')[0]);

                if( connectID > 65000 )
                    cmp.get()->setRight(nullptr);
                else 
                    cmp.get()->setRight(ObjectManager::getInstance().getObject(connectID).get()->getComponent<TerrainComponent>());

                //If not null, connect our component with prev object
                connectID = (uint16_t) std::stoi(split(component->first_attribute("l2")->value(), ':')[0]);

                if( connectID > 65000 )
                    cmp.get()->setPrev(nullptr);
                else 
                    cmp.get()->setPrev(ObjectManager::getInstance().getObject(connectID).get()->getComponent<TerrainComponent>());

                //If not null, connect our component with left object
                connectID = (uint16_t) std::stoi(split(component->first_attribute("l3")->value(), ':')[0]);

                if( connectID > 65000 )
                    cmp.get()->setLeft(nullptr);
                else 
                    cmp.get()->setLeft(ObjectManager::getInstance().getObject(connectID).get()->getComponent<TerrainComponent>());

            }
        }
    }

    //Update every thing that has been created
    EventManager::getInstance().update();

}

//====================================================
//  DELEGATE FUNCTIONS
//====================================================
//Functions that create or destroy objects
void setStateEvent(EventData eData)
{
    Game::getInstance().setState((IGameState::stateType) eData.Id);
}

void changeFullscreen(EventData eData) {

    if(GlobalVariables::getInstance().getFullscreen()) {

        GlobalVariables::getInstance().setFullscreen(false);
        SDL_Window* w = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice()->getWindow();

        int width, height;

        SDL_GetWindowSize(w,&width,&height); 

        SDL_SetWindowFullscreen(w, 0);
        
        SDL_SetWindowSize(w, width*0.8, height*0.8);
             
        SDL_SetWindowPosition(w, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

        RenderManager::getInstance().getRenderFacade()->getWindow().size.x = width*0.8;
        RenderManager::getInstance().getRenderFacade()->getWindow().size.y = height*0.8;

    }
    else {
        
        GlobalVariables::getInstance().setFullscreen(true);
        SDL_Window* w = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice()->getWindow();

        int width, height;
        
        SDL_SetWindowFullscreen(w, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_GetWindowSize(w,&width,&height);      

        RenderManager::getInstance().getRenderFacade()->getWindow().size.x = width;
        RenderManager::getInstance().getRenderFacade()->getWindow().size.y = height;
    }

}