#include "AudioManager.h"
#include "../GameFacade/AudioFMOD.h"
#include "../GameObject/AudioComponent/ListenerComponent.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void addListenerComponent(EventData data);


//==============================================
// AUDIO MANAGER FUNCTIONS
//============================================== 

void AudioManager::init() {

    audioFacade = new AudioFMOD();
    audioFacade->openAudioEngine(1);

    //Listeners
    EventManager::getInstance().addListener(EventListener {EventType::ListenerComponent_Create, addListenerComponent});

}

AudioManager& AudioManager::getInstance()
{
    static AudioManager instance;
    return instance;
}

void AudioManager::update() {
    audioFacade->update();
}

void AudioManager::play() {

}

void AudioManager::close() {
    audioFacade->closeAudioEngine();
    delete audioFacade;
}

IComponent::Pointer AudioManager::createListenerComponent(GameObject& newGameObject) {
    //Make shared pointer of the listener component
    IComponent::Pointer component = std::make_shared<ListenerComponent>(newGameObject);

    //Attach to game object
    newGameObject.addComponent(component);

    //Send event of creation
    EventData data;
    data.Component = component;
    EventManager::getInstance().addEvent(Event {EventType::ListenerComponent_Create, data});

    return component;
}

//==============================================
// DELEGATES
//============================================== 
void addListenerComponent(EventData data) {
    AudioManager::getInstance().setListenerComponent(data.Component);
    data.Component.get()->init();
}