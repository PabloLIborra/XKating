#pragma once

#include <string>
#include <iostream>
#include "Composite.h"


class Selector : public Composite
{
    protected:
        int s_actualIdx;
        virtual void Initialized() override;
        virtual BehaviourState Update(float d) override;
        std::string name = "Selector";
        
    public: 
        Selector() : s_actualIdx(0) {};
        std::string getName() {     return name;    }

};