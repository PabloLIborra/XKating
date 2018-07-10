 #pragma once

#include <vector>
#include <iostream>
#include "../GlobalVariables.h"
#include "../GameEvent/EventManager.h"
#include "../GameObject/ItemComponent/ItemHolderComponent.h"
#include "../GameObject/ItemComponent/ItemRedShellComponent.h"
#include "../GameObject/ItemComponent/ItemBlueShellComponent.h"
#include "../GameObject/ItemComponent/ItemTrapComponent.h"
#include "../GameObject/ItemComponent/ItemMushroomComponent.h"
#include "../GameObject/ItemComponent/ItemStarComponent.h"
#include "../GameObject/ItemComponent/ItemBoxComponent.h"
#include "../GameObject/NetworkComponent/RemoteItemComponent.h"
#include "PhysicsManager.h"
#include "RenderManager.h"
#include "ObjectManager.h"
#include "NetworkManager.h"

class NetworkManager;
class ObjectManager;

class ItemManager {
    public:

        ItemManager();

        ~ItemManager() {};

        void init();

        void update(float dTime);

        void close();

        IComponent::Pointer createItemHolderComponent(GameObject& newGameObject);
        IComponent::Pointer createItemBox(GameObject& obj);

        //====================================================
        /////       ITEM CREATOR
        //====================================================
        IComponent::Pointer createItem(GameObject& obj);

        static ItemManager& getInstance();

        //Item Create
        IComponent::Pointer createRedShell(GameObject& obj, IItemComponent::InstanceType mode);
        IComponent::Pointer createBlueShell(GameObject& obj, IItemComponent::InstanceType mode);
        IComponent::Pointer createTrap(GameObject& obj, IItemComponent::InstanceType mode);
        IComponent::Pointer createMushroom(GameObject& obj);
        IComponent::Pointer createStar(GameObject& obj);
        

        //Getters
        std::vector<std::shared_ptr<IItemComponent>>& getItemComponents()  {       return ItemComponents;    };
        std::vector<IComponent::Pointer>& getItemHolderComponents()        {       return ItemHolders;       };

        //Item delete
        void deleteItem(IComponent::Pointer component);

    private:
        //====================================================
        //      PRIVATE DATA
        //====================================================
        //Global variables bush
        GlobalVariables* globalVariables;

        std::vector<IComponent::Pointer> ItemHolders;
        std::vector<std::shared_ptr<IItemComponent>> ItemComponents;
        std::vector<std::shared_ptr<ItemBoxComponent>> ItemBoxes;

        //Item ids index
        int ids;

 };
