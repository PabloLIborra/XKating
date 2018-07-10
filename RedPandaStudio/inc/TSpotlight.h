#pragma once
#include "TLight.h"

//The directional light of our engine is entirely based on the spot light tutorial provided at ogldev.atspace.co.uk
//More specifically, in the following url: http://ogldev.atspace.co.uk/www/tutorial21/tutorial21.html
class TSpotlight : public TLight{
    public:
        //Constructors and destructors
        TSpotlight(){};
        TSpotlight(glm::vec3 in, glm::vec3 dir, float cut)
        {
            intensity = glm::vec4(in, 1.0);
            direction = dir;
            cutoff = cut;
        }
        ~TSpotlight(){};

        /////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////// GETTERS && SETTERS ////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        glm::vec3 getDirection()            {   return direction;       }
        float getCutoff()                   {   return cutoff;          }
        void setDirection(glm::vec3 d)      {   direction = d;          }
        void setCutoff(float c)             {   cutoff = c;             }

    private:
        //Direction of the light
        glm::vec3 direction;
        //Cutoff value of the light
        float cutoff;
};