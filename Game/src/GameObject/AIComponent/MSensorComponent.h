#pragma once

#include <iostream>
#include "VSensorComponent.h"
#include "../../GameManager/PhysicsManager.h"

class MSensorComponent : public ISensorComponent{
    public:

        //Constructors
        MSensorComponent(GameObject& newGameObject);
        MSensorComponent(GameObject& newGameObject, float angV, float angI);
        
        //Destructor
        virtual ~MSensorComponent() {};

        //Initialize
        virtual void init(){};

        //Close
        virtual void close(){};

        //update
        virtual void update(float dTime){};

        //Checks the objects receives and stores the ones seen in the seenObjects vector
        void updateMapCollisions();

        //Auxiliar function to calculate A and B of given objective
        void calculateABTerrainBack(glm::vec3& objective, float& a, float& b);

        //Auxiliar function to calculate A and B of composition of individual sensors
        glm::vec3 calculateSensorCollision(glm::vec3& position, glm::vec3& tPoint, float* a, float* b);

        //Getters and setters
        std::vector<VObject::Pointer> getMapCollisions()         {   return seenObjects;    };
        glm::vec3 getSensorLeft()                                {   return sensorLeft;     };
        glm::vec3 getSensorRight()                               {   return sensorRight;    };
        float getAngleInitial()                                  {   return angleInitial;   };
        float getAngleVision()                                   {   return angleVision;    };

        void setAngleInitial(float a)                            {   angleInitial = a;      };
        void setAngleVision(float a)                             {   angleVision = a;       };

    private:
        //Angle of the sensor
        float angleInitial;
        //Angle of vision
        float angleVision;

        //Vectors that define the limit of the sensors
        glm::vec3 sensorLeft, sensorRight;

        //Objects being seen
        std::vector<VObject::Pointer> seenObjects;


};