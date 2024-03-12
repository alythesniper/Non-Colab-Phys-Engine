#pragma once
#include "flatBody.cpp"
#include "collisions.hpp"
#include <vector>

class FlatWorld
{
public:
	std::vector<flatBody*> bodies;
	FlatVector gravity;
	
	static const float minBodySize; //m
	static const float maxBodySize;

	static const float minDensity; // g/cm^3
	static const float maxDensity;
		
	FlatWorld()
	{
		gravity = FlatVector(0, 9.81);
		this->bodies = std::vector<flatBody*>();
	}

	void addBody(flatBody* body)
	{
		this->bodies.push_back(body);
	}

	void removeBody(flatBody* body)
	{
		for (int i = 0; i < this->bodies.size(); i++)
		{
			if (this->bodies[i] == body)
			{
				this->bodies.erase(this->bodies.begin() + i);
				break;
			}
		}
	}

	flatBody* getBody(int index)
	{
		if (index < this->bodies.size())
		{
			return this->bodies[index];
		}
		else
		{
			return NULL;
		}
	}

	int getbodiesCount()
	{
		return this->bodies.size();
	}

	void resolveCollision(flatBody* body1, flatBody* body2, FlatVector normal, float depth)
	{
		FlatVector relativeVelocity = body2->getLinearVelocity() - body1->getLinearVelocity();
		float e = fmin(body1->restitution, body2->restitution);
		float impulse = -(1 + e) * relativeVelocity.dotProd(relativeVelocity, normal);
		impulse /= (1 / body1->mass) + (1 / body2->mass);
		
		body1->setLinearVelocity(body1->getLinearVelocity() - impulse / body1->mass * normal);
		body2->setLinearVelocity(body2->getLinearVelocity() + impulse / body2->mass * normal);

	}
	

	bool collide(flatBody* body1, flatBody* body2, FlatVector&normal, float& depth)
	{
		normal = FlatVector(0, 0);
		depth = 0;

		shapeType type1 = body1->bodyShapeType;
		shapeType type2 = body2->bodyShapeType;
		if (type1 == shapeType::box)
		{
			if (type2 == shapeType::box)
			{
				return collisions::intersectPolygons(body1->getTransformedVertices(), body2->getTransformedVertices(),
				 normal, depth);
			}
			else if (type2 == shapeType::circle)
			{
				bool result = collisions::intersectCirclePolygon(body2->getPosition(), body2->radius, body1->getTransformedVertices(), normal, depth);

				normal = -normal;
				return result;
			}
			
		}
		else if (type1 == shapeType::circle)
		{
			if (type2 == shapeType::box)
			{
				return collisions::intersectCirclePolygon(body1->getPosition(), body1->radius, body2->getTransformedVertices(), normal, depth);
			}
			else if (type2 == shapeType::circle)
			{
				return collisions::intersectCircles(body1->getPosition(), body1->radius, body2->getPosition(), body2->radius, &normal, &depth);
			}

		}
		return false;
	}

	void Step(float time)
	{
		//movement step
		for(int i = 0; i < bodies.size(); i++)
		{
			bodies[i]->step(time);
		}
		
		//collision step
		FlatVector normal = { 0,0 };
		float depth = 0;
		for (int i = 0; i < bodies.size(); i++)
		{
			for (int j = i + 1; j < bodies.size(); j++)
			{
				if(collide(bodies[i], bodies[j], normal, depth))
				{
					//bodies[i]->move(-normal * depth);
					//bodies[j]->move(normal * depth);

					this->resolveCollision(bodies[i], bodies[j], normal, depth);
				}
			}
			
		}
		
	}
	
};


//initialising static variables
const float FlatWorld::minBodySize = 0.01 * 0.01;
const float FlatWorld::maxBodySize = 10000 * 10000;
const float FlatWorld::minDensity = 0.2; 
const float FlatWorld::maxDensity = 21.4;
