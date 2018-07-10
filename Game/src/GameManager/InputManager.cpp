#include "InputManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void selectedDown(EventData eData); 
void selectedUp(EventData eData); 
void backDown(EventData eData); 
void backUp(EventData eData); 
void advanceDown(EventData eData); 
void advanceUp(EventData eData); 
void brakeDown(EventData eData); 
void brakeUp(EventData eData); 
void turnLeftDown(EventData eData); 
void turnLeftUp(EventData eData); 
void turnRightDown(EventData eData); 
void turnRightUp(EventData eData); 
void jumpUp(EventData eData);
void jumpDown(EventData eData);
void driftUp(EventData eData);
void driftDown(EventData eData);
void useItemUp(EventData eData);
void useItemDown(EventData eData);
void debugAIDown(EventData eData);
void debugCameraDown(EventData eData);
void debugBehaviourDown(EventData eData);
void addInputComponent(EventData data);
void slowControlDown(EventData eData);
void fastControlDown(EventData eData);
void normalControlDown(EventData data);

//==============================================
// INPUT MANAGER FUNCTIONS
//============================================== 
InputManager& InputManager::getInstance() {
    static InputManager instance;
    return instance;
}

void InputManager::init(int engine){

    //InputFacade is already initialized in RenderIrrlicht
    //Important!!! Once we swich to our own render engine, we have to init input also from there
    inputFacade->openInput(device);
 
    //Bind functions
    EventManager::getInstance().addListener(EventListener {EventType::Key_Select_Down, selectedDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Select_Up, selectedUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Back_Down, backDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Back_Up, backUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Advance_Down, advanceDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Advance_Up, advanceUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Brake_Down, brakeDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Brake_Up, brakeUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnLeft_Down, turnLeftDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnLeft_Up, turnLeftUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnRight_Down, turnRightDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnRight_Up, turnRightUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Jump_Down, jumpUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Jump_Up, jumpDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Drift_Down, driftDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Drift_Up, driftUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_UseItem_Down, useItemDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_UseItem_Up, useItemUp});
    EventManager::getInstance().addListener(EventListener {EventType::InputComponent_Create, addInputComponent});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugAI_Down, debugAIDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugCamera_Down, debugCameraDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugCamera_Down, debugBehaviourDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_SlowControl_Down, slowControlDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_FastControl_Down, fastControlDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_NormalControl_Down, normalControlDown});
  
}

void InputManager::close(){

}

void InputManager::update(){

    inputFacade->updateInput();

}

IComponent::Pointer InputManager::createInputComponent(GameObject& newGameObject) {
    //Make shared pointer of input component
    IComponent::Pointer component = std::make_shared<InputComponent>(newGameObject);

    //Attach component to game object
    newGameObject.addComponent(component);

    //Send event of creation
    EventData data;
    data.Component = component;
    EventManager::getInstance().addEvent(Event {EventType::InputComponent_Create, data});

    return component;
}


//==============================================
// DELEGATES
//==============================================
void selectedDown(EventData eData){
    //std::cout << "Select button pressed!" << std::endl;
}
void selectedUp(EventData eData){
    //std::cout << "Select button released!" << std::endl;
}
void backDown(EventData eData){
    //std::cout << "Back button pressed!" << std::endl;
}
void backUp(EventData eData){
    //std::cout << "Back button released!" << std::endl;
}
void advanceDown(EventData eData) {
    //std::cout << "Advance button pressed!" << std::endl;
}
void advanceUp(EventData eData) {
    //std::cout << "Advance button released!" << std::endl;
}
void brakeDown(EventData eData) {
    //std::cout << "Brake button pressed!" << std::endl;
}
void brakeUp(EventData eData) {
    //std::cout << "Brake button released!" << std::endl;
}
void turnLeftDown(EventData eData) {
    //std::cout << "Left turn button pressed!" << std::endl;
}
void turnLeftUp(EventData eData) {
    //std::cout << "Left turn button released!" << std::endl;
}
void turnRightDown(EventData eData) {
    //std::cout << "Right turn button pressed!" << std::endl;
}
void turnRightUp(EventData eData) {
    //std::cout << "Right turn button released!" << std::endl;
}
void jumpDown(EventData eData){
    //std::cout << "Jump button pressed!" << std::endl;
}
void jumpUp(EventData eData){
    //std::cout << "Jump button released!" << std::endl;
}
void driftUp(EventData eData){
    //std::cout << "Drift button pressed!" << std::endl;
}
void driftDown(EventData eData){
    //std::cout << "Drift button released!" << std::endl;
}
void useItemDown(EventData eData){
    //std::cout << "Use item button pressed!" << std::endl;
}
void useItemUp(EventData eData){
    //std::cout << "Use item button released!" << std::endl;
}
void debugAIDown(EventData eData){
    //std::cout << "Use debug button pressed!" << std::endl;
}
void debugCameraDown(EventData eData){
    //std::cout << "Use debug button pressed!" << std::endl;
}
void debugBehaviourDown(EventData eData){
    //std::cout << "Use debug button pressed!" << std::endl;
}
void slowControlDown(EventData eData){
    //std::cout << "Use slow control button pressed!" << std::endl;
}
void fastControlDown(EventData eData){
    //std::cout << "Use fast control button pressed!" << std::endl;
}
void normalControlDown(EventData eData){
    //std::cout << "Use normal control button pressed!" << std::endl;
}
void addInputComponent(EventData data) {
    InputManager::getInstance().setComponent(data.Component);
    data.Component.get()->init();
}