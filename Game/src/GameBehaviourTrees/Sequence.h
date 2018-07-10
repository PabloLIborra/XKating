#pragma once

#include <string>
#include <iostream>
#include "Composite.h"

class Sequence : public Composite
{
    public:
        Sequence() : s_currentIdx(0) {}
        std::string getName() {     return name;    }

    protected: 
        int s_currentIdx;
        virtual void OnInitialized();
        virtual BehaviourState Update(float d);    
        std::string name = "Sequence";  
};
