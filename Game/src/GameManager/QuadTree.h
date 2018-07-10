#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../GameObject/GameObject.h"

class QuadTree {

    public:

        //==============================================================
        // Class related Functions
        //==============================================================

        //Constructor
        QuadTree() {};

        //Destructor
        ~QuadTree() {};

        //Initializer
        void init(unsigned int max, int rang, std::vector<IComponent::Pointer> &componentList, float qx0, float qx1, float qy0, float qy1);

        //Splits a node in 4 and distributes its children between the subnodes if
        //the number of children it's more than the maximum
        void divide();

        //Updates a component
        void update(float dTime, glm::vec3 position);

        //Clears all the nodes in the QuadTree
        void clear();

        //Debug QuadTree structure
	    void debugStructure(int n);

private:
        //==============================================================
        // Class related Data
        //==============================================================

        //List of entities to insert into the QuadTree
        std::vector<IComponent::Pointer> components;

        //Children of the quadtree
        std::vector<QuadTree> nodes;

        //Maximum of entities in each node
        unsigned int maxEntity;

        //Quadrants in division
        float x0, x1, y0, y1;

        //Default range for updating
        int range;

};

