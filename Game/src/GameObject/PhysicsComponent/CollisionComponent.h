#pragma once

#include "MoveComponent.h"

class CollisionComponent : public IComponent {

public:

	enum Shape {
		Circle,
		Rectangle
	};

	enum Type {
		Default,
		Ramp,
		ItemBox,
		Trap,
		BlueShell,
		RedShell,
		StartLine
	};

    //Constructor
	CollisionComponent(GameObject& newGameObject, float newRadius, float newLength, bool isKinetic, Type newType) : IComponent(newGameObject), 
						radius(newRadius), length(newLength), kinetic(isKinetic), type(newType){ shape = Shape::Circle; }
	CollisionComponent(GameObject& newGameObject, LAPAL::plane3f newRectangle, float newLength, bool isKinetic, Type newType) : IComponent(newGameObject), 
						rectangle(newRectangle), length(newLength), kinetic(isKinetic), type(newType){ shape = Shape::Rectangle; }

	//Destructor
	virtual ~CollisionComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

	//Getters		
	const float 			getRadius() 			{ return radius; 		}
	const LAPAL::plane3f 	getRectangle() 			{ return rectangle; 	}
	const float 			getLength() 			{ return length; 		}
	const Shape 			getShape()				{ return shape;			}
	const Type 				getType()				{ return type;			}
	const bool 				getKinetic()			{ return kinetic; 		}

	//Setters
	void 				setKinetic(bool k)				{ 	kinetic = k; 	}



private:

    float 			radius = -1.f;
	LAPAL::plane3f 	rectangle;
	float 			length;
	Shape 			shape;
	bool 			kinetic;
	Type			type;

};