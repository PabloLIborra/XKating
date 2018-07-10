#pragma once

#include <stdint.h>
#include <list>

#include "IComponent.h"
#include "../GameEvent/EventManager.h"
#include <iostream>
class GameObject {

public:

	//Define shared pointer type
	typedef std::shared_ptr<GameObject> Pointer;

	struct TransformationData {
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	//===========================================
	// BASIC FUNCTIONS
	//===========================================

	//Constructor
	GameObject(const uint16_t newId, const TransformationData &newPos) 
		: id(newId), transformData(newPos), oldTransformData(newPos), newTransformData(newPos)  {}

	//Destructor
	~GameObject() {
		components.clear();
	}

	//GetID
	uint16_t getId() { return id; }

	//Get position
	TransformationData getTransformData(){
		return transformData;
	}
	//Set position
	void setTransformData(TransformationData data){
		transformData = data;
	}

	//Get old position
	TransformationData getOldTransformData(){
		return oldTransformData;
	}
	//Set old position
	void setOldTransformData(TransformationData data){
		oldTransformData = data;
	}

	//Get new position
	TransformationData getNewTransformData(){
		return newTransformData;
	}
	//Set new position
	void setNewTransformData(TransformationData data){
		newTransformData = data;
	}

	//===========================================
	// SPECIFIC FUNCTIONS
	//===========================================

	//Init
	void init();

	//Update
	void update(float dTime);

	//Close
	void close();

	//Add component
	void addComponent(IComponent::Pointer component);

	//Get component
	template<typename Component>
	std::shared_ptr<Component> getComponent() {
		for (auto comp: components) {
			if (std::shared_ptr<Component> cmp = std::dynamic_pointer_cast<Component>(comp)) {
				return cmp;
			}
		}
		return nullptr;
	}

	//Get component
	template<typename Component>
	void deleteComponent() {
		std::list<std::shared_ptr<IComponent>>::iterator it;
		for (it = components.begin(); it != components.end(); it++) {
			if (std::shared_ptr<Component> cmp = std::dynamic_pointer_cast<Component>(*it)) {
				components.erase(it);
				return;
			}
		}
	}


private:

	//ObjectID
	uint16_t  id;

	//Object Transformation Data
	TransformationData transformData;
	//Old Transformation Data for interpolation
	TransformationData oldTransformData;
	//New Transformation Data for interpolation
	TransformationData newTransformData;

	//Object Components
	std::list<IComponent::Pointer> components;

};