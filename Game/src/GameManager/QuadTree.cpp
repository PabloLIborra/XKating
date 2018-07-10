#include "QuadTree.h"
#include <iostream>

//@parameter max: the maximum entities inside a node
//@parameter entityList: the list of entities to sort into the quadtree
//@parameters int qx0, qx1, qy0, qy1: the position of the corners of the nodes
//@parameter type: the type of the pointers of the entityList
void QuadTree::init(unsigned int max, int rang, std::vector<IComponent::Pointer> &componentList, float qx0, float qx1, float qy0, float qy1){

    for(unsigned int i = 0; i<componentList.size(); ++i)
        components.push_back(componentList[i]);

    x0 = qx0;
    x1 = qx1;
    y0 = qy0;
    y1 = qy1;

    maxEntity = max;

    range = rang;
    
}

void QuadTree::divide(){

    //splits the quadrant into four equal parts
    if (maxEntity < components.size()){
        float x01 = (x0 + x1) / 2.0;
        float y01 = (y0 + y1) / 2.0;

        //Creation of the 4 children nodes
        std::vector<IComponent::Pointer> aux1;
        QuadTree northWest;
        northWest.init(maxEntity, range, aux1, x0, x01, y01, y1);

        std::vector<IComponent::Pointer> aux2;
        QuadTree northEast;
        northEast.init(maxEntity, range, aux2, x01, x1, y01, y1);

        std::vector<IComponent::Pointer> aux3;
        QuadTree southWest;
        southWest.init(maxEntity, range, aux3, x0, x01, y0, y01);

        std::vector<IComponent::Pointer> aux4;
        QuadTree southEast;
        southEast.init(maxEntity, range, aux4, x01, x1, y0, y01);

        //Inserting the entities into the children nodes
        for (unsigned int i = 0; i< components.size(); i++){

            GameObject object = components[i].get()->getGameObject();
            glm::vec3 position = object.getTransformData().position;

            if (x0 <= position.x && x01 >= position.x && y1 >= position.z && y01 <= position.z){//if it is inside node northwest
                northWest.components.push_back(components[i]);
            }
            else if (x01 < position.x && x1 >= position.x && y1 >= position.z && y01 <= position.z){//if it is inside node northeast
                northEast.components.push_back(components[i]);
            }
            else if (x0 <= position.x && x01 >= position.x && y01 > position.z && y0 <= position.z){//if it is inside node southwest
                southWest.components.push_back(components[i]);
            }
            else if (x01 < position.x && x1 >= position.x && y01 > position.z && y0 <= position.z){//if it is inside node southeast
                southEast.components.push_back(components[i]);
            }

        }

        components.clear();

        //Inserting the children nodes into the vector
        nodes.push_back(northWest);
        nodes.push_back(northEast);
        nodes.push_back(southWest);
        nodes.push_back(southEast);

        for (unsigned int i = 0; i < nodes.size(); i++){//We keep dividing the children in case they're full of objects
            nodes[i].divide();
        }

    }
}

//@parameter dTime
//@parameter position: the position of the entity
void QuadTree::update(float dTime, glm::vec3 position){

    int range = 200; //range around the object
    float x01 = (x0 + x1) / 2.0;
    float y01 = (y0 + y1) / 2.0;


    for(unsigned int i=0; i<components.size(); i++){
        components[i].get()->update(dTime); //calls the Update function from each
    }

     //We check the entities around the position of our entity to see if they have to update
    if(components.size() != 0){ 
        if(x0-range <= position.x && x01+range >= position.x && y0-range <= position.z && y01+range >= position.z){
            nodes[0].update(dTime, position); //updating the entities inside the node northwest
        }
        if(x01-range <= position.x && x1+range >= position.x && y0-range <= position.z && y01+range >= position.z){
            nodes[1].update(dTime, position); //updating the entities inside the node northeast
         }
         if(x0-range <= position.x && x01+range >= position.x && y01-range <= position.z && y1+range >= position.z){
            nodes[2].update(dTime, position); //updating the entities inside the node southwest
        }
        if(x01-range <= position.x && x1+range >= position.x && y01-range <= position.z && y1+range >= position.z){
            nodes[3].update(dTime, position); //updating the entities inside the node southeast
        }
    }
}

void QuadTree::clear(){

    components.clear();

    for(unsigned int i=0; i<nodes.size(); i++){
            nodes[i].clear();
    }
}

void QuadTree::debugStructure(int n) {
	if (n == 1)
    {
		//std::cout << "*Level 1* - 4 children" << std::endl;
	}
    else {
		//std::cout << "*Level " << n << " - " << nodes.size() << " nodes - " << components.size() << " components - P1: ";
        //std::cout << x0 << "," << y0 << " | P2:" << x1 << "," << y1 << std::endl;
        for(unsigned int i=0; i<components.size(); i++){
            //std::cout << "     P: " << components[i].get()->getGameObject().getTransformData().position.x << ",";
            //std::cout << components[i].get()->getGameObject().getTransformData().position.z << std::endl;
        }
	}
	for (unsigned int i = 0; i < nodes.size(); i++)
		nodes[i].debugStructure(n + 1);
}



