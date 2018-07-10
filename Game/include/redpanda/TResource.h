#pragma once
#include <iostream>

//Abstract class for the resources in the engine
class TResource{
    public:
        //Constructor and destructor
        TResource() {};
        virtual ~TResource() {
            if(name!=NULL)
                delete name;
        };

        //Method used for loading the resource
        virtual bool loadResource() = 0;
        //Method used for drawing the resource
        virtual void draw() = 0;

        //////////////////////////////////////////////////////////////////////
        /////////////// GETTERS & SETTERS
        //////////////////////////////////////////////////////////////////////

        const char* getName()                                          {   return name;    };
        void  setName(const char* n);

    protected:
        //Route of the resource
        const char*  name = NULL;

        //Maybe a ID variable should be added in order to manage the resources optimally
};