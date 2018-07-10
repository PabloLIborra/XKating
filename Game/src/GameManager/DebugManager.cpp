#include "DebugManager.h"

//////////////////////////////////////////////
//            Possible upgrades
//////////////////////////////////////////////
/*
>Add lists of players connected, their server_id and game_id
>Add lists of functions that each sends and receives
>Add lists of calls and green in last server call
*/
//////////////////////////////////////////////

//==============================================
// DELEGATE NAMESPACING
//==============================================
void swapDebugAI(EventData eData);
void swapDebugBehaviour(EventData eData);
void swapDebugCamera(EventData eData);
void swapDebugNetwork(EventData eData);

//==============================================
// MAIN FUNCTIONS
//==============================================
void DebugManager::init(){
    //Inital debugging is false
    debugAI        = false;
    debugBehaviour = false;
    debugCamera    = false;
    debugNetwork   = false;

    //Connection to the network manager (continuous access)
    networkManager = &NetworkManager::getInstance();
    renderManager  = &RenderManager::getInstance();
    objectManager  = &ObjectManager::getInstance();

    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugAI_Down,swapDebugAI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugBehaviour_Down, swapDebugBehaviour});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugCamera_Down,swapDebugCamera});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugNetwork_Down,swapDebugNetwork});
}

void DebugManager::update(){
    //If a checker is on, update the debug given
    if(debugAI){
        updateDebugAI();
    }

    if(debugBehaviour){
        updateDebugBehaviour();
    }

    if(debugCamera){
        updateDebugCamera();
    }

    if(debugNetwork){
        updateDebugNetwork();
    }
}

void DebugManager::close(){
    //Clean all debugs
    cleanDebugAI();
    cleanDebugBehaviour();
    cleanDebugCamera();
    cleanDebugNetwork();
}

//==============================================
// INITIALIZE DEBUGS
//==============================================

void DebugManager::initDebugAI()
{

}

void DebugManager::initDebugBehaviour()
{

}

void DebugManager::initDebugCamera()
{

}

void DebugManager::initDebugNetwork()
{

    //Clean lists to prevent collapsed messages
    cleanDebugNetworkLists();

    //Auxiliar data
    int32_t auxIdText;
    int widthText = 250;
    int separationText = 20;
    int leftOffset = 5;
    int actualHeight = 10;
    int textr = 0, textg = 0, textb = 0, textalpha = 255;
    std::string textFont("media/font/Razor_m.png");

    //Last message received board illuminated and value
    idLastMessageBoard = renderManager->createRectangleColor(glm::vec2(0,0),glm::vec2(widthText * 2+10,400), 255, 255, 255, 200);

    //Generate message and push its id to the list, then generate spacing down below
    //------------------------------------------------------------------------------
    auxIdText = renderManager->createText("Last received server message", glm::vec2(leftOffset,actualHeight), textr-20, textg-255, textb-255, textalpha, glm::vec2(widthText * 2,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);

    actualHeight += separationText+5;
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    auxIdText = renderManager->createText("Player created: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);

    auxIdText = renderManager->createText("", glm::vec2(leftOffset+widthText,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastDataTexts.push_back(auxIdText);

    actualHeight += separationText;
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    auxIdText = renderManager->createText("Box collision: ", glm::vec2(leftOffset,actualHeight),textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);

    auxIdText = renderManager->createText("", glm::vec2(leftOffset+widthText,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastDataTexts.push_back(auxIdText);
    
    actualHeight += separationText;
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    auxIdText = renderManager->createText("Trap created: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);

    auxIdText = renderManager->createText("", glm::vec2(leftOffset+widthText,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastDataTexts.push_back(auxIdText);

    actualHeight += separationText;
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    auxIdText = renderManager->createText("Trap destroyed: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);

    auxIdText = renderManager->createText("", glm::vec2(leftOffset+widthText,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastDataTexts.push_back(auxIdText);

    actualHeight += separationText;
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    auxIdText = renderManager->createText("Tire created: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);

    auxIdText = renderManager->createText("", glm::vec2(leftOffset+widthText,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastDataTexts.push_back(auxIdText);
    
    actualHeight += separationText;
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    auxIdText = renderManager->createText("Tire destroyed: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);

    auxIdText = renderManager->createText("", glm::vec2(leftOffset+widthText,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastDataTexts.push_back(auxIdText);

    actualHeight += separationText;
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    auxIdText = renderManager->createText("Bomb created: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);

    auxIdText = renderManager->createText("", glm::vec2(leftOffset+widthText,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastDataTexts.push_back(auxIdText);

    actualHeight += separationText;
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    auxIdText = renderManager->createText("Bomb destroyed: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);

    auxIdText = renderManager->createText("", glm::vec2(leftOffset+widthText,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastDataTexts.push_back(auxIdText);

    actualHeight += separationText+5;
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    auxIdText = renderManager->createText("Last sender: ", glm::vec2(leftOffset,actualHeight), textr-200, textg-20, textb-100, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);

    auxIdText = renderManager->createText("", glm::vec2(leftOffset+widthText,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastDataTexts.push_back(auxIdText);
    //------------------------------------------------------------------------------

    //Set a default value to the last id message
    idLastMessageText = auxIdText;

    //Each data for each player: system address, server_id actual position
    for(unsigned int i = 0; i < networkManager->getRemotePlayerComponentList().size(); i++){
        //Auxiliar variables
        int id       = 31000+i; //for every player, generate an ID of the network tracker (2 of them)
        int id2      = 31000+i+networkManager->getRemotePlayerComponentList().size();

        float height = 5.f;     //Height of the cylinder
        float rad    = 10.f;    //Radius of the cylinder

        //Get initial position
        GameObject::TransformationData point = networkManager->getRemotePlayerComponentList()[i]->getGameObject().getTransformData();
        
        //Clean the transforms
        GameObject::TransformationData transform;
        transform.position = point.position;
        transform.rotation = glm::vec3(0, 0, 3.141592653589f);
        transform.scale    = glm::vec3(1, 1, 1);

        GameObject::TransformationData transform2;
        transform2.position = point.position;
        transform2.rotation = glm::vec3(0, 0, 3.141592653589f);
        transform2.scale    = glm::vec3(1, 1, 1);

        //Create cylinders
        GameObject::Pointer collisionCylinder = ObjectManager::getInstance().createObject(id, transform);
        GameObject::Pointer collisionCylinder2 = ObjectManager::getInstance().createObject(id2, transform);

        //Create render component
        RenderManager::getInstance().createObjectRenderComponent(*collisionCylinder.get(), ObjectRenderComponent::Shape::Cylinder, "whiteWithTransparency.png", rad, height, 10.f, true);
        RenderManager::getInstance().createObjectRenderComponent(*collisionCylinder2.get(), ObjectRenderComponent::Shape::Cylinder, "whiteWithTransparency.png", rad, height, 10.f, true);
        
        //Create tracker component
        auto objective = std::dynamic_pointer_cast<RemotePlayerComponent>(networkManager->getRemotePlayerComponentList()[i]);
        createTrackerDNComponent(*collisionCylinder.get(), objective.get()->getServerId(), 'a');
        createTrackerDNComponent(*collisionCylinder2.get(), objective.get()->getServerId(), 'b');

        //Update list of ids
        idCylynderDN.push_back(id);
        idCylynderDN.push_back(id2);
    }

    //Each object on the map data: player creator (server_id), actual position 

    //Initialize debug state in network manager
    networkManager->setDebugNetworkState(true);

}

//==============================================
// CLEAN DEBUGS
//==============================================

void DebugManager::cleanDebugAI()
{
    renderManager->cleanVI();
}

void DebugManager::cleanDebugBehaviour()
{
    renderManager->cleanVI();
}

void DebugManager::cleanDebugCamera()
{
    renderManager->cleanVI();
}

void DebugManager::cleanDebugNetwork()
{
    //Clean interface
    renderManager->cleanVI();

    //Set network to false
    networkManager->setDebugNetworkState(false);

    //Delete trackers
    for(unsigned int i = 0; i < idCylynderDN.size(); i++){
        //Delete marker
        EventData data;
        data.Id = idCylynderDN[i];
        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
    }

    //Clean lists
    cleanDebugNetworkLists();
}

void DebugManager::cleanDebugNetworkLists()
{
    //Free network lists
    networkManager->getLastPackets()->clear();
    networkManager->getLastData()->clear();
    networkManager->getLastSenders()->clear();

    //Clear internal lists
    idLastDataTexts.clear();
    idCylynderDN.clear();         
    idLastMessageTexts.clear();   
    idLastDataTexts.clear();      
}

//==============================================
// UPDATE DEBUGS
//==============================================

void DebugManager::updateDebugAI(){
}

void DebugManager::updateDebugBehaviour(){
}

void DebugManager::updateDebugCamera(){
}

void DebugManager::updateDebugNetwork(){
    //Checks if player is connected to a server
    if(networkManager->getConnected()){
        //Match debug
        if(networkManager->getStarted()){
            //Process all packets
            while(!networkManager->getLastPackets()->empty()){
                //Get last data and load it at string, then remove it from the list
                std::string lastDataString = networkManager->getLastData()->front();
                networkManager->getLastData()->pop_front();

                //Get last sender and show it
                int lastSender = networkManager->getLastSenders()->front();  //Get it from the list
                networkManager->getLastSenders()->pop_front();               //Erase from list
                std::string lastSenderString = std::to_string(lastSender);   //transform into string
                updateNetworkText(idLastDataTexts.at(8), lastSenderString);  //Show it

                //Get first packet to process and remove it from the list
                customMessages lastPacket = networkManager->getLastPackets()->front();
                networkManager->getLastPackets()->pop_front();

                switch(lastPacket){
                    //Game related
                    case ID_GAME_ENDED:
                        std::cout<<"Game ended."<<std::endl;
                        break;

                    //Player related
                    case ID_CREATE_PLAYER:
                        setActiveLastMessage(idLastMessageTexts.at(1));
                        updateNetworkText(idLastDataTexts.at(0), lastDataString);
                        break;
                        
                    case ID_CREATE_REMOTE_PLAYER:
                        setActiveLastMessage(idLastMessageTexts.at(1));
                        updateNetworkText(idLastDataTexts.at(0), lastDataString);
                        break;

                    case ID_PLAYERS_POSITION:
                        updateCylinderDN(lastSender);
                        break;

                    //Item related
                    case ID_BOX_COLLISION:
                        setActiveLastMessage(idLastMessageTexts.at(2));
                        updateNetworkText(idLastDataTexts.at(1), lastDataString);
                        break;
                    
                    case ID_CREATE_TRAP:
                        setActiveLastMessage(idLastMessageTexts.at(3));
                        updateNetworkText(idLastDataTexts.at(2), lastDataString);
                        break;

                    case ID_DESTROY_TRAP:
                        setActiveLastMessage(idLastMessageTexts.at(4));
                        updateNetworkText(idLastDataTexts.at(3), lastDataString);
                        break;
                    
                    case ID_CREATE_RED_SHELL:
                        setActiveLastMessage(idLastMessageTexts.at(5));
                        updateNetworkText(idLastDataTexts.at(4), lastDataString);
                        break;

                    case ID_DESTROY_RED_SHELL:
                        setActiveLastMessage(idLastMessageTexts.at(6));
                        updateNetworkText(idLastDataTexts.at(5), lastDataString);
                        break;

                    case ID_RED_SHELLS_POSITION:
                        break;

                    case ID_CREATE_BLUE_SHELL:
                        setActiveLastMessage(idLastMessageTexts.at(7));
                        updateNetworkText(idLastDataTexts.at(6), lastDataString);
                        break;
                    
                    case ID_DESTROY_BLUE_SHELL:
                        setActiveLastMessage(idLastMessageTexts.at(8));
                        updateNetworkText(idLastDataTexts.at(7), lastDataString);
                        break;

                    case ID_BLUE_SHELLS_POSITION:
                        break;

                    default:
                        break;
                }
            }

        }
    }
}

//==============================================
// NETWORK PARTICULAR FUNCTIONS
//==============================================
void DebugManager::setActiveLastMessage(uint32_t id)
{
    //Change color of last message to white
    renderManager->changeBackgroundColorText( idLastMessageText, 255,255,255,255);

    //Change color of new message to green
    renderManager->changeBackgroundColorText( id, 0,255,0,255);
    idLastMessageText = id;
}

void DebugManager::updateNetworkText(uint32_t id, std::string text){
    //Change text of the message with the given id
    renderManager->changeText( id, text);
}

void DebugManager::updateCylinderDN(int id){
    //Update the position of the trackers
    for(unsigned int i = 0; i < trackerDNComponentList.size(); i++){
        std::shared_ptr<RemoteItemComponent> cyl = std::dynamic_pointer_cast<RemoteItemComponent>(networkManager->getRemotePlayerComponentList()[i]);
    }
}

//==============================================
// MANAGING COMPONENTS
//==============================================

IComponent::Pointer DebugManager::createTrackerDNComponent(GameObject& newGameObject, int server_id, char type){
    //make component
    IComponent::Pointer component = std::make_shared<TrackerDNComponent>(newGameObject, server_id, type);

    //Add to object
    newGameObject.addComponent(component);

    //Add to list
    trackerDNComponentList.push_back(component);

    //Send creation event
    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::TrackerDNComponent_Create, data});

    return component;
}


//==============================================
// DELEGATE FUNCTIONS
//==============================================
//Swaps AI debug state
void swapDebugAI(EventData eData)
{
    DebugManager::getInstance().setDebugAI(!DebugManager::getInstance().getDebugAI());

    //See if it shoulde be initialized or closed
    if(DebugManager::getInstance().getDebugAI()){
        //Clean rest of debugs
        DebugManager::getInstance().cleanDebugCamera();
        DebugManager::getInstance().cleanDebugBehaviour();
        DebugManager::getInstance().cleanDebugNetwork();

        DebugManager::getInstance().initDebugAI();

        //Change to debug state
        RenderManager::getInstance().setDebugState(true);
    }else{
        DebugManager::getInstance().cleanDebugAI();

        //Close normal state
        RenderManager::getInstance().setDebugState(false);
    }

    RenderManager::getInstance().renderAIDebug();
}

//Swaps Battle Behaviour debug state
void swapDebugBehaviour(EventData eData){
    DebugManager::getInstance().setDebugBehaviour(!DebugManager::getInstance().getDebugBehaviour());

    //See if it shoulde be initialized or closed
    if(DebugManager::getInstance().getDebugBehaviour()){
        //Clean rest of debugs
        DebugManager::getInstance().cleanDebugCamera();
        DebugManager::getInstance().cleanDebugNetwork();
        DebugManager::getInstance().cleanDebugAI();

        DebugManager::getInstance().initDebugBehaviour();

        //Change to debug state
        RenderManager::getInstance().setDebugState(true);

    }else{
        DebugManager::getInstance().cleanDebugBehaviour();

        //Change to normal state
        RenderManager::getInstance().setDebugState(false);
    }

    RenderManager::getInstance().renderBattleDebug();
}

//Swaps Camera debug state
void swapDebugCamera(EventData eData)
{
    DebugManager::getInstance().setDebugCamera(!DebugManager::getInstance().getDebugCamera());

    //See if it shoulde be initialized or closed
    if(DebugManager::getInstance().getDebugCamera()){
        //Clean rest of debugs
        DebugManager::getInstance().cleanDebugNetwork();
        DebugManager::getInstance().cleanDebugBehaviour();
        DebugManager::getInstance().cleanDebugAI();

        DebugManager::getInstance().initDebugCamera();

        //Change to debug state
        RenderManager::getInstance().setDebugState(true);
    }else{
        DebugManager::getInstance().cleanDebugCamera();

        //Change to normal state
        RenderManager::getInstance().setDebugState(false);
    }

    RenderManager::getInstance().renderCameraDebug();
}

//Swap Network debug state
void swapDebugNetwork(EventData eData){
    DebugManager::getInstance().setDebugNetwork(!DebugManager::getInstance().getDebugNetwork());
    
    //See if it should be initialized or closed
    if(DebugManager::getInstance().getDebugNetwork()){
        //Clean rest of debugs
        DebugManager::getInstance().cleanDebugCamera();
        DebugManager::getInstance().cleanDebugBehaviour();
        DebugManager::getInstance().cleanDebugAI();

        DebugManager::getInstance().initDebugNetwork();
    }else{
        DebugManager::getInstance().cleanDebugNetwork();
    }
}
