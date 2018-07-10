#pragma once

#include <glm/glm.hpp>
#include <memory>

class GameObject;

				   //Allows to use this to create a shared pointer
class IComponent {

public:

	//Define shared pointer type
	typedef std::shared_ptr<IComponent> Pointer;

	//Constructor
	IComponent(GameObject& newGameObject) : gameObject(newGameObject) {}

	//Destructor
	virtual ~IComponent() = default;

	//Getter
	GameObject& getGameObject() { return gameObject; } 

	//Initilizer
	virtual void init() = 0;

	//Update
	virtual void update(float dTime) = 0;

	//Closer
	virtual void close() = 0;


protected:
	
	GameObject& gameObject; //we store the reference to our game Object	

};