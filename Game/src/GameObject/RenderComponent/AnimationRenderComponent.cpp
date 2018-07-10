#include "AnimationRenderComponent.h"
#include "../../GameManager/RenderManager.h"

//Initilizer
void AnimationRenderComponent::init() {

}

//Update
void AnimationRenderComponent::update(float dTime) {

}

//Closer
void AnimationRenderComponent::close() {

    RenderManager::getInstance().getRenderFacade()->deleteObject(this);
  
}

//Drawer
void AnimationRenderComponent::draw() {
       
} 

//Pause animation
void AnimationRenderComponent::pause() {
    state = 0;
    RenderManager::getInstance().getRenderFacade()->updateAnimation(this);
}


//Play animation once
void AnimationRenderComponent::playOnce() {
    state = 1;
    RenderManager::getInstance().getRenderFacade()->updateAnimation(this);
}

//Start animation 
void AnimationRenderComponent::playLoop() {
    state = 2;
    RenderManager::getInstance().getRenderFacade()->updateAnimation(this);
}