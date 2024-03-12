#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include <vector>
#include <fstream>

#include "flatVector.hpp"


const float minBodySize = 0.01 * 0.01;
const float maxBodySize = 10000 * 10000;
const float minDensity = 0.2;
const float maxDensity = 21.4;

enum class shapeType
{
	circle = 0,
	box
};

void logError(const std::string& message) 
{
	std::ofstream logFile;
	logFile.open("error_log.txt", std::ios_base::app); // Append to the existing file
	logFile << message << std::endl;
	logFile.close();
}

class flatBody
{
private:
	FlatVector force;
	FlatVector position;
	FlatVector linearVelocity;
	float rotation;
	float rotationalVelocity;
	
	std::vector<FlatVector> vertices;
	std::vector<FlatVector> transformedVerticies;
	bool transformedVerticiesNeedUpdate;
	
public:
	//getters for all private variables
	FlatVector getPosition() { return position; }
	FlatVector getLinearVelocity() { return linearVelocity; }
	float getRotation() { return rotation; }
	float getRotationalVelocity() { return rotationalVelocity; }
	bool getTransformedVerticiesNeedUpdate() { return transformedVerticiesNeedUpdate; }
	
	//creating required variables	
	std::vector<int> boxTriangles;
	float density = 0;
	float mass = 0;
	float restitution = 0;
	float area = 0;
	
	//will not be affected by physics
	bool isStatic = 0;
	float radius = 0;
	float width = 0;
	float height = 0;
	shapeType bodyShapeType;
	
	std::vector<FlatVector> createBoxVerticies(float width, float height)
	{
		//x and y coordinates of sides
		float left = -width / 2;
		float right = left + width;
		float bottom = -height / 2;
		float top = bottom + height;

		//create vertices
		std::vector<FlatVector> vertices;
		vertices.push_back(FlatVector(left, top));
		vertices.push_back(FlatVector(right, top));
		vertices.push_back(FlatVector(right, bottom));
		vertices.push_back(FlatVector(left, bottom));
		
		return vertices;
	}

	//indicies for 2 trianges that make up box(clockwise)
	std::vector<int> triangulateBox()
	{
		std::vector<int> triangles;
		triangles.push_back(0);
		triangles.push_back(1);
		triangles.push_back(2);
		triangles.push_back(0);
		triangles.push_back(2);
		triangles.push_back(3);
		return triangles;
	}
	std::vector<FlatVector> getTransformedVertices()
	{
		if (transformedVerticiesNeedUpdate)
		{
			flatTransform transform(this->position.x, this->position.y, this->rotation);

			for (int i = 0; i < vertices.size(); i++)
			{
				FlatVector v = vertices[i];
				transformedVerticies[i] = *v.transform(v, transform);
			}
		}
		transformedVerticiesNeedUpdate = false;
		return transformedVerticies;
	 }
	flatBody(FlatVector positioni, float densityi, float massi, float restitutioni,
		float areai, bool isStatici, float radiusi, float widthi, float heighti,
		shapeType ShapeTypei)
		: position(positioni), density(densityi), mass(massi), restitution(restitutioni),
		area(areai), isStatic(isStatici), radius(radiusi), width(widthi), height(heighti),
		bodyShapeType(ShapeTypei), transformedVerticiesNeedUpdate(true), rotation(0),
		rotationalVelocity(0), force(0, 0)
	{
		transformedVerticiesNeedUpdate = true;
		// If shapetype is box, then calculate vertices
		if (bodyShapeType == shapeType::box) {
			vertices = createBoxVerticies(this->width, this->height);
			
			//also initialise transformed vertices
			transformedVerticies.resize((vertices.size()));
			boxTriangles = triangulateBox(); // Initialize boxTriangles here
		}
	}


	void step(float time)
	{
		this->linearVelocity += this->force * time;
		
		this->position += this->linearVelocity * time;
		this->rotation += this->rotationalVelocity * time;

		this->force = FlatVector(0, 0);
		this->transformedVerticiesNeedUpdate = true;
	}
	void move(FlatVector ammounti)
	{
		this->position += ammounti;
		this->transformedVerticiesNeedUpdate = true;
	}
	void moveTo(FlatVector positioni)
	{
		this->position = positioni;
		this->transformedVerticiesNeedUpdate = true;
	}
	void rotate(float ammount)
	{
		this->rotation += ammount;
		this->transformedVerticiesNeedUpdate = true;
	}
	void addForce(FlatVector forcei)
	{
		this->force += forcei;
	}
	void setLinearVelocity(FlatVector velocity)
	{
		this->linearVelocity = velocity;
	}
};

flatBody* createCircleBody(float radiusi, FlatVector positioni, float densityi, bool isStatici,
	float restitutioni)
{
	float area = radiusi * radiusi * M_PI;
	float mass = area * densityi;

	if (area < minBodySize)
	{
		logError("Error: Body size is less than minimum allowed size.");
		return nullptr;
	}

	if (area > maxBodySize)
	{
		logError("Error: Body size is greater than maximum allowed size.");
		return nullptr;
	}

	if (densityi < minDensity)
	{
		logError("Error: Density is less than minimum allowed density.");
		return nullptr;
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

	if (area < minBodySize)
	{
		logError("Error: Body size is less than minimum allowed size.");
		return nullptr;
	}

	if (area > maxBodySize)
	{
		logError("Error: Body size is greater than maximum allowed size.");
		return nullptr;
	}

	if (densityi < minDensity)
	{
		logError("Error: Density is less than minimum allowed density.");
		return nullptr;
	}

	flatBody* body = new flatBody(positioni, densityi, mass, restitutioni, area, isStatici,
		0, widthi, heighti, shapeType::box);
	return body;
}
