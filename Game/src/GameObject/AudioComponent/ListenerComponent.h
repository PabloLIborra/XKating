#pragma once

#include "../IComponent.h"

class ListenerComponent : public IComponent {

public:

    //Constructor
	ListenerComponent(GameObject& newGameObject) : IComponent(newGameObject) {}

	//Destructor
	virtual ~ListenerComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();
};