#pragma once

#include "../IComponent.h"

class IRenderComponent : public IComponent {

public: 

    IRenderComponent(GameObject& newGameObject) : IComponent(newGameObject) {}

    //Drawer
	virtual void draw() = 0;

};