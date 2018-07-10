#include "ListenerComponent.h"
#include "../../GameManager/AudioManager.h"
#include "../../GameFacade/IAudioFacade.h"

//Initilizer
void ListenerComponent::init() {
    AudioManager::getInstance().getAudioFacade()->setListener(gameObject);
}

//Update
void ListenerComponent::update(float dTime) {

}

//Closer
void ListenerComponent::close() {

}
