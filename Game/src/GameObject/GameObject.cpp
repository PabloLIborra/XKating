#include "GameObject.h"

//Init
void GameObject::init() {
    //Initilize all components
    for (auto comp : components)
		comp.get()->init();
}

//Update
void GameObject::update(float dTime) {

}

//Close
void GameObject::close(){
    for (auto comp : components)
        comp.get()->close();
}

//Add component
void GameObject::addComponent(IComponent::Pointer component) {
    components.push_back(component);
}   