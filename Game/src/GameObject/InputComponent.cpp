#include "InputComponent.h"
#include "../GameManager/ItemManager.h"
#include "../GameState/MatchState.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void advanceDownI(EventData eData); 
void advanceUpI(EventData eData); 
void brakeDownI(EventData eData); 
void brakeUpI(EventData eData); 
void turnLeftDownI(EventData eData); 
void turnLeftUpI(EventData eData); 
void turnRightDownI(EventData eData); 
void turnRightUpI(EventData eData); 
void jumpUpI(EventData eData);
void jumpDownI(EventData eData);
void driftUpI(EventData eData);
void driftDownI(EventData eData);
void useItemDownI(EventData eData);
void useAIDebug(EventData eData);
void useCameraDebug(EventData eData);
void useBehaviourDebug(EventData eData);
void slowControl(EventData eData);
void fastControl(EventData eData);
void normalControl(EventData eData);

//==============================================
// INPUT COMPONENT FUNCTIONS
//============================================== 

void InputComponent::init(){

    //Bind functions
    EventManager::getInstance().addListener(EventListener {EventType::Key_Advance_Down, advanceDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Advance_Up, advanceUpI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Brake_Down, brakeDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Brake_Up, brakeUpI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnLeft_Down, turnLeftDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnLeft_Up, turnLeftUpI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnRight_Down, turnRightDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnRight_Up, turnRightUpI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Jump_Down, jumpDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Jump_Up, jumpUpI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Drift_Down, driftDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Drift_Up, driftUpI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_UseItem_Down, useItemDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugAI_Down, useAIDebug});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugCamera_Down, useCameraDebug});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugBehaviour_Down, useBehaviourDebug});
    EventManager::getInstance().addListener(EventListener {EventType::Key_SlowControl_Down, slowControl});
    EventManager::getInstance().addListener(EventListener {EventType::Key_FastControl_Down, fastControl});
    EventManager::getInstance().addListener(EventListener {EventType::Key_NormalControl_Down, normalControl});

}

void InputComponent::update(float dTime){

}

void InputComponent::close(){

}

//==============================================
// DELEGATES
//==============================================
void advanceDownI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();

    float maxAcc = comp->getMovemententData().max_acc;
    float accInc = (eData.grade == -2) ? (maxAcc) : (maxAcc * eData.grade);

    comp->changeAccInc(accInc);
    comp->isMoving(true);
}
void advanceUpI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->changeAccInc(0.0);
    comp->isMoving(false);
}
void brakeDownI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->isBraking(true);
}
void brakeUpI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->isBraking(false);
}
void turnLeftDownI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();

    float spin = (eData.grade == -2) ? (0.1) : (0.1 * eData.grade);

    comp->changeSpinIncrement(spin);
    comp->isSpinning(true);
}
void turnLeftUpI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->changeSpinIncrement(0.0);
    comp->isSpinning(false);
}
void turnRightDownI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();

    float spin = (eData.grade == -2) ? (0.1) : (0.1 * eData.grade);

    comp->changeSpinIncrement(-spin);
    comp->isSpinning(true);
}
void turnRightUpI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->changeSpinIncrement(0.0);
    comp->isSpinning(false);
}
void jumpDownI(EventData eData){
    if(GlobalVariables::getInstance().getGameState() == IGameState::stateType::MATCH){
        auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
        comp->isJumping(true);
    }
}
void jumpUpI(EventData eData){
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->isJumping(false);
}
void driftDownI(EventData eData){
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->isDrifting(true);
}
void driftUpI(EventData eData){
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->isDrifting(false);
}

void useItemDownI(EventData eData){
    auto obj = InputManager::getInstance().getComponent().get()->getGameObject();
    ItemManager::getInstance().createItem(obj);
}
void useAIDebug(EventData eData){
    RenderManager::getInstance().renderAIDebug();
}
void useCameraDebug(EventData eData){
    RenderManager::getInstance().renderCameraDebug();
}
void useBehaviourDebug(EventData eData){
    RenderManager::getInstance().renderBattleDebug();
}
void slowControl(EventData eData){
    auto ratio = MatchState::getInstance().getRatio();
    if(ratio <= 1.0f)
    {
        MatchState::getInstance().setRatio(1.2);
    }
    else if(ratio == 1.2f)
    {
        MatchState::getInstance().setRatio(1.35);
    }
    else if(ratio == 1.35f)
    {
        MatchState::getInstance().setRatio(1.5);
    }
}
void fastControl(EventData eData){
    auto ratio = MatchState::getInstance().getRatio();
    if(ratio >= 1.0f)
    {
        MatchState::getInstance().setRatio(0.8);
    }
    else if(ratio == 0.8f)
    {
        MatchState::getInstance().setRatio(0.65);
    }
    else if(ratio == 0.65f)
    {
        MatchState::getInstance().setRatio(0.5);
    }
}
void normalControl(EventData eData){
    auto ratio = MatchState::getInstance().getRatio();
    if(ratio == 1.0f)
    {
        MatchState::getInstance().setRatio(0.0);
    }
    else if(ratio != 1.0f)
    {
        MatchState::getInstance().setRatio(1.0);
    }
}