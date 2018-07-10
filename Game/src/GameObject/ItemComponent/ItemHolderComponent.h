#pragma once

#include "../GameObject.h"

class ItemHolderComponent : public IComponent {

    public:

        //Constructor
        ItemHolderComponent(GameObject& newGameObject);

        //Destructor
        ~ItemHolderComponent() {}

        //Initializer
        virtual void init();

        //Update
        virtual void update(float dTime);

        //Closer
        virtual void close();

        //Getter
        int getItemType(){
            return itemType;
        }

        //Setter
        void setItemType(int i){
            itemType = i;
        }

    private:

        int itemType = -1;


};