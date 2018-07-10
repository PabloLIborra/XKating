#pragma once

#include <string>
#include <iostream>
#include "IRenderComponent.h"

class ObjectRenderComponent : public IRenderComponent {

public:

	enum Shape {
		Cube,
		Sphere,
		Mesh,
		Circuit,
		Cone,
		Plane,
		Arrow,
		Cylinder,
		Portion,
		Pyramid,
		Skybox
	};

	enum Poly {
		High,
		Medium,
		Low
	};

    //Constructor
	ObjectRenderComponent(GameObject& newGameObject, Shape newShape, const char* newStr) : IRenderComponent(newGameObject), objectShape(newShape), polyMesh(Poly::High) {
		//If type is mesh, search it in mesh folder
		if(newShape == Shape::Mesh){

			folder = "";
			int i = 0;

			while(newStr[i]!='.'){
				folder += newStr[i];
				i++;
			}

			name = std::string(newStr);
			mesh = "media/mesh/" + folder + "/" + std::string(newStr);
		}
		//If not in img folder
		else{
			name = std::string(newStr);
			img = "media/img/" + std::string(newStr);
		}

	}

	ObjectRenderComponent(GameObject& newGameObject, Shape newShape) : IRenderComponent(newGameObject), objectShape(newShape), polyMesh(Poly::High) {};

	//Destructor
	virtual ~ObjectRenderComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

    //Drawer
	virtual void draw();

	Shape getObjectShape() {
		return objectShape;
	}

	Poly getPolyMesh() {
		return polyMesh;
	}

	std::string getMesh() {
		return mesh;
	}

	std::string getImg() {
		return img;
	}

	std::string getFolder() {
		return folder;
	}

	std::string getName() {
		return name;
	}

	void setMesh(const char* m)	{
		mesh = std::string(m);
	}

	void setPolyMesh(Poly p){
		polyMesh = p;
	}

private:

	Shape objectShape;
	Poly polyMesh;
	std::string mesh;
	std::string img;
	std::string name;
	std::string folder;

};