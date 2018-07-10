#include "AIDrivingComponent.h"
#include "WaypointComponent.h"
#include "../../GameManager/WaypointManager.h"
#include <memory>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <fstream>

//Initilizer
void AIDrivingComponent::init() {
	readTurnValues();
	readSpeedValues();
}

//Update
void AIDrivingComponent::update(float dTime) {
}

//Closer
void AIDrivingComponent::close() {
}

//Decides where to turn and in which grade (with a percentage of 0 to 1)

//---------------------------*/
float AIDrivingComponent::girar(GameObject& myPos, std::vector<VObject::Pointer> array, std::vector<VObject::Pointer> walls, glm::vec3 waypoint, float a, float b)
{
	//final turn decision
	float decision = 0.0f;

	//Fuzzy decisions
	float steeringNone = 0.0f, steeringLeft = 0.0f, steeringRight = 0.0f;

	//calculate the arctg being a the right side, then b over a is the right choice. Returns in radians.
	float atan_w = glm::atan(a,b)/3.14159265358979323846264338327f;

	//waypoints
	if(atan_w <=-0.75){ //Since the value of the atan 0.25 corresponds to the center, from 1 to 0.25 is left, from 0.25 to -0.75 is right, and -0.75 to -1 is left.
		atan_w += 2.f; 	//that's why we add this
	}
	//Limits: -0.75 - 0.25 right, 0.25 center, 0.25 to 1.25 left
	float wp_left 	= inferL(atan_w		,wp_left_min 		,wp_left_c 		,wp_left_max   	);
	float wp_center = inferT(atan_w		,wp_center_min		,wp_center_c 	,wp_center_max	);
	float wp_right 	= inferL(atan_w		,wp_right_min		,wp_right_c		,wp_right_max 	);

	//If we have collisions to collide with
	if(array.size()>0){
		float atan_obs 	 = 0.0f, atan_box = 0.0f;
		float atan_walls 	 = 0.0f;
		float obs_left   = 0.f, obs_center = 0.f, obs_right = 0.f;
		float box_left   = 0.f, box_center = 0.f, box_right = 0.f;
		float rmp_left   = 0.f, rmp_center = 0.f, rmp_right = 0.f;
		float go_left   = 0.f, go_right = 0.f;
		int box = 0, ramp = 0, obs_count = 0;

		float nearA, nearB;

		//Accumulate the mean atan value of them all to pickpoint a medium point of no collisions
		for(unsigned i = 0; i<array.size(); ++i){
			if(array[i]->getType() == 0)
			{
				if(array[i]->getRadius() != -1)
				{
					array[i]->nearAB(&nearA, &nearB, myPos);
				}
				else
				{
					nearA = array[i].get()->getA();
					nearB = array[i].get()->getB();
				}

				atan_obs = (glm::atan(nearA, nearB) / 3.14159265358979323846264338327f );
				//collisions
				obs_left	+= inferL(atan_obs		,obs_left_min		,obs_left_c		,obs_left_max  	);
				obs_center 	+= inferT(atan_obs		,obs_center_min		,obs_center_c 	,obs_center_max	);
				obs_right	+= inferL(atan_obs		,obs_right_min		,obs_right_c	,obs_right_max 	);

				obs_count++;

			}
			else if(array[i]->getType() == 1)
			{
				nearA = array[i].get()->getA();
				nearB = array[i].get()->getB();
				
				atan_box = (glm::atan(nearA, nearB) / 3.14159265358979323846264338327f );
				//collisions
				box_left 		+= inferL(atan_box		,box_left_min  		,box_left_c 	,box_left_max  	);
				box_center 		+= inferT(atan_box		,box_center_min		,box_center_c 	,box_center_max	);
				box_right 		+= inferL(atan_box		,box_right_min		,box_right_c	,box_right_max 	);
				
				box++;
			}
			else if(array[i]->getType() == 2)
			{
				nearA = array[i].get()->getA();
				nearB = array[i].get()->getB();
				
				atan_box = (glm::atan(nearA, nearB) / 3.14159265358979323846264338327f );
				//collisions
				rmp_left 		+= inferL(atan_box		,rmp_left_min	 	,rmp_left_c		,rmp_left_max  	);
				rmp_center 		+= inferT(atan_box		,rmp_center_min		,rmp_center_c 	,rmp_center_max	);
				rmp_right 		+= inferL(atan_box		,rmp_right_min		,rmp_right_c	,rmp_right_max 	);
				
				ramp++;
			}
		}


		for(unsigned i = 0; i<walls.size(); ++i){
			atan_walls = (glm::atan(walls[i].get()->getA(),walls[i].get()->getB()) / 3.14159265358979323846264338327f );
			//collisions
			if(atan_walls <=-0.75){ 	//Same process as the waypoint
				atan_walls += 2.f;
			}
			obs_left	+= inferL(atan_walls	,wls_left_min		,wls_left_c		,wls_left_max	);
			obs_center 	+= inferT(atan_walls	,wls_center_min		,wls_center_c 	,wls_center_max	);
			obs_right 	+= inferL(atan_walls	,wls_right_min		,wls_right_c	,wls_right_max 	);

			obs_count++;
		}

		float types = ramp + box;

		if(obs_count != 0)
		{
			obs_left   = obs_left 	/ ((array.size() + walls.size()) - types);					// (array.size() - types) because two items can be box and ramp
			obs_center = obs_center / ((array.size() + walls.size()) - types);
			obs_right  = obs_right 	/ ((array.size() + walls.size()) - types);
		}

		auto haveItem = myPos.getComponent<ItemHolderComponent>()->getItemType();

		if(types == 0)
		{
			go_left = wp_left;
			go_right = wp_right;
		}
		else if(types == 1)
		{
			if(box == 1)
			{
				if(haveItem == -1)
				{
					go_left = wp_left 		* 	wp_left_r1 		+ 	box_left 	* 	box_left_r1;
					go_right = wp_right 	* 	wp_right_r1 	+ 	box_right 	* 	box_right_r1;
				}
				else
				{
					go_left = wp_left;
					go_right = wp_right;
				}
			}
			else if(ramp == 1)
			{
				go_left = wp_left 		*	 wp_left_r2		 +	 rmp_left	* 	rmp_left_r2;
				go_right = wp_right		*	wp_right_r2		 +	 rmp_right	*	rmp_right_r2;
			}
		}
		else if(types == 2)
		{
			if(haveItem == -1)
			{
				go_left = wp_left	*	wp_left_r3	+	box_left	*	box_left_r3		+	rmp_left	*	rmp_left_r3;
				go_right = wp_right *	wp_right_r3 +	box_right	*	box_right_r3	+	rmp_right	*	rmp_right_r3;
			}
			else
			{
				go_left = wp_left 		*	 wp_left_r2		 +	 rmp_left	* 	rmp_left_r2;
				go_right = wp_right		*	wp_right_r2		 +	 rmp_right	*	rmp_right_r2;
			}
		}


		steeringLeft 	= glm::max( go_left, obs_right);
		steeringRight 	= glm::max( go_right, obs_left);
		steeringNone 	= 1-obs_center;

	}else{
		//ruleset
		steeringLeft = wp_left;
		steeringNone = wp_center;
		steeringRight = wp_right;
	}

	//defuzzifier inference
	//Here we use the centroid point between the defuzzified inferences, to pinpoint the crisp steering value
	float op1_cx, op1_cy, op1_area, op2_cx, op2_cy, op2_area, op3_cx, op3_cy, op3_area;

	centroidT(&op1_cx, &op1_cy, &op1_area, steeringNone, ctd_left_r1, ctd_center_r1, ctd_right_r1);
	centroidT(&op2_cx, &op2_cy, &op2_area, steeringRight, ctd_left_r2, ctd_center_r2, ctd_right_r2);
	centroidT(&op3_cx, &op3_cy, &op3_area, steeringLeft, ctd_left_r3, ctd_center_r3, ctd_right_r3);

	//adding all the centroids and crisping end result
	float cx = (op1_cx * op1_area + op2_cx * op2_area + op3_cx * op3_area ) / (op1_area + op2_area + op3_area);

	decision = cx;

	return decision;

}


//Decides wheter the NPC should brake, do nothing or accelerate, and in which proportion. Takes in account where objects are, distance to closest one, where the NPC
//is going, and where it is headed to.

//---------------------------*/
float AIDrivingComponent::acelerar_frenar(GameObject& myPos, std::vector<VObject::Pointer> array, float direction, float speed, float a_W, float b_w)
{
	//final turn decision
	float decision = 0.0f;

	float accelerating, none, braking;

	//fuzzifier and inference
	//Where am I going
	float going_left 	= inferL(direction,		s_going_left_min,		s_going_left_c,		s_going_left_max);
	float going_center 	= inferT(direction,		s_going_center_min,		s_going_center_c,	s_going_center_max);
	float going_right 	= inferL(direction,		s_going_right_min,		s_going_right_c,	s_going_right_max);

	//If there are objects to collide with
	if(array.size()>0 && speed!=0.0f){

		//Previous calculus
		float atan_obs = 0.0f, min_value = FLT_MAX;
		for(unsigned i = 0; i<array.size(); i++){

			if(array[i]->getType() == 0)
			{
				float nearA, nearB;

				if(array[i]->getRadius() != -1)
				{
					array[i]->nearAB(&nearA, &nearB, myPos);
				}
				else
				{
					nearA = array[i].get()->getA();
					nearB = array[i].get()->getB();
				}

				atan_obs = (glm::atan(nearA,nearB) / 3.14159265358979323846264338327f )/array.size();
				min_value = glm::min(min_value,nearA+nearB);
			}
		}

		//Dividing between speed to get a time of impact
		min_value = (min_value >0 && speed > 0) ? min_value / speed : 0.f;
		
		//collisions
		//-----------------------------------
		float obs_left = inferL(atan_obs,		s_obs_left_min,		s_obs_left_c,		s_obs_left_max);
		float obs_center = inferT(atan_obs,		s_obs_center_min,	s_obs_center_c,		s_obs_center_max);
		float obs_right = inferL(atan_obs,		s_obs_right_min,	s_obs_right_c,		s_obs_right_max);

		//-----------------------------------

		//distance
		//-----------------------------------
		float obs_closeRange = inferL(min_value,	s_obs_closeRange_min,	s_obs_closeRange_c,		s_obs_closeRange_max);
		float obs_mediumRange = inferT(min_value,	s_obs_mediumRange_min,	s_obs_mediumRange_c,	s_obs_mediumRange_max);
		float obs_farRange = inferL(min_value,		s_obs_farRange_min,		s_obs_farRange_c,		s_obs_farRange_max);

		//Ruleset

		accelerating = glm::min(
							obs_farRange, 
							glm::max(
								glm::min(obs_left,1-going_left), glm::min(obs_right, 1-going_right)
							)
					   ); //Acelerar cuando no hay objetos cerca y no estamos en su rumbo de colisión

		none =  glm::min(
					glm::max(
						glm::max(glm::min(obs_left,going_left), glm::min(obs_right, going_right)), glm::min(obs_center,going_center)
					), 
					glm::max(obs_mediumRange,obs_farRange)
				); //No aumentar la velocidad cuando estamos en el rumbo de colisión pero están lejos o a media distancia

		braking = glm::min(
						glm::max(
							glm::max(glm::min(obs_left,going_left), glm::min(obs_right, going_right)), glm::min(obs_center,going_center)
						), 
						obs_closeRange
				); //Frenar cuando vamos en rumbo de colisión y están cerca los objetos
		if(accelerating == 0 && none == 0 && braking == 0){
			accelerating = 1.f;
			none = 0.f;
			braking = 0.f;
		}

	//if there are no objects to collide with
	}else{
		//Ruleset

		accelerating = 1.0f;
		none = 0.0f;
		braking = 0.0f;

	}

	//defuzzifier inference
	//Here we use the centroid point between the defuzzified inferences, to pinpoint the crisp steering value
	float op1_cx, op1_cy, op1_area, op2_cx, op2_cy, op2_area, op3_cx, op3_cy, op3_area;

	centroidT(&op1_cx, &op1_cy, &op1_area, none,			s_ctd_left_r1,	s_ctd_center_r1,	s_ctd_right_r1);
	centroidT(&op2_cx, &op2_cy, &op2_area, braking,			s_ctd_left_r2,	s_ctd_center_r2,	s_ctd_right_r2);
	centroidT(&op3_cx, &op3_cy, &op3_area, accelerating,	s_ctd_left_r3,	s_ctd_center_r3,	s_ctd_right_r3);

	//adding all the centroids and crisping end result
	float cx = (op1_cx * op1_area + op2_cx * op2_area + op3_cx * op3_area ) / (op1_area + op2_area + op3_area);
	decision = cx;

	return decision;
}

//Inferes the fuzzy value in a line with the type given
float AIDrivingComponent::inferL(float value, float limit1, float limit2, int type){
	//Switching the type
	switch(type){
		//Lineal case
		case 0:
			if(value>limit2)
			{
				return 1;
			}
			else if(value<limit1)
			{
				return 0;
			}
			else
			{
				return ((value-limit1)/(limit2-limit1));
			}
			break;

		//Inverse line case
		case 1:
			if(value>limit2)
			{
				return 0;
			}
			else if(value<limit1)
			{
				return 1;
			}
			else
			{
				return 1-(value-limit1)/(limit2-limit1);
			}
			break;

		//Circular line case
		case 2:
			if(value>limit2)
			{
				return 1;
			}
			else if(value<limit1)
			{
				return 0;
			}
			else
			{
				
				float cos=(value-limit1)/(limit2-limit1);
				return sqrt(1-cos*cos);
			}

			break;
		//Inverse circular line case
		case 3:
			if(value>limit2)
			{
				return 0;
			}
			else if(value<limit1)
			{
				return 1;
			}
			else
			{
				
				float cos=1-(value-limit1)/(limit2-limit1);
				return sqrt(1-cos*cos);
			}

			break;

	}
	return -1;
}

//Inferes the fuzzy value in a triangular function given the parameters
float AIDrivingComponent::inferT(float value, float limit1, float limit2, float limit3){
	if(value<limit1){
		return 0.0f;
	}else if(value>limit3){
		return 0.0f;
	}else if(limit1<=value && value<limit2){
		return (value-limit1)/(limit2-limit1);
	}else if(limit2<=value && value<=limit3){
		return 1-(value-limit2)/(limit3-limit2);
	}
	return -1;
}

//Inferes the fuzzy value in a trapeizodal function
float AIDrivingComponent::inferT2(float value, float limit1, float limit2, float limit3, float limit4){
	if(value<limit1){
		return 0;
	}else if(value>limit4){
		return 0;
	}else if(limit1<=value && value<limit2){
		return (value-limit1)/(limit2-limit1);
	}else if(limit2<=value && value<limit3){
		return 1;
	}else if(limit3<=value && value<=limit4){
		return 1-(value-limit3)/(limit4-limit3);
	}
	return -1;
}

//Inferes the fuzzy value with a circular inference
float AIDrivingComponent::inferC(float value, float limit1, float limit2, float limit3){
		if(value<limit1){
			return 0;
		}else if(value>limit3){
			return 0;
		}else if(limit1<=value && value<limit2){
			float cos=(value-limit1)/(limit2-limit1);
			return sqrt(1-cos*cos);
		}else if(limit2<=value && value<=limit3){
			float cos=1-(value-limit1)/(limit2-limit1);
			return sqrt(1-cos*cos);
		}
		return -1;
}

//--------------------------------------------
//DEFUZZING FORMULAE OF HELL: TRAPEZOIDS REILM
//--------------------------------------------

//Returns in Cx and Cy the triangle's X,Y positions of its centroid, given the height h
void AIDrivingComponent::centroidT(float* cx, float* cy, float* area, float h, float limit1, float limit2, float limit3){
	//if the height is 1, then it shall be a triangle
	if(h == 0.0f){
		*cx = 0.0f;
		*cy = 0.0f;
		*area = 0.0f;
	}else{
		if(h == 1.f){
			
			//float y1 = 0.f, y3 = 0.f, y2 = 0.5f, y4= 0.5f;
			//float x1 = limit1, x3 = limit3, x4 = (limit1+limit2)*0.5f, x2=(limit3+limit2)*0.5f;
			
			float m = 0.5f / ((limit3+limit2)*0.5f-limit1);
			float m2 = 0.5f / ((limit1+limit2)*0.5f-limit3);
			float n = - m * limit1;
			float n2 = -m2 * limit3;
			
			//Triangle Cx
			*cx = (n - n2) /(m2 - m);;

			//Triangle Cy
			*cy = m * (*cx) + n;

			//triangle area
			*area = (1+limit3-limit1)*0.5f;



		//if not, it is a trapezoid, and it shall be treated as one
		}else{
			float c = limit2 - (limit2-limit1) * (1 - h);
			float a = (limit3-limit2) * (1 - h ) + limit2 - c;
			float b = limit3-limit1;

			//Friggin' trapezoid Cx
			*cx = limit1 + (2*a*c + a*a + c*b + a*b + b*b) / (3*(a+b));
			
			//Friggin' trapezoid Cy
			*cy = h * (2*a + b) / (3*(a + b));

			//Trapezoid Area
			*area = h * (a + b)*0.5f;
		}
	}
}

//Returns in Cx and Cy the trapezoid's X,Y positions of its centroid, given the height h
void AIDrivingComponent::centroidT2(float* cx, float* cy, float* area, float h, float limit1, float limit2, float limit3, float limit4){
	if(h == 0.0f){
		*cx = 0.0f;
		*cy = 0.0f;
		*area = 0.0f;
	}else{
		float c = limit2-limit1;
		float a = limit3-limit2;
		float b = limit4-limit1;

		*cx = limit1 + (2*a*c + a*a + c*b + a*b + b*b) / (3*(a+b));
		*cy = h * (2*a + b) / (3*(a + b));
		*area = h * (a + b)*0.5f;
	}
}



//==============================================
// READ AI VALUES
//==============================================

void AIDrivingComponent::readTurnValues()
{
    using namespace rapidxml;

    xml_document<> doc;
    xml_node<> * root_node;

    //Read the file and put it into a char array
    std::ifstream theFile("media/xml/TurnAIValues.xml");
	std::string buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);

    // Find our root node
	root_node = doc.first_node("player");

    // Iterate over the gameObjects, reading them
    for (xml_node<> * player = root_node; player; player = player->next_sibling()) {
		
		if(std::stoi(player->first_attribute("name")->value()) == 
			getGameObject().getComponent<MoveComponent>().get()->getMovemententData().player){

			//Read Waypoint Data
			xml_node<> * waypoint = player->first_node("waypoint");

			wp_left_min 	= std::stof(waypoint->first_node("left")->first_attribute("wp_left_min")->value());
			wp_left_c   	= std::stof(waypoint->first_node("left")->first_attribute("wp_left_c")->value());
			wp_left_max 	= std::stof(waypoint->first_node("left")->first_attribute("wp_left_max")->value());

			wp_center_min 	= std::stof(waypoint->first_node("center")->first_attribute("wp_center_min")->value());
			wp_center_c   	= std::stof(waypoint->first_node("center")->first_attribute("wp_center_c")->value());
			wp_center_max 	= std::stof(waypoint->first_node("center")->first_attribute("wp_center_max")->value());

			wp_right_min 	= std::stof(waypoint->first_node("right")->first_attribute("wp_right_min")->value());
			wp_right_c   	= std::stof(waypoint->first_node("right")->first_attribute("wp_right_c")->value());
			wp_right_max 	= std::stof(waypoint->first_node("right")->first_attribute("wp_right_max")->value());

			//Read Obstacle Data
			xml_node<> * obstacle = player->first_node("obstacle");

			obs_left_min 	= std::stof(obstacle->first_node("left")->first_attribute("obs_left_min")->value());
			obs_left_c   	= std::stof(obstacle->first_node("left")->first_attribute("obs_left_c")->value());
			obs_left_max 	= std::stof(obstacle->first_node("left")->first_attribute("obs_left_max")->value());

			obs_center_min 	= std::stof(obstacle->first_node("center")->first_attribute("obs_center_min")->value());
			obs_center_c   	= std::stof(obstacle->first_node("center")->first_attribute("obs_center_c")->value());
			obs_center_max 	= std::stof(obstacle->first_node("center")->first_attribute("obs_center_max")->value());

			obs_right_min 	= std::stof(obstacle->first_node("right")->first_attribute("obs_right_min")->value());
			obs_right_c   	= std::stof(obstacle->first_node("right")->first_attribute("obs_right_c")->value());
			obs_right_max 	= std::stof(obstacle->first_node("right")->first_attribute("obs_right_max")->value());

			//Read Box Data
			xml_node<> * box = player->first_node("box");

			box_left_min 	= std::stof(box->first_node("left")->first_attribute("box_left_min")->value());
			box_left_c   	= std::stof(box->first_node("left")->first_attribute("box_left_c")->value());
			box_left_max 	= std::stof(box->first_node("left")->first_attribute("box_left_max")->value());

			box_center_min 	= std::stof(box->first_node("center")->first_attribute("box_center_min")->value());
			box_center_c   	= std::stof(box->first_node("center")->first_attribute("box_center_c")->value());
			box_center_max 	= std::stof(box->first_node("center")->first_attribute("box_center_max")->value());

			box_right_min 	= std::stof(box->first_node("right")->first_attribute("box_right_min")->value());
			box_right_c   	= std::stof(box->first_node("right")->first_attribute("box_right_c")->value());
			box_right_max 	= std::stof(box->first_node("right")->first_attribute("box_right_max")->value());

			//Read Ramp Data
			xml_node<> * ramp = player->first_node("ramp");

			rmp_left_min 	= std::stof(ramp->first_node("left")->first_attribute("rmp_left_min")->value());
			rmp_left_c   	= std::stof(ramp->first_node("left")->first_attribute("rmp_left_c")->value());
			rmp_left_max 	= std::stof(ramp->first_node("left")->first_attribute("rmp_left_max")->value());

			rmp_center_min 	= std::stof(ramp->first_node("center")->first_attribute("rmp_center_min")->value());
			rmp_center_c   	= std::stof(ramp->first_node("center")->first_attribute("rmp_center_c")->value());
			rmp_center_max 	= std::stof(ramp->first_node("center")->first_attribute("rmp_center_max")->value());

			rmp_right_min 	= std::stof(ramp->first_node("right")->first_attribute("rmp_right_min")->value());
			rmp_right_c   	= std::stof(ramp->first_node("right")->first_attribute("rmp_right_c")->value());
			rmp_right_max 	= std::stof(ramp->first_node("right")->first_attribute("rmp_right_max")->value());

			//Read Walls Data
			xml_node<> * walls = player->first_node("walls");

			wls_left_min 	= std::stof(walls->first_node("left")->first_attribute("wls_left_min")->value());
			wls_left_c   	= std::stof(walls->first_node("left")->first_attribute("wls_left_c")->value());
			wls_left_max 	= std::stof(walls->first_node("left")->first_attribute("wls_left_max")->value());

			wls_center_min 	= std::stof(walls->first_node("center")->first_attribute("wls_center_min")->value());
			wls_center_c   	= std::stof(walls->first_node("center")->first_attribute("wls_center_c")->value());
			wls_center_max 	= std::stof(walls->first_node("center")->first_attribute("wls_center_max")->value());

			wls_right_min 	= std::stof(walls->first_node("right")->first_attribute("wls_right_min")->value());
			wls_right_c   	= std::stof(walls->first_node("right")->first_attribute("wls_right_c")->value());
			wls_right_max 	= std::stof(walls->first_node("right")->first_attribute("wls_right_max")->value());

			//Read Ratio1 Data
			xml_node<> * ratio1 = player->first_node("ratio1");

			wp_left_r1		= std::stof(ratio1->first_node("waypoint")->first_attribute("wp_left_r1")->value());
			wp_right_r1   	= std::stof(ratio1->first_node("waypoint")->first_attribute("wp_right_r1")->value());
			box_left_r1		= std::stof(ratio1->first_node("box")->first_attribute("box_left_r1")->value());
			box_right_r1   	= std::stof(ratio1->first_node("box")->first_attribute("box_right_r1")->value());

			//Read Ratio2 Data
			xml_node<> * ratio2 = player->first_node("ratio2");

			wp_left_r2		= std::stof(ratio2->first_node("waypoint")->first_attribute("wp_left_r2")->value());
			wp_right_r2   	= std::stof(ratio2->first_node("waypoint")->first_attribute("wp_right_r2")->value());
			rmp_left_r2		= std::stof(ratio2->first_node("ramp")->first_attribute("rmp_left_r2")->value());
			rmp_right_r2   	= std::stof(ratio2->first_node("ramp")->first_attribute("rmp_right_r2")->value());

			//Read Ratio3 Data
			xml_node<> * ratio3 = player->first_node("ratio3");

			wp_left_r3		= std::stof(ratio3->first_node("waypoint")->first_attribute("wp_left_r3")->value());
			wp_right_r3   	= std::stof(ratio3->first_node("waypoint")->first_attribute("wp_right_r3")->value());
			rmp_left_r3		= std::stof(ratio3->first_node("ramp")->first_attribute("rmp_left_r3")->value());
			rmp_right_r3   	= std::stof(ratio3->first_node("ramp")->first_attribute("rmp_right_r3")->value());
			box_left_r3		= std::stof(ratio3->first_node("box")->first_attribute("box_left_r3")->value());
			box_right_r3   	= std::stof(ratio3->first_node("box")->first_attribute("box_right_r3")->value());

			//Read CentroidT1 Data
			xml_node<> * centroidT1 = player->first_node("centroidT1");

			ctd_left_r1			= std::stof(centroidT1->first_node("left")->first_attribute("ctd_left_r1")->value());
			ctd_center_r1   	= std::stof(centroidT1->first_node("center")->first_attribute("ctd_center_r1")->value());
			ctd_right_r1		= std::stof(centroidT1->first_node("right")->first_attribute("ctd_right_r1")->value());

			//Read CentroidT2 Data
			xml_node<> * centroidT2 = player->first_node("centroidT2");

			ctd_left_r2			= std::stof(centroidT2->first_node("left")->first_attribute("ctd_left_r2")->value());
			ctd_center_r2   	= std::stof(centroidT2->first_node("center")->first_attribute("ctd_center_r2")->value());
			ctd_right_r2		= std::stof(centroidT2->first_node("right")->first_attribute("ctd_right_r2")->value());

			//Read CentroidT3 Data
			xml_node<> * centroidT3 = player->first_node("centroidT3");

			ctd_left_r3			= std::stof(centroidT3->first_node("left")->first_attribute("ctd_left_r3")->value());
			ctd_center_r3   	= std::stof(centroidT3->first_node("center")->first_attribute("ctd_center_r3")->value());
			ctd_right_r3		= std::stof(centroidT3->first_node("right")->first_attribute("ctd_right_r3")->value());
		}
    }
}

void AIDrivingComponent::readSpeedValues()
{
    using namespace rapidxml;

    xml_document<> doc;
    xml_node<> * root_node;

    //Read the file and put it into a char array
    std::ifstream theFile("media/xml/SpeedAIValues.xml");
	std::string buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);

    // Find our root node
	root_node = doc.first_node("player");

    // Iterate over the gameObjects, reading them
    for (xml_node<> * player = root_node; player; player = player->next_sibling()) {
		
		if(std::stoi(player->first_attribute("name")->value()) == 
			getGameObject().getComponent<MoveComponent>().get()->getMovemententData().player){

			//Read where i am going Data
			xml_node<> * going = player->first_node("going");

			s_going_left_min 	= std::stof(going->first_node("left")->first_attribute("s_going_left_min")->value());
			s_going_left_c   	= std::stof(going->first_node("left")->first_attribute("s_going_left_c")->value());
			s_going_left_max 	= std::stof(going->first_node("left")->first_attribute("s_going_left_max")->value());

			s_going_center_min 	= std::stof(going->first_node("center")->first_attribute("s_going_center_min")->value());
			s_going_center_c   	= std::stof(going->first_node("center")->first_attribute("s_going_center_c")->value());
			s_going_center_max 	= std::stof(going->first_node("center")->first_attribute("s_going_center_max")->value());

			s_going_right_min 	= std::stof(going->first_node("right")->first_attribute("s_going_right_min")->value());
			s_going_right_c   	= std::stof(going->first_node("right")->first_attribute("s_going_right_c")->value());
			s_going_right_max 	= std::stof(going->first_node("right")->first_attribute("s_going_right_max")->value());

			//Read object collision Data
			xml_node<> * collision = player->first_node("collision");

			s_obs_left_min 	= std::stof(collision->first_node("left")->first_attribute("s_obs_left_min")->value());
			s_obs_left_c   	= std::stof(collision->first_node("left")->first_attribute("s_obs_left_c")->value());
			s_obs_left_max 	= std::stof(collision->first_node("left")->first_attribute("s_obs_left_max")->value());

			s_obs_center_min 	= std::stof(collision->first_node("center")->first_attribute("s_obs_center_min")->value());
			s_obs_center_c   	= std::stof(collision->first_node("center")->first_attribute("s_obs_center_c")->value());
			s_obs_center_max 	= std::stof(collision->first_node("center")->first_attribute("s_obs_center_max")->value());

			s_obs_right_min 	= std::stof(collision->first_node("right")->first_attribute("s_obs_right_min")->value());
			s_obs_right_c   	= std::stof(collision->first_node("right")->first_attribute("s_obs_right_c")->value());
			s_obs_right_max 	= std::stof(collision->first_node("right")->first_attribute("s_obs_right_max")->value());

			//Read object distance Data
			xml_node<> * distance = player->first_node("distance");

			s_obs_closeRange_min 	= std::stof(distance->first_node("left")->first_attribute("s_obs_closeRange_min")->value());
			s_obs_closeRange_c   	= std::stof(distance->first_node("left")->first_attribute("s_obs_closeRange_c")->value());
			s_obs_closeRange_max 	= std::stof(distance->first_node("left")->first_attribute("s_obs_closeRange_max")->value());

			s_obs_mediumRange_min 	= std::stof(distance->first_node("center")->first_attribute("s_obs_mediumRange_min")->value());
			s_obs_mediumRange_c   	= std::stof(distance->first_node("center")->first_attribute("s_obs_mediumRange_c")->value());
			s_obs_mediumRange_max 	= std::stof(distance->first_node("center")->first_attribute("s_obs_mediumRange_max")->value());

			s_obs_farRange_min 	= std::stof(distance->first_node("right")->first_attribute("s_obs_farRange_min")->value());
			s_obs_farRange_c   	= std::stof(distance->first_node("right")->first_attribute("s_obs_farRange_c")->value());
			s_obs_farRange_max 	= std::stof(distance->first_node("right")->first_attribute("s_obs_farRange_max")->value());

			//Read Speed CentroidT1 Data
			xml_node<> * centroidT1 = player->first_node("centroidT1");

			s_ctd_left_r1		= std::stof(centroidT1->first_node("left")->first_attribute("s_ctd_left_r1")->value());
			s_ctd_center_r1   	= std::stof(centroidT1->first_node("center")->first_attribute("s_ctd_center_r1")->value());
			s_ctd_right_r1		= std::stof(centroidT1->first_node("right")->first_attribute("s_ctd_right_r1")->value());

			//Read Speed CentroidT2 Data
			xml_node<> * centroidT2 = player->first_node("centroidT2");

			s_ctd_left_r2		= std::stof(centroidT2->first_node("left")->first_attribute("s_ctd_left_r2")->value());
			s_ctd_center_r2   	= std::stof(centroidT2->first_node("center")->first_attribute("s_ctd_center_r2")->value());
			s_ctd_right_r2		= std::stof(centroidT2->first_node("right")->first_attribute("s_ctd_right_r2")->value());

			//Read Speed CentroidT3 Data
			xml_node<> * centroidT3 = player->first_node("centroidT3");

			s_ctd_left_r3		= std::stof(centroidT3->first_node("left")->first_attribute("s_ctd_left_r3")->value());
			s_ctd_center_r3   	= std::stof(centroidT3->first_node("center")->first_attribute("s_ctd_center_r3")->value());
			s_ctd_right_r3		= std::stof(centroidT3->first_node("right")->first_attribute("s_ctd_right_r3")->value());

		}
    }
}