#include "TResourceLoD.h"

//Inserts an obj in a particular level of the resource
void TResourceLoD::insertObj(int lvl, TResourceOBJ* obj)
{
    if(lvl >= 0 && obj != nullptr)
    {
        if(objs.find(lvl)->second == nullptr)
        {
            objs.insert(std::pair<int, TResourceOBJ*>(lvl, obj));
        }
        else
        {
            std::map<int,TResourceOBJ*>::iterator it = objs.find(lvl);
            objs.erase(it);
            objs.insert(std::pair<int, TResourceOBJ*>(lvl, obj));
        }
    }
}

//Getter for a single OBJ of the resource
TResourceOBJ* TResourceLoD::getResourceObj(int lvl)
{
    if(lvl > 0)
    {
        TResourceOBJ* obj = objs.find(lvl)->second;
    
        return obj;
    }
    return nullptr;
}