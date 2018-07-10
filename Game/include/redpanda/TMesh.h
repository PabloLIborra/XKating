#pragma once

#include "TEntity.h"
#include "TResource.h"
#include <GL/glew.h>


class TMesh : public TEntity{
    public:
        //Constructor and destructor
        TMesh() {}
        ~TMesh() {}

        //Draw Methods
        void beginDraw();
        void endDraw() {}



        //////////////////////////////////////////////////////////////////////
        /////////////// GETTERS & SETTERS
        //////////////////////////////////////////////////////////////////////

        TResource* getMesh()                                      {   return mesh;      };
        TResource* getMaterial()                                  {   return material;  };

        void  setMesh(TResource* m)                               {   mesh=m;           };
        void  setMaterial(TResource* m)                           {   material=m;       };
    private:
        //Resource mesh
        TResource* mesh;
        //Resource material
        TResource* material;
};