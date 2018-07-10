#pragma once

class IGameState {
    
public:
    //Type of states
    enum stateType {
        INTRO,          //Game intro (initial state)
        SELECTION,      //Choose character and load game
        PREMATCH,       //State before match
        MATCH,          //Inside the singleplayer game
        POSTMATCH,      //State after match
        CLIENTLOBBY,    //Client waiting in the lobby for a match
        MULTIMATCH,     //Inside the multiplayer game
        MULTISELECTION, //Inside the multiplayer selection
        MULTIPREMATCH,  //Before multiplayer match
        MULTIPOSTMATCH, //After multiplayer match
        PAUSE           //For when the game is paused
    };

    //Constructor
	IGameState(){};

    //Destructor
	virtual ~IGameState() {};

    //Initialization
    virtual void init() = 0;

    //Updater
    virtual void update(float &accumulatedTime) = 0;

    //Drawer
    virtual void draw() = 0;

    //Shutdown
    virtual void close() = 0;

    stateType type;     //Type of state

};
