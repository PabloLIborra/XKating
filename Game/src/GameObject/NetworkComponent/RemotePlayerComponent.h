#pragma once

#include "../GameObject.h"

class RemotePlayerComponent : public IComponent{
    public:
        //Define ::Pointer
        typedef std::shared_ptr<RemotePlayerComponent> Pointer;

        //Constructor
        RemotePlayerComponent(GameObject& newGameObject);

        //Destructor
        ~RemotePlayerComponent() {}

        //Initilizer
        virtual void init() {};

        //Update
        virtual void update(float dTime) {};

        //Closer
        virtual void close() {};

        //==============================================================
        // Getters and setters
        //==============================================================
        int getServerId(){           return server_id;                    }; 
        void setServerId(int id){    server_id = id;                      };

    private:
        int server_id = -1;
    
};