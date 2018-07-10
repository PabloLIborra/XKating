#include "DynamicSelector.h"

void DynamicSelector::Initialized()
{
    actualIdx = 0;
} 

BehaviourState DynamicSelector::Update(float d)
{
    int lastIdx = actualIdx;

    for(size_t i = 0; i < c_children.size(); i++)
    {
        const Behaviour::Pointer& child = c_children[i];

        if(child->Terminated())
        {
            child->Reset();
        }

        BehaviourState s = child->Tick(d);

        if(s != BehaviourState::FAILED)
        {
            for(int j = i + 1; j <= lastIdx; j++)
            {
                c_children[j]->Stop();
            }

            actualIdx = i;
            return s;
        }
    }

    return BehaviourState::FAILED;
}
