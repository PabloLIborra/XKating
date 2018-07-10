#include "InputRedPanda.h"
#include "../GlobalVariables.h"
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_sdl_gl3.h>

//Define macros
#define DetectKeyInput(TheKey,Event_Down,Event_Up,Mapping) \
    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_##TheKey && keyMapping[Mapping] == false) { \
            EventData eventD; \
            eventD.grade = -2; \
            EventManager::getInstance().addEvent(Event {EventType::Event_Down, eventD}); \
            EventManager::getInstance().addEvent(Event {EventType::Key_Pressed}); \
            keyMapping[Mapping] = true; \
    } \
    else if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_##TheKey){ \
            EventData eventD; \
            eventD.grade = -2; \
            EventManager::getInstance().addEvent(Event {EventType::Event_Up, eventD}); \
            keyMapping[Mapping] = false; \
    };


#define DetectButtonInput(TheKey,Event_Down,Event_Up,Mapping) \
    if(SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_##TheKey)) { \
            EventData eventD; \
            eventD.grade = -2; \
            EventManager::getInstance().addEvent(Event {EventType::Event_Down, eventD}); \
            EventManager::getInstance().addEvent(Event {EventType::Key_Pressed}); \
            buttonMapping[Mapping] = true; \
    } \
    else if(buttonMapping[Mapping]){ \
            EventData eventD; \
            eventD.grade = -2; \
            EventManager::getInstance().addEvent(Event {EventType::Event_Up, eventD}); \
            buttonMapping[Mapping] = false; \
    };



void InputRedPanda::openInput(uintptr_t dev) {
    device = reinterpret_cast<SDL_Window*>(dev);
    
    if(SDL_NumJoysticks() > 0)
        gamepad = SDL_GameControllerOpen(0);

}
//<___
struct nk_context *inputGUI; //:::> global variable
//___>

void InputRedPanda::updateInput() { 


    // Process events
    SDL_Event event;    
    //<___
    nk_input_begin(inputGUI);
    //___>
    while( SDL_PollEvent( &event ) )
    {
        //Update GUI input
        //<___
        nk_sdl_handle_event(&event);
        //___>
        DetectKeyInput(ESCAPE,Idle,Game_Pause,1)

        if(!GlobalVariables::getInstance().getIgnoreInput()){

            //Update input 
            DetectKeyInput(SPACE, Key_Jump_Down, Key_Jump_Up,0)
            DetectKeyInput(w,Key_Advance_Down,Key_Advance_Up,2)
            DetectKeyInput(s,Key_Brake_Down,Key_Brake_Up,3)
            DetectKeyInput(a,Key_TurnLeft_Down,Key_TurnLeft_Up,4)
            DetectKeyInput(d,Key_TurnRight_Down,Key_TurnRight_Up,5)
            DetectKeyInput(t,Key_Drift_Down,Key_Drift_Up,6)
            DetectKeyInput(q,Key_UseItem_Down,Key_UseItem_Up,7)
            DetectKeyInput(F6,Key_SlowControl_Down,Key_SlowControl_Up,8)
            DetectKeyInput(F7,Key_NormalControl_Down,Key_NormalControl_Up,9)
            DetectKeyInput(F8,Key_FastControl_Down,Key_FastControl_Up,10)
            DetectKeyInput(F9,Key_DebugAI_Down,Key_DebugAI_Up,11)
            DetectKeyInput(F10,Key_DebugBehaviour_Down,Key_DebugBehaviour_Up,12)
            DetectKeyInput(F11,Key_DebugCamera_Down,Key_DebugCamera_Up,13)
            DetectKeyInput(0,Key_Scheduling_Down,Key_Scheduling_Up,14)
            DetectKeyInput(F5,Idle,Global_ChangeFullscreen,15)

            DetectButtonInput(A, Key_Jump_Down, Key_Jump_Up, 0)
            DetectButtonInput(B, Key_Drift_Down, Key_Drift_Up, 1)
            DetectButtonInput(Y, Key_UseItem_Down, Key_UseItem_Up, 2)

            DetectAxisInput();
        }

        //Exit game
        if (event.type == SDL_QUIT)
            EventManager::getInstance().addEvent(Event {EventType::Game_Close});
    }
    //<___
    nk_input_end(inputGUI);

    if(gamepad == nullptr && SDL_NumJoysticks() > 0) {
        gamepad = SDL_GameControllerOpen(0);
    }
    if(SDL_NumJoysticks() == 0)
        gamepad = nullptr;
}

void InputRedPanda::closeInput() {

    SDL_GameControllerClose(gamepad);
       
} 

void InputRedPanda::setGUIContext(void* ctx) {
    //<___
    inputGUI = (nk_context*)ctx;
}

void InputRedPanda::DetectAxisInput() {

    if(gamepad) {

        int StickX = SDL_GameControllerGetAxis(gamepad, SDL_CONTROLLER_AXIS_LEFTX);
        int TriggerL = SDL_GameControllerGetAxis(gamepad, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
        int TriggerR = SDL_GameControllerGetAxis(gamepad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

        if(StickX > 9000){
            EventData eventData;
            eventData.grade = StickX/32000;
            Event event;
            event.type = EventType::Key_TurnRight_Down;
            event.data = eventData; 
            EventManager::getInstance().addEvent(event); 
            buttonMapping[3] = true; 
        }
        else if(StickX > 0 && buttonMapping[3]){
            EventData eventData;
            eventData.grade = -2;
            Event event;
            event.type = EventType::Key_TurnRight_Up;
            event.data = eventData; 
            EventManager::getInstance().addEvent(event); 
            buttonMapping[3] = false; 
        }

        if(StickX < -9000){
            EventData eventData;
            eventData.grade = -StickX/32000;
            Event event;
            event.type = EventType::Key_TurnLeft_Down;
            event.data = eventData; 
            EventManager::getInstance().addEvent(event); 
            buttonMapping[4] = true; 
        }
        else if(StickX < 0 && buttonMapping[4]){
            EventData eventData;
            eventData.grade = -2;
            Event event;
            event.type = EventType::Key_TurnLeft_Up;
            event.data = eventData; 
            EventManager::getInstance().addEvent(event); 
            buttonMapping[4] = false; 
        }

        if(TriggerL > 12000){
            EventData eventData;
            eventData.grade = -2;
            Event event;
            event.type = EventType::Key_Brake_Down;
            event.data = eventData; 
            EventManager::getInstance().addEvent(event); 
            buttonMapping[5] = true; 
        }
        else if(buttonMapping[5]){
            EventData eventData;
            eventData.grade = -2;
            Event event;
            event.type = EventType::Key_Brake_Up;
            event.data = eventData; 
            EventManager::getInstance().addEvent(event); 
            buttonMapping[5] = false; 
        }

        if(TriggerR > 3000){
            EventData eventData;
            eventData.grade = TriggerR/32000;
            Event event;
            event.type = EventType::Key_Advance_Down;
            event.data = eventData; 
            EventManager::getInstance().addEvent(event); 
            buttonMapping[6] = true; 
        }
        else if(buttonMapping[6]){
            EventData eventData;
            eventData.grade = -2;
            Event event;
            event.type = EventType::Key_Advance_Up;
            event.data = eventData; 
            EventManager::getInstance().addEvent(event); 
            buttonMapping[6] = false; 
        }

    }

}