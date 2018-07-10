#include "TerrainComponent.h"
#include "../../GameManager/ObjectManager.h"

//Initilizer
void TerrainComponent::init() {

}

//Update
void TerrainComponent::update(float dTime) {

}

//Closer
void TerrainComponent::close() {

}

//Connectors
void TerrainComponent::connectSiblingFront(uint16_t id) {

    std::shared_ptr<TerrainComponent> nextTerrain;

    //Get terrain component of the object we want to link with
    auto auxTerrain = ObjectManager::getInstance().getObject(id).get()->getComponent<TerrainComponent>();
    auto ourTerrain = ObjectManager::getInstance().getObject(this->getGameObject().getId()).get()->getComponent<TerrainComponent>();
    if(auxTerrain!=nullptr){
        nextTerrain = auxTerrain;
        nextTerrain.get()->setPrev(ourTerrain);   //make the next of previous terrain point to us
        setNext(nextTerrain);           //make our prev point to the previous terrain
    }
} 
  
void TerrainComponent::connectSiblingBack(uint16_t id) {

    std::shared_ptr<TerrainComponent> prevTerrain;

    //Get terrain component of the object we want to link with
    auto auxTerrain = ObjectManager::getInstance().getObject(id).get()->getComponent<TerrainComponent>();
    auto ourTerrain = ObjectManager::getInstance().getObject(this->getGameObject().getId()).get()->getComponent<TerrainComponent>();
    if(auxTerrain!=nullptr){
        prevTerrain = auxTerrain;
        prevTerrain.get()->setNext(ourTerrain);   //make the next of previous terrain point to us
        setPrev(prevTerrain);           //make our prev point to the previous terrain
    }
}

void TerrainComponent::connectSiblingLeft(uint16_t id) {

    std::shared_ptr<TerrainComponent> leftTerrain;

    //Get terrain component of the object we want to link with
    auto auxTerrain = ObjectManager::getInstance().getObject(id).get()->getComponent<TerrainComponent>();
    auto ourTerrain = ObjectManager::getInstance().getObject(this->getGameObject().getId()).get()->getComponent<TerrainComponent>();
    if(auxTerrain!=nullptr){
        leftTerrain = auxTerrain;
        leftTerrain.get()->setRight(ourTerrain);   //make the next of previous terrain point to us
        setLeft(leftTerrain);           //make our prev point to the previous terrain
    }

}

void TerrainComponent::connectSiblingRight(uint16_t id) {

    std::shared_ptr<TerrainComponent> rightTerrain;

    //Get terrain component of the object we want to link with
    auto auxTerrain = ObjectManager::getInstance().getObject(id).get()->getComponent<TerrainComponent>();
    auto ourTerrain = ObjectManager::getInstance().getObject(this->getGameObject().getId()).get()->getComponent<TerrainComponent>();
    if(auxTerrain!=nullptr){
        rightTerrain = auxTerrain;
        rightTerrain.get()->setLeft(ourTerrain);   //make the next of previous terrain point to us
        setRight(rightTerrain);           //make our prev point to the previous terrain
    }

}