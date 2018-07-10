#include "TNode.h"

//Constructor
TNode::TNode()
{
    father = nullptr;
    entity = nullptr;
}

//Destructor
TNode::~TNode()
{
    if(entity != nullptr)
        delete entity;
}


///////////////////////////////
/////       METHODS       /////
///////////////////////////////

//Adds a child to the node
bool TNode::addChild(TNode* c)
{
    //Push_back es una funcion void, siempre insertará el valor que se le pasa
    //Y en caso de que no lo consiga, lanzará una excepción 'bad_alloc' parando la ejecución
    child.push_back(c);
    return true;
}

//Removes a child from the node
bool TNode::removeChild(TNode* c)
{
    for(unsigned int i = 0; i < child.size(); i++)
    {
        if(c == child[i])
        {
            //Erase devuelve un iterator al elemento que sigue al borrado o una excepción si se intenta
            //borrar algo fuera de rango
            child.erase(child.begin()+i);
            return true;
        }
    }
    return false;
}

//Search a child between all the childs of the node
TNode* TNode::searchChild(unsigned int c)
{
    if(c > 0 && c < child.size()-1)
    {
        return nullptr;
    }
    return child[c];
}

///////////////////////////////
/////        DRAW         /////
///////////////////////////////

//Draws the entity contained by the node
void TNode::draw()
{
        
    if(entity != nullptr)
    {
        entity->beginDraw();
    }
    for(unsigned int i = 0; i < child.size(); i++)
    {
        child[i]->draw();
    }
    if(entity != nullptr)
    {
        entity->endDraw();
    }
}