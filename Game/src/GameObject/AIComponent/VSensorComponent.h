#pragma once

#include <memory>
#include <vector>
#include <glm/ext.hpp>
#include "VObject.h"
#include "ISensorComponent.h"
#include "../PhysicsComponent/CollisionComponent.h"
#include "../ItemComponent/ItemBoxComponent.h"
#include "../PhysicsComponent/RampComponent.h"

class VSensorComponent : public ISensorComponent{
    public:

        //Constructors
        VSensorComponent(GameObject& newGameObject);
        VSensorComponent(GameObject& newGameObject, float angV, float angI, float md, float ml); //AngV = angle of vision in degrees, AngI = angle of init in radians
        
        //Destructor
        virtual ~VSensorComponent() {};

        //Initialize
        virtual void init(){};

        //Close
        virtual void close(){};

        //update
        virtual void update(float dTime){};

        //Checks the objects seen and stores the ones seen in the seenObjects vector
        void updateSeenObjects(std::vector<GameObject> objects);

        //Auxiliar function to calculate A and B of given objective
        void calculateAB(glm::vec3 objective, float& a, float& b);

        //Getters and setters
        std::vector<VObject::Pointer> getSeenObjects()           {   return seenObjects;    };
        glm::vec3 getSensorLeft()                                {   return sensorLeft;     };
        glm::vec3 getSensorRight()                               {   return sensorRight;    };
        float getAngleInitial()                                  {   return angleInitial;   };
        float getAngleVision()                                   {   return angleVision;    };
        float getMaxDistance()                                   {   return maxDistance;    };
        float getMaxLength()                                   {   return maxLength;    };

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

        float maxDistance;
        float maxLength;


};