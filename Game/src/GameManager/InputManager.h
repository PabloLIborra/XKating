#pragma once

#include <iostream>
#include "../GameFacade/IInputFacade.h"
#include "../GameObject/InputComponent.h"


class InputManager{

public:

    //Constructor
    InputManager() {}

    //Destructor
    ~InputManager() {
        delete inputFacade;
    }

    //Initializer
    void init(int engine);

    //Updater
    void update();

    //Closer
    void close();

    //Static class getter
    static InputManager& getInstance();

    void setInputFacade(IInputFacade* facade){
        inputFacade = facade;
    }

    IInputFacade* getInputFacade(){
        return inputFacade;
    }

    void setDevice(uintptr_t dev){
        device = dev;
    }

    void setComponent(IComponent::Pointer ptr){
        inputComponent = ptr;
    }

    IComponent::Pointer getComponent(){
        return inputComponent;
    }

    //InputComponent creator
	IComponent::Pointer createInputComponent(GameObject& newGameObject);

private: 

    IInputFacade* inputFacade;

    //This variable is only used when we choose to use irrlicht
    uintptr_t device;

    //Input component is managed from inputManager
    IComponent::Pointer inputComponent;

};