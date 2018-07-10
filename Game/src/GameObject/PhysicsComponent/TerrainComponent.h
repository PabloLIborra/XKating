#pragma once

#include "../IComponent.h"
#include "../../GamePhysics/LAPALPhysics.h"

class TerrainComponent : public IComponent {

public:

    //Constructor
	TerrainComponent(GameObject& newGameObject, LAPAL::plane3f newPlane) : IComponent(newGameObject), terrain(newPlane) {

        next =          nullptr;
        prev =          nullptr;
        leftSibling =   nullptr;
        rightSibling =  nullptr;

    }

	//Destructor
	virtual ~TerrainComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

    //Getters and setters
    std::shared_ptr<TerrainComponent>   getNext()                                       {    return next;           }
    std::shared_ptr<TerrainComponent>   getPrev()                                       {    return prev;           }
    std::shared_ptr<TerrainComponent>   getLeft()                                       {    return leftSibling;    }
    std::shared_ptr<TerrainComponent>   getRight()                                      {    return rightSibling;   }
    LAPAL::plane3f                      getTerrain()                                    {    return terrain;        }
    void                                setNext(std::shared_ptr<TerrainComponent> t)    {    next = t;              }
    void                                setPrev(std::shared_ptr<TerrainComponent> t)    {    prev = t;              }
    void                                setLeft(std::shared_ptr<TerrainComponent> t)    {    leftSibling = t;       }
    void                                setRight(std::shared_ptr<TerrainComponent> t)   {    rightSibling = t;      }
    void                                setTerrain(LAPAL::plane3f p)                    {    terrain = p;           }
    //Connectors
    void connectSiblingFront(uint16_t); //receives as input the id of the prev node
    void connectSiblingBack(uint16_t);  //receives as input the id of the prev node
    void connectSiblingLeft(uint16_t);  //receives as input the id of the left node
    void connectSiblingRight(uint16_t); //receives as input the id of the left node


private:

    LAPAL::plane3f terrain;

    std::shared_ptr<TerrainComponent> next;
    std::shared_ptr<TerrainComponent> prev;
    std::shared_ptr<TerrainComponent> leftSibling;
    std::shared_ptr<TerrainComponent> rightSibling;

};