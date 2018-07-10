#pragma once
#include "TEntity.h"
#include <iostream>

class TLight : public TEntity{

protected:
    glm::vec4 intensity = glm::vec4(1.0,1.0,1.0,1.0);
    int id;
    bool activated;

public:

    TLight() {}
    TLight(glm::vec3 in) {
        intensity = glm::vec4(in, 1.0);
    }
    ~TLight() {}

    //Draw Methods
    void beginDraw();
    void endDraw();

    //////////////////////////////////////////////////////////////////////
    //////////////////////// GETTERS && SETTERS //////////////////////////
    //////////////////////////////////////////////////////////////////////

    void setID(int i)                                   {   id = i;            }
    void setActivated(bool b)                           {   activated = b;     }
    glm::vec4 getIntensity()                            {   return intensity;  }
    int getID()                                         {   return id;         }
    bool getActivated()                                 {   return activated;  }

};