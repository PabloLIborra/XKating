#pragma once

#include <vector>
#include "PhysicsManager.h"
#include "../GlobalVariables.h"

class MSensorComponent;

class SensorManager{

public: 

    //Constructor
    SensorManager() {}

    //Destructor
    ~SensorManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Shutdown
    void close();

    std::vector<IComponent::Pointer>& getComponentList() { //Visual sensors
        return sensorComponentList;
    }

    std::vector<IComponent::Pointer>& getMComponentList() {//Map sensors
        return sensorMComponentList;
    }

    //Static class getter
    static SensorManager& getInstance();

    //Component creators
    IComponent::Pointer createVSensorComponent(GameObject& newGameObject, float angV, float angI, float md, float ml);
    IComponent::Pointer createMSensorComponent(GameObject& newGameObject, float angV, float angI);


    //Fill list of world objects
    void fillWorldObjects();

    //Update Vision Sensor
    void updateVisionSensor(VSensorComponent* sensor);

    //Update Map Sensor
    void updateMapSensor(MSensorComponent* sensor);

private:
    std::vector<IComponent::Pointer> sensorComponentList;
    std::vector<IComponent::Pointer> sensorMComponentList;
    std::vector<GameObject> worldObjects;
};