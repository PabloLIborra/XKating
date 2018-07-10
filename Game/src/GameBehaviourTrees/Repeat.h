#pragma once

#include <string>
#include <iostream>
#include "Decorator.h"

class Repeat : public Decorator
{
    protected: 
        virtual BehaviourState Update(float d) override;
        std::string name = "Repeat";
        
    public:
        Repeat(const Behaviour::Pointer& child) : Decorator(child) {};
        std::string getName() {     return name;    }

};