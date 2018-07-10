#include "Behaviour.h"


Behaviour::~Behaviour()
{

}
BehaviourState Behaviour::Tick(float d)
{
    if(Terminated())
    {
        return b_state;
    }

    if(b_state == BehaviourState::INVALID)
    {
        Initialized();
    }

    b_state = Update(d);

    if(b_state != BehaviourState::INVALID)
    {
        OnTerminated();
    }

    return b_state;
} 

void Behaviour::Stop()
{
    if(b_state == BehaviourState::RUNNING)
    {
        b_state = BehaviourState::ABORTED;
        OnTerminated();
    }
}

void Behaviour::Reset()
{
    if(b_state == BehaviourState::RUNNING)
    {
        b_state = BehaviourState::ABORTED;
        OnTerminated();
    }

    b_state = BehaviourState::INVALID;
}

BehaviourState Behaviour::getState() const
{
    return b_state;
}

bool Behaviour::Terminated() const
{
    if((b_state == BehaviourState::SUCCEEDED) || (b_state == BehaviourState::SUCCEEDED) || (b_state == BehaviourState::SUCCEEDED))
    {
        return true;
    }
    return false;
}

void Behaviour::Initialized()
{

}

void Behaviour::OnTerminated()
{

}