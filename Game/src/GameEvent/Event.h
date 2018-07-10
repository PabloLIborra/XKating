#pragma once

#include <vector>
#include <list>
#include <cstdint>
#include <memory>
#include <glm/glm.hpp>

class GameObject;
class IComponent;

//Enumeration of possible events
enum EventType {
    //COMPONENTS: Creation events
    GameObject_Create,
    GameObject_Delete,
    ObjectRenderComponent_Create,
    ObjectRenderComponent_Delete,
    LightRenderComponent_Create,
    LightRenderComponent_Delete,
    CameraRenderComponent_Create,
    CameraRenderComponent_Delete,
    InputComponent_Create,
    InputComponent_Delete,
    MoveComponent_Create,
    MoveComponent_Delete,
    TerrainComponent_Create,
    TerrainComponent_Delete,
    CollisionComponent_Create,
    CollisionComponent_Delete,
    AIDrivingComponent_Create,
    AIDrivingComponent_Delete,
    RampComponent_Create,
    RampComponent_Delete,
    VSensorComponent_Create,
    VSensorComponent_Delete,
    MSensorComponent_Create,
    MSensorComponent_Delete,
    ItemBoxComponent_Create,
    ItemBoxComponent_Delete,
    ListenerComponent_Create,
    ListenerComponent_Delete,
    RemotePlayerComponent_Create,
    RemotePlayerComponent_Delete,
    TrackerDNComponent_Create,
    TrackerDNComponent_Delete,
    //OBJECTS: Creation events
    Item_Create,
    Trap_Create,
    RedShell_Create,
    BlueShell_Create,
    //COLLISION: Events triggered to certain collisions
    Default_Collision,
    RampComponent_Collision,
    StartLineComponent_Collision,
    ItemBoxComponent_Collision,
    TrapComponent_Collision,
    BlueShellComponent_Collision,
    RedShellComponent_Collision,
    //PLAYER: Basic control keys
    Key_Pressed,
    Key_Advance_Down,
    Key_Advance_Up,
    Key_Brake_Down,
    Key_Brake_Up,
    Key_TurnLeft_Down,
    Key_TurnLeft_Up,
    Key_TurnRight_Down,
    Key_TurnRight_Up,
    //PLAYER: Control keys
    Key_UseItem_Down,
    Key_UseItem_Up,
    Key_Jump_Down,
    Key_Jump_Up,
    Key_Kneel_Down,
    Key_Kneel_Up,
    Key_Drift_Up,
    Key_Drift_Down,
    //MENUs: Menu control
    Key_Select_Down,
    Key_Select_Up,
    Key_Back_Down,
    Key_Back_Up,
    //DEBUG: Select debug
    Key_DebugAI_Down,
    Key_DebugAI_Up,
    Key_DebugCamera_Down,
    Key_DebugCamera_Up,
    Key_DebugBehaviour_Down,
    Key_DebugBehaviour_Up,
    Key_DebugNetwork_Down,
    Key_DebugNetwork_Up,
    Key_SlowControl_Down,
    Key_SlowControl_Up,
    Key_FastControl_Down,
    Key_FastControl_Up,
    Key_NormalControl_Down,
    Key_NormalControl_Up,
    Key_Scheduling_Down,
    Key_Scheduling_Up,
    //GAME: Game related events
    Game_Menu,
    Game_Start,
    Game_Pause,
    Game_Close,
    Game_PlayerSelection,
    Game_LoadingScreen,
    Game_ClientLobby,
    Game_Select,
    State_Change,
    //CHOOSE: Choose game type
    Key_Multiplayer_Down,
    Key_Singleplayer_Down,
    Key_Multiplayer_Up,
    Key_Singleplayer_Up,
    //Object: object related events
    Update_Transform_Position,
    Update_Transform_Rotation,
    Update_Transform_Scale,
    //MATCH: Match related events
    Match_Start,
    Match_Countdown,
    Match_Race_Start,
    Match_Race_End,
    Match_End,
    //SCORE: Score related events
    Score_OnNewLap,
    Score_OnOvertake,
    Score_OnOvertaken,
    //MUSIC: Music related events
    Music_MainTheme,
    Music_Menu,
    Music_Finish,
    //PARTICLES: Particle related events
    Particles_SmokeON,
    Particles_SmokeOFF,
    //GLOBAL: Global related events
    Global_ChangeVolume,
    Global_ChangeLanguage,
    Global_ChangeFullscreen,
    //PLAYER: Player relared events
    Player_Speed,
    Player_Slide,
    Player_Jump,
    Player_Select,
    //Idle event
    Idle
};

//Struct containg diferent types of data
struct EventData {

    uint16_t                    Id;
    std::shared_ptr<GameObject> Object;
    std::shared_ptr<IComponent> Component;
    std::shared_ptr<IComponent> CollComponent;
    float                       grade;
    glm::vec3                   Vector;
};

//A event contains a type and the data related to the object
struct Event {

    EventType type;
    EventData data;

};

//An event listener contains a type and a pointer to the funcion to active
struct EventListener{
    EventType listenerType;
    void (*listener)(EventData);
};

//List containig event listeners
typedef std::list<EventListener> EventListenerList;
