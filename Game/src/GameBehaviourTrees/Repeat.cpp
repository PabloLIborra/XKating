#include "Repeat.h"

BehaviourState Repeat::Update(float d)
{
    if(d_child)
    {
        if(d_child->Terminated())
        {
            d_child->Reset();
        }
        d_child->Tick(d);
        
        return BehaviourState::RUNNING;
    }
    return BehaviourState::INVALID;
}
