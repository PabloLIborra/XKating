#pragma once

#include <iostream>
#include <map>
#include "../GlobalVariables.h"

//Managers include
#include "../GameEvent/EventManager.h"
#include "PhysicsManager.h"
#include "ItemManager.h"
#include "WaypointManager.h"
#include "AIManager.h"
#include "RenderManager.h"
#include "SensorManager.h"
#include "ScoreManager.h"
#include "AudioManager.h"
#include "NetworkManager.h"

class ItemManager;
class RenderManager;
class NetworkManager;

class ObjectManager{

public: 

    enum PlayerType{
            
        punk            = 0,   
        crocodile       = 1,   
        cyborg          = 2,    
        witch           = 3        
        
    };

    //Constructor
    ObjectManager() {
        gameRunning = true;
    }

    //Destructor
    ~ObjectManager() {}

    //Initialization
    void init();

    //Shutdown
    void close();

    //Create an object and fire creation event
    GameObject::Pointer createObject(uint16_t id, GameObject::TransformationData transform);


    //Add an object
    void addObject(GameObject::Pointer ptr);

    //Delete an object
    void deleteObject(uint16_t id);

    //Get an object
    GameObject::Pointer getObject(uint16_t id);

    //Show the ids of the current objects
    void showObjects();

    //Init all objects
    void initObjects();

    //Static class getter
    static ObjectManager& getInstance();

    //Create Player and IA
    void createPlayer(GameObject::TransformationData tansform, int type, int move, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent); //Read .cpp 

    GameObject::Pointer createPunk(GameObject::TransformationData tansform, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent);
    GameObject::Pointer createWitch(GameObject::TransformationData tansform, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent);
    GameObject::Pointer createCyborg(GameObject::TransformationData tansform, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent);
    GameObject::Pointer createCrocodile(GameObject::TransformationData tansform, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent);

    //==============================================
    // Create player auxiliars
    //============================================== 

    //Adds the components needed to the character created
    void createComponents(GameObject::Pointer ob, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent, LAPAL::movementData mData, int player);

    //Creates character depending on option selected specified by the int
    void createMove(GameObject::Pointer obj, int move);

    //Game running getter and setter
    void setGameRunning(bool s) {   gameRunning = s;    }
    bool getGameRunning()       {   return gameRunning; }

private:

    //Map of objects
    std::map<uint16_t, GameObject::Pointer> objectsMap;

    bool gameRunning;

};