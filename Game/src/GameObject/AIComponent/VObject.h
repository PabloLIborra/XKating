#pragma once

#include <glm/ext.hpp>
#include <memory>
#include <iostream>
#include "../GameObject.h"

class VObject{

    enum ItemType{
        
        Collide         = 0,
        Box             = 1,      
        Ramp            = 2

    };

    private:

        glm::vec3 position;
        float a, b, radius, length;
        int type;

    public:

        //Define shared pointer type
        typedef std::shared_ptr<VObject> Pointer;

        //Constructors
        VObject();
        VObject(glm::vec3 p, float x, float y, float r, int t, float l);

        //Destructor
        ~VObject();

        //Getters
        glm::vec3 getPosition()     {   return position;    };
        float getA()                {   return a;           };
        float getB()                {   return b;           };
        float getRadius()           {   return radius;      };
        float getLength()           {   return length;      };
        int getType()               {   return type;        };

        //Setters
        void setPosition(glm::vec3 p)   {   position = p;   };
        void setA(float x)              {   a = x;          };
        void setB(float y)              {   b = y;          };
        void setRadius(float r)         {   radius = r;     };
        void setLength(float l)         {   length = l;     };
        void setType(int t)             {   type = t;       };

        //Methods
        void nearAB(float* a, float* b, GameObject& myPos);
};
