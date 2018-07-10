#pragma once

#include "../IComponent.h"
#include "../../GameEvent/EventManager.h"
#include "../../GameObject/ScoreComponent.h"
#include "../../GameBehaviourTrees/DynamicSelector.h"
#include "../../GameBehaviourTrees/Repeat.h"
#include "../../GameBehaviourTrees/Selector.h"
#include "../../GameBehaviourTrees/Sequence.h"
#include "../../GameManager/RenderManager.h"
#include "../../GameManager/ItemManager.h"

class ItemManager;

class AIBattleComponent : public IComponent {
    public:
        //Constructor
        AIBattleComponent(GameObject& newGameObject) : IComponent(newGameObject) {};

        //Destructor
        virtual ~AIBattleComponent() {};

        //Initilizer
        virtual void init();

        //Update
        virtual void update(float dTime);

        //Closer
        virtual void close();

    private:
        std::shared_ptr<Behaviour> root;

};