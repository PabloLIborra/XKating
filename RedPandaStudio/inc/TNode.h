#pragma once

#include <vector>
#include "TEntity.h"
#include <iostream>

class TNode{

public:

    //Constructors
    TNode();
    TNode(TNode* f) : father(f) {}
    TNode(TNode* f, TEntity* e) : father(f), entity(e) {}

    //Destructor
    ~TNode();

    //Setters
    bool setEntity(TEntity* e)                  {       entity = e;  
                                                        return true;        }

    //Getters
    TNode* getFather()                          {       return father;      }
    TEntity* getEntity()                        {       return entity;      }
    std::vector<TNode*> getChild()              {       return child;       }

    //Methods for child management
    bool addChild(TNode* c);
    bool removeChild(TNode* c);
    TNode* searchChild(unsigned int c);

    //Draws the tree
    void draw();

private:
    //Father of the node
    TNode* father;
    //Entity of the node
    TEntity* entity;
    //Vector of childs of the node
    std::vector<TNode*> child;
        
};