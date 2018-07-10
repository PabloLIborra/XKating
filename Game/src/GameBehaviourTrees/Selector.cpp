#include "Selector.h"

void Selector::Initialized()
{
    s_actualIdx = 0;
}

BehaviourState Selector::Update(float d)
{
    for(size_t i = s_actualIdx; i < c_children.size(); i++)
    {
        const Behaviour::Pointer& child = c_children[s_actualIdx];
        BehaviourState s = child->Tick(d);

        if(s != BehaviourState::FAILED)
        {
            return s;
        }
        s_actualIdx++;
    }
    return BehaviourState::FAILED;
}
