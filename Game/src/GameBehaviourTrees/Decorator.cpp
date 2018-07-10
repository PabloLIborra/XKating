#include "Decorator.h"

void Decorator::Stop()
{
    if(d_child)
    {
        d_child->Stop();
    }
    Behaviour::Stop();
}

void Decorator::Reset()
{
    if(d_child)
    {
        d_child->Reset();
    }
    Behaviour::Reset();
}