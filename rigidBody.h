#pragma once
#include "flatVector.h"

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
	rigidBody(FlatVector position, float density, float mass, float restitution, float area,
		bool isStatic, float radius, float width, float height, shapeType inputShapeType);
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
};

