#pragma once

#include "../IComponent.h"
#include "../GameObject.h"

class TrackerDNComponent : public IComponent{
    public:
        //Define ::Pointer
        typedef std::shared_ptr<TrackerDNComponent> Pointer;

        //Constructor
        TrackerDNComponent(GameObject& newGameObject, int server_id, char t);

        //Destructor
        ~TrackerDNComponent() {};

        //Initilizer
        virtual void init() {};

        //Update
        virtual void update(float dTime) {};

        //Closer
        virtual void close() {};

        //==============================================================
        // Getters and setters
        //==============================================================
        int getObjective()       { return objective; }
        void setObjective(int o) { objective = o;    }
        char getType()           { return type;      }
        void setType(char t)     { type = t;         }
    
    private:
        //==============================================================
        // Private data
        //==============================================================
        //Server id of the objective
        int objective;

        //Type of the tracker (last position or new position)
        char type;
};
