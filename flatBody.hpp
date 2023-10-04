#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include "flatVector.hpp"
#include "flatWorld.hpp"

enum class shapeType
{
	circle = 0,
	box
};

class flatBody
{
/*private:
	FlatVector position{ 0, 0 };
	FlatVector linearVelocity{ 0, 0 };
	float rotation = 0;
	float rotationalVelocity = 0;*/

public:
	flatBody(FlatVector positioni, float densityi, float massi, float restitutioni,
		float areai, bool isStatici, float radiusi, float widthi, float heighti,
		shapeType ShapeTypei) :
		position(positioni), density(densityi), mass(massi), restitution(restitutioni),
		area(areai), isStatic(isStatici), radius(radiusi), width(widthi), height(heighti),
		bodyShapeType(ShapeTypei)
	{};
	const float density = 0;
	const float mass = 0;
	const float restitution = 0;
	const float area = 0;
	
	//can't be asked to make setters
	FlatVector position{ 0, 0 };
	FlatVector linearVelocity{ 0, 0 };
	float rotation = 0;
	float rotationalVelocity = 0;

	//will not be affected by physics
	const bool isStatic = 0;
	const float radius = 0;
	const float width = 0;
	const float height = 0;
	shapeType bodyShapeType;

	void move(FlatVector ammounti)
	{
		position += ammounti;
	}
	void moveTo(FlatVector positioni)
	{
		position = positioni;
	}
};

flatBody* createCircleBody(float radiusi, FlatVector positioni, float densityi, bool isStatici,
	float restitutioni)
{
	float area = radiusi * radiusi * M_PI;
	float mass = area * densityi;

	if (area < FlatWorld::minBodySize)
	{
		return nullptr;
		//do error logging
	}

	if (area > FlatWorld::maxBodySize)
	{
		return nullptr;
		//do error logging
	}

	if (densityi < FlatWorld::minDensity)
	{
		return nullptr;
		//do error logging
	}

	flatBody* body = new flatBody(positioni, densityi, mass, restitutioni, area, isStatici,
		radiusi, 0, 0, shapeType::circle);

	return body;
}

flatBody* createBoxBody(float widthi, float heighti, FlatVector positioni, float densityi,
	bool isStatici, float restitutioni)
{
	float area = widthi * heighti;
	float mass = area * densityi;

	if (area < FlatWorld::minBodySize)
	{
		return nullptr;
		//do error logging
	}

	if (area > FlatWorld::maxBodySize)
	{
		return nullptr;
		//do error logging
	}

	if (densityi < FlatWorld::minDensity)
	{
		return nullptr;
		//do error logging
	}

	flatBody* body = new flatBody(positioni, densityi, mass, restitutioni, area, isStatici,
		0, widthi, heighti, shapeType::box);
	return body;
}
