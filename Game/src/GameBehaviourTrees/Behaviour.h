
#pragma once

#include <string>
#include <memory>
#include "../GameObject/GameObject.h"

enum class BehaviourState
{
    INVALID,
    RUNNING,
    SUCCEEDED,
    FAILED,
    ABORTED,
};

class Behaviour
{

    protected:
        virtual void Initialized();
        virtual BehaviourState Update(float d) = 0;
        virtual void OnTerminated();

        BehaviourState b_state;

    public:
        typedef std::shared_ptr<Behaviour> Pointer;
        Behaviour() : b_state(BehaviourState::INVALID) {};

        virtual ~Behaviour();

        BehaviourState Tick(float d);

        virtual void Stop();
        virtual void Reset();
        virtual std::string getName() = 0;

        bool Terminated() const;

        //Getters
        BehaviourState getState() const;

};
