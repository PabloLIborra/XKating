#pragma once

#include <redpanda/RedPandaStudio.h>
#include "IInputFacade.h"
#include "../GameEvent/EventManager.h"


class InputRedPanda : public IInputFacade {

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    InputRedPanda() {}

    //Destructor
    virtual ~InputRedPanda() {}

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openInput(uintptr_t device);

    //Updates window info in the engine
    virtual void updateInput();

    //Closes engine window
    virtual void closeInput();

    //==============================================================
    // Input Related functions
    //==============================================================
    void setGUIContext(void* ctx);
    void DetectAxisInput();

private: 
    
    SDL_Window *device = nullptr;
    SDL_GameController *gamepad = nullptr;

    bool buttonMapping[7] = {false, false, false, false, false, false, false};
    bool keyMapping[16] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

};