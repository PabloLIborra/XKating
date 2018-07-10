#pragma once

#include <string>
 #include <iostream>
#include "Composite.h"

class DynamicSelector : public Composite
{
    protected:
        int actualIdx;    
        std::string name = "DynamicSelector";
        
    public:
        virtual void Initialized() override;

        virtual BehaviourState Update(float d) override;
        std::string getName() {     return name;    }

};
