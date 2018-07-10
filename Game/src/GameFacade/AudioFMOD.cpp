#include "AudioFMOD.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/AudioManager.h"
#include "../GameObject/PhysicsComponent/MoveComponent.h"

//==============================================================================================================================
// MACROS
//==============================================================================================================================

//Error management
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
    if (result != FMOD_OK)
    {
        std::cerr << file << "(" << line << "): FMOD error " << result << " - " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
} 

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

//Macro for importing new Banks ands EventDescriptions
#define LOAD_EVENT(bank, event) \
    if (banks.find(#bank) == banks.end()) { \
        banks.insert(std::pair<std::string, FMOD_STUDIO_BANK*>(#bank, nullptr)); \
        ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/"#bank".bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &banks[#bank])); \
    } \
    if (eventDescriptions.find(#event) == eventDescriptions.end()) { \
        eventDescriptions.insert(std::pair<std::string, FMOD_STUDIO_EVENTDESCRIPTION*>(#event, nullptr)); \
        ERRCHECK(FMOD_Studio_System_GetEvent(system, "event:/"#event,  &eventDescriptions[#event])); \
    }
    

//==============================================================================================================================
// DELEGATES DECLARATIONS
//==============================================================================================================================
//COLLISIONS
void shootDefaultCollisionEvent(EventData e);
void shootRampCollisionEvent(EventData e);
void shootItemBoxCollisionEvent(EventData e);
void shootTrapCollisionEvent(EventData e);
//SCORE
void shootOnNewLapEvent(EventData e);
void shootOnOvertakeEvent(EventData e);
void shootOnOvertakenEvent(EventData e);
//MUSIC
void shootOnMusicMainTheme(EventData e);
void shootOnMusicMenu(EventData e);
void shootOnMusicFinish(EventData e);
//OTHERS
void changeAudioLanguage(EventData e);
//Movement
void shootOnSpeed(EventData e);
void shootOnSlide(EventData e);
void shootOnJump(EventData e);
void shootOnTrap(EventData e);
void shootOnShell(EventData e);
void shootOnPlayerSelect(EventData e);



//==============================================================================================================================
// AUDIO FMOD FUNCTIONS
//==============================================================================================================================

//Initializer
void AudioFMOD::openAudioEngine(int lang) {
    //Initialize FMOD System
    ERRCHECK(FMOD_Studio_System_Create(&system, FMOD_VERSION));
    ERRCHECK(FMOD_Studio_System_GetLowLevelSystem(system, &lowLevelSystem));
    ERRCHECK(FMOD_System_SetSoftwareFormat(lowLevelSystem, 0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(FMOD_System_SetOutput(lowLevelSystem, FMOD_OUTPUTTYPE_AUTODETECT));
    ERRCHECK(FMOD_Studio_System_Initialize(system, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

    //Initialize banks
    ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));

    loadBanks();

    //Listeners
    //COLLISION
    EventManager::getInstance().addListener(EventListener {EventType::Default_Collision, shootDefaultCollisionEvent});
    EventManager::getInstance().addListener(EventListener {EventType::RampComponent_Collision, shootRampCollisionEvent});
    EventManager::getInstance().addListener(EventListener {EventType::ItemBoxComponent_Collision, shootItemBoxCollisionEvent});
    EventManager::getInstance().addListener(EventListener {EventType::TrapComponent_Collision, shootTrapCollisionEvent});
    //SCORE
    EventManager::getInstance().addListener(EventListener {EventType::Score_OnNewLap, shootOnNewLapEvent});
    EventManager::getInstance().addListener(EventListener {EventType::Score_OnOvertake, shootOnOvertakeEvent});
    EventManager::getInstance().addListener(EventListener {EventType::Score_OnOvertaken, shootOnOvertakenEvent});
    //MUSIC
    EventManager::getInstance().addListener(EventListener {EventType::Music_MainTheme, shootOnMusicMainTheme});
    EventManager::getInstance().addListener(EventListener {EventType::Music_Menu, shootOnMusicMenu});
    EventManager::getInstance().addListener(EventListener {EventType::Music_Finish, shootOnMusicFinish});
    //OTHERS
    EventManager::getInstance().addListener(EventListener {EventType::Global_ChangeLanguage, changeAudioLanguage});
    //MOVEMENT
    EventManager::getInstance().addListener(EventListener {EventType::Player_Speed, shootOnSpeed});
    EventManager::getInstance().addListener(EventListener {EventType::Player_Slide, shootOnSlide});
    EventManager::getInstance().addListener(EventListener {EventType::Player_Jump, shootOnJump});
    EventManager::getInstance().addListener(EventListener {EventType::Trap_Create, shootOnTrap});
    EventManager::getInstance().addListener(EventListener {EventType::BlueShell_Create, shootOnShell});
    EventManager::getInstance().addListener(EventListener {EventType::RedShell_Create, shootOnShell});
    EventManager::getInstance().addListener(EventListener {EventType::Player_Select, shootOnPlayerSelect});

    //Game veriables
    worldUnits = 0.05;
    gameVolume = 1.0;

}

//Updater
void AudioFMOD::update() {
    #ifndef __APPLE__
    //Update listener position and orientation
    setListenerPosition();

    float vol = GlobalVariables::getInstance().getVolume();

    //Update position of events
    for(auto event : soundEvents) {        
        if(!event.second->isPlaying() || event.second->getEmitter().expired()) {
            //If the event has stopped playing or the object it comes from has been destroyed
            delete event.second;
            soundEvents.erase(event.first);
        }
        else {
            //Set sound position
            auto pos = event.second->getEmitter().lock().get()->getGameObject().getTransformData().position;
            event.second->setPosition(pos * worldUnits);
            event.second->setVolume(vol);
        }

    }

    //Update FMOD system
    ERRCHECK( FMOD_Studio_System_Update(system) );
    #endif
}

//Closer
void AudioFMOD::closeAudioEngine() {

    unloadBanks();

    ERRCHECK( FMOD_Studio_Bank_Unload(stringsBank) );
    ERRCHECK( FMOD_Studio_Bank_Unload(masterBank) );
    
    ERRCHECK( FMOD_Studio_System_Release(system) );
}

//Sets the basic volume of the game. Expected value between 0 and 1;
void AudioFMOD::setVolume(float vol) {
    gameVolume = vol;
}

//Sets the 3D position of the listener
void AudioFMOD::setListenerPosition() {

    if(getListener()!=nullptr){
        //Update listener position
        FMOD_3D_ATTRIBUTES attributes;
        auto pos = getListener()->getTransformData().position;
        auto vel = getListener()->getComponent<MoveComponent>().get()->getMovemententData().velocity;
        auto ang = getListener()->getComponent<MoveComponent>().get()->getMovemententData().angle;
        attributes.position = { pos.x * worldUnits, pos.y * worldUnits, pos.z * worldUnits };
        attributes.velocity = { vel.x * worldUnits, vel.y * worldUnits, vel.z * worldUnits };
        attributes.forward = { -std::cos(ang), 0.0f, -std::sin(ang) };
        attributes.up = { 0.0f, -1.0f, 0.0f };

        ERRCHECK( FMOD_Studio_System_SetListenerAttributes(system, 0, &attributes) );
    }

}

//Load and unload backs
void AudioFMOD::loadBanks() {

    int lang = GlobalVariables::getInstance().getLanguage();

    if(lang == 0){

        LOAD_EVENT(CharacterEN, CharacterEN);

    }
    else if (lang == 1){

        LOAD_EVENT(CharacterES, CharacterES);

    }

    LOAD_EVENT(Music, Music/Music_Game);
    LOAD_EVENT(Music, Music/Music_Menu);
    LOAD_EVENT(Music, Music/Music_Victory);
    LOAD_EVENT(Music, Music/Music_Defeat);
    
    LOAD_EVENT(Items, Items/Item_Trap);
    LOAD_EVENT(Items, Items/Item_Shell);
    
    LOAD_EVENT(Movement, Movement/Jump);
    LOAD_EVENT(Movement, Movement/Slide);
    LOAD_EVENT(Movement, Movement/Collision);
    LOAD_EVENT(Other, Movement/Skate);
    
    LOAD_EVENT(Other, Other/Position_Change);
    LOAD_EVENT(Other, Other/Lap_Last);
    LOAD_EVENT(Other, Other/Lap_New);
    LOAD_EVENT(Other, Other/Menu_Selection);
    LOAD_EVENT(Other, Other/Menu_PlayerSelection);

}
void AudioFMOD::unloadBanks() {
    
    for(auto event : soundEvents)         
        delete event.second;
        
    soundEvents.clear();
    
    eventDescriptions.clear();
    
    for(auto bank : banks)
        ERRCHECK( FMOD_Studio_Bank_Unload(bank.second) );
        
    banks.clear();
}

//Inserts a new event if it doesn't exist
void AudioFMOD::insertSoundEvent(std::string name, ISoundEvent* sound) { 
    soundEvents[name] = sound;
}

//Returns true if we have a soundEvent with that name
bool AudioFMOD::existsSoundEvent(std::string name) {
    if(soundEvents.find(name) == soundEvents.end())
        return false;
    return true;
}

void AudioFMOD::stop(std::string name) {
    if(soundEvents.find(name) != soundEvents.end())
        soundEvents[name]->stop();
}

//==============================================================================================================================
// DELEGATE FUNCTIONS
//==============================================================================================================================
//COLLISION
void shootDefaultCollisionEvent(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "DefaultCollisionEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);

    ISoundEvent* sound1 = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnCollisionEvent");
    sound1->initalizeSound(audioFMOD, e);
  
}
void shootRampCollisionEvent(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "RampCollisionEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
void shootItemBoxCollisionEvent(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "ItemBoxCollisionEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);

    ISoundEvent* sound1 = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnJumpEvent");
    sound1->initalizeSound(audioFMOD, e);
  
}
void shootTrapCollisionEvent(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "TrapCollisionEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);

    ISoundEvent* sound1 = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnTrapEvent");
    sound1->initalizeSound(audioFMOD, e);
  
}
//SCORE
void shootOnNewLapEvent(EventData e) {

    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnNewLapEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);

    ISoundEvent* sound1 = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnNewLapSoundEvent");
    sound1->initalizeSound(audioFMOD, e);

}
void shootOnOvertakeEvent(EventData e) {

    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnOvertakeEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);

    ISoundEvent* sound1 = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnOvertakeSoundEvent");
    sound1->initalizeSound(audioFMOD, e);

}
void shootOnOvertakenEvent(EventData e) {

    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnOvertakenEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);

    ISoundEvent* sound1 = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnOvertakeSoundEvent");
    sound1->initalizeSound(audioFMOD, e);

}

void shootOnMusicMainTheme(EventData e) {

    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "MusicMainThemeEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);

}

void changeAudioLanguage(EventData e) {

    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    audioFMOD->unloadBanks();
    audioFMOD->loadBanks();

}
//MOVEMENT
void shootOnSpeed(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnSpeedEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
void shootOnSlide(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnSlideEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
void shootOnJump(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnJumpEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
void shootOnTrap(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnTrapEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
void shootOnShell(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnShellEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
void shootOnMusicMenu(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "MusicMenuEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
void shootOnMusicFinish(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "MusicFinishEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
void shootOnPlayerSelect(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnPlayerSelectEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
