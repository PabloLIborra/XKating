#pragma once

#include "../GameObject.h"

class RemoteItemComponent : public IComponent{
    public:
        //Define ::Pointer
        typedef std::shared_ptr<RemoteItemComponent> Pointer;

        //Constructor
        RemoteItemComponent(GameObject& newGameObject);

        //Destructor
        ~RemoteItemComponent() {}

        //Initilizer
        virtual void init() {};

        //Update
        virtual void update(float dTime) {};

        //Closer
        virtual void close() {};

        //==============================================================
        // Getters and setters
        //==============================================================
        int getServerId()       {    return server_id;   }; 
        void setServerId(int id){    server_id = id;     };
        int getType()           {    return type;        }; 
        void setType(int t)     {    type = t;           };       
        int getParentId()       {    return parent_id;   }; 
        void setParentId(int id){    parent_id = id;     };
        
    private:
        int server_id = -1;
        int parent_id = -1;
        int type      = -1;
    
};