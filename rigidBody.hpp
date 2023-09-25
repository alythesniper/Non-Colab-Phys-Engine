#pragma once
#define _USE_MATH_DEFINES
#include "flatVector.hpp"
#include <cmath>
#include "flatWorld.hpp"

enum class shapeType
{
	circle = 0,
	box
};

class rigidBody
{
private:
	FlatVector position{ 0, 0 };
	FlatVector linearVelocity{ 0, 0 };
	float rotation = 0;
	float rotationalVelocity = 0;

public:
	rigidBody(FlatVector positioni, float densityi, float massi, float restitutioni,
		float areai, bool isStatici, float radiusi, float widthi, float heighti,
		shapeType ShapeTypei) :
		position(positioni), density(densityi), mass(massi),
		restitution(restitutioni), area(areai), isStatic(isStatici), radius(radiusi),
		width(widthi), height(heighti), bodyShapeType(ShapeTypei)
	{};
	const float density = 0;
	const float mass = 0;
	const float restitution = 0;
	const float area = 0;

	//will not be affected by physics
	const bool isStatic = 0;

	const float radius = 0;
	const float width = 0;
	const float height = 0;

	shapeType bodyShapeType;

	bool createCircleBody(float radius, FlatVector position, float density, bool isStatic,
		float restitution)
	{
		float area = radius * radius * M_PI;

		if (area < FlatWorld::minBodySize) 
		{
			return false;
			//do error logging
		}

		if (area > FlatWorld::maxBodySize)
		{
			return false;
			//do error logging
		}

		if (density < FlatWorld::minDensity)
		{
			return 0;
			//do error logging
		}



	}
};
