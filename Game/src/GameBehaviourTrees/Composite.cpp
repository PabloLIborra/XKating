#include "Composite.h"


void Composite::Stop()
{
    for(size_t i = 0; i < c_children.size(); i++)
    {
        c_children[i]->Stop();
    }
    Behaviour::Stop();
}

void Composite::Reset()
{
    for(size_t i = 0; i < c_children.size(); i++)
    {
        c_children[i]->Reset();
    }
    Behaviour::Reset();
}