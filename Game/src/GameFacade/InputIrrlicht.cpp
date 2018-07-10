#include "InputIrrlicht.h"

//Define macros
#define DetectKeyInput(TheKey,Event_Down,Event_Up) \
    if(event.KeyInput.PressedDown && event.KeyInput.Key == irr::EKEY_CODE::TheKey) { \
        if(!KeyIsDown[irr::EKEY_CODE::TheKey]){ \
            EventData eventD; \
            eventD.grade = -2; \
            EventManager::getInstance().addEvent(Event {EventType::Event_Down, eventD}); \
            KeyIsDown[irr::EKEY_CODE::TheKey] = true; \
        } \
    } \
    else { \
        if(KeyIsDown[irr::EKEY_CODE::TheKey] && event.KeyInput.Key == irr::EKEY_CODE::TheKey){ \
            EventData eventD; \
            eventD.grade = -2; \
            EventManager::getInstance().addEvent(Event {EventType::Event_Up, eventD}); \
            KeyIsDown[irr::EKEY_CODE::TheKey] = false; \
        } \
    };

void InputIrrlicht::openInput(uintptr_t dev) {

    device = reinterpret_cast<irr::IrrlichtDevice*>(dev);

}

void InputIrrlicht::updateInput() { 
    //Close game if not working
    if(!device->run()){
        EventManager::getInstance().addEvent(Event {EventType::Game_Close});
    }
}

void InputIrrlicht::closeInput() {
       
} 

bool InputIrrlicht::OnEvent(const irr::SEvent& event) {

    if(event.EventType == irr::EET_KEY_INPUT_EVENT){
        DetectKeyInput(KEY_SPACE, Key_Jump_Down, Key_Jump_Up)
        DetectKeyInput(KEY_ESCAPE,Key_Back_Down,Key_Back_Up)
        DetectKeyInput(KEY_KEY_W,Key_Advance_Down,Key_Advance_Up)
        DetectKeyInput(KEY_KEY_S,Key_Brake_Down,Key_Brake_Up)
        DetectKeyInput(KEY_KEY_A,Key_TurnLeft_Down,Key_TurnLeft_Up)
        DetectKeyInput(KEY_KEY_D,Key_TurnRight_Down,Key_TurnRight_Up)
        DetectKeyInput(KEY_KEY_T,Key_Drift_Down,Key_Drift_Up)
        DetectKeyInput(KEY_KEY_Q,Key_UseItem_Down,Key_UseItem_Up)
        DetectKeyInput(KEY_KEY_5,Key_Multiplayer_Down,Key_Multiplayer_Up)
        DetectKeyInput(KEY_KEY_6,Key_Singleplayer_Down,Key_Singleplayer_Up)
        DetectKeyInput(KEY_F5,Key_SlowControl_Down,Key_SlowControl_Up)
        DetectKeyInput(KEY_F6,Key_NormalControl_Down,Key_NormalControl_Up)
        DetectKeyInput(KEY_F7,Key_FastControl_Down,Key_FastControl_Up)
        DetectKeyInput(KEY_F8,Key_DebugNetwork_Down,Key_DebugNetwork_Up)
        DetectKeyInput(KEY_F9,Key_DebugAI_Down,Key_DebugAI_Up)
        DetectKeyInput(KEY_F10,Key_DebugBehaviour_Down,Key_DebugBehaviour_Up)
        DetectKeyInput(KEY_F11,Key_DebugCamera_Down,Key_DebugCamera_Up)
        DetectKeyInput(KEY_KEY_0,Key_Scheduling_Down,Key_Scheduling_Up)
    }
    if(event.EventType == irr::EET_MOUSE_INPUT_EVENT){
        switch(event.MouseInput.Event)
        {
        case irr::EMIE_LMOUSE_PRESSED_DOWN:
            EventManager::getInstance().addEvent(Event {EventType::Key_Select_Down});
            break;
        case irr::EMIE_LMOUSE_LEFT_UP:
            EventManager::getInstance().addEvent(Event {EventType::Key_Select_Up});
            break;
        case irr::EMIE_RMOUSE_PRESSED_DOWN:
            EventManager::getInstance().addEvent(Event {EventType::Key_Back_Down});
            break;
        case irr::EMIE_RMOUSE_LEFT_UP:
            EventManager::getInstance().addEvent(Event {EventType::Key_Back_Up});
            break;
        case irr::EMIE_MOUSE_MOVED:
            MouseState.Position.X = event.MouseInput.X;
            MouseState.Position.Y = event.MouseInput.Y;
            break;
        default: break;
        }
    }
    return false;
}