 #pragma once

 #include <vector>
 #include <memory>
 #include <string>
 #include <iostream>
 #include "Behaviour.h"

 class Composite : public Behaviour 
 {
    protected:
        std::vector<Behaviour::Pointer> c_children;
        std::string name = "Composite";

    public:
        typedef std::shared_ptr<Composite> Pointer;

        template<class Behaviour>
        void AddChild(const std::shared_ptr<Behaviour>& child)
        {
            c_children.push_back(child);
        }

        virtual void Stop() override;
        virtual void Reset() override;
        std::string getName() {     return name;    }

 };
