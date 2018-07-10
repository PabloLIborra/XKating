#include "Sequence.h"

void Sequence::OnInitialized()
{
    s_currentIdx = 0;
} 

BehaviourState Sequence::Update(float d)
{
    for(size_t i = s_currentIdx; i < c_children.size(); i++)
    {
        const Behaviour::Pointer& child = c_children[s_currentIdx];
        BehaviourState s = child->Tick(d);

        if(s != BehaviourState::SUCCEEDED)
        {
            return s;
        }
        s_currentIdx++;
    }
    return BehaviourState::SUCCEEDED;
}
