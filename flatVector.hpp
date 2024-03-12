#pragma once
#include <typeinfo>
#include <exception>
#include <string>

#include "raylib.h"
#include "flatTransform.hpp"

const struct FlatVector {
public:
	float x, y;

	//default contstructor
	FlatVector(){x = 0; y = 0;}
	//overload constructor
	FlatVector(float X, float Y)
	{
		x = X;
		y = Y;
	}
	
	//to Vector2
	Vector2 toVector2()
	{
		Vector2 temp = { 0, 0 };
		temp.x = this->x;
		temp.y = this->y;
		return temp;
	}
	
	//overloading basic vector operations
	FlatVector operator + (const FlatVector& b)
	{
		return FlatVector(x + b.x, y + b.y);
	}
	FlatVector operator - (const FlatVector& b)
	{
		return FlatVector(x - b.x, y - b.y);
	}
	FlatVector operator += (const FlatVector& b)
	{
	this->x += b.x;
		this->y += b.y;
		return FlatVector(x + b.x, y + b.y);
	}
	FlatVector operator -= (const FlatVector& b)
	{
		this->x += b.x;
		this->y += b.y;
		return FlatVector(x - b.x, y - b.y);
	}
	FlatVector operator - ()
	{
		return FlatVector(-x, -y);
	}
	FlatVector operator * (const float& b)
	{
		return FlatVector(x * b, y * b);
	}
	FlatVector operator * (const FlatVector& b)
	{
		return FlatVector(x * b.x, y * b.y);
	}
	friend FlatVector operator * (const float& a, const FlatVector& v)
	{
		return FlatVector(a * v.x, a * v.y);
	}
	FlatVector operator / (const float& b)
	{
		return FlatVector(x / b, y / b);
	}
	FlatVector& operator/=(const float& scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}
	bool operator == (FlatVector& b)
	{
		//checks if operand is FlatVector Type
		if (typeid(b) == typeid(FlatVector))
		{
			if (x == b.x && y == b.y) { return true; }
			else { return false; }
		}
		//otherwise return 0
		else
		{
			return 0;
			//add error to future logging system
		}
	}
	FlatVector* transform(FlatVector v, flatTransform transform)
	{
		//vector rotation fomula plus translation
		return new FlatVector(
		transform.Cos * v.x - transform.Sin * v.y + transform.positionx,
		transform.Sin * v.x + transform.Cos * v.y + transform.positiony);
	}


	float length(FlatVector vec1)
	{
		return sqrt(pow(vec1.x, 2) + pow(vec1.y, 2));
	}
	float distance(FlatVector vec1, FlatVector vec2)
	{
		return sqrt(pow(sqrt(vec1.x * vec1.x + vec1.y * vec1.y - sqrt(vec1.x * vec1.x + vec1.y * vec1.y)), 2));
	}
	FlatVector* normalise(FlatVector vec1)
	{
		float len = length(vec1);
		return new FlatVector(vec1.x / len, vec1.y / len);
	}
	float dotProd(FlatVector vec1, FlatVector vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}
	float crossProd(FlatVector vec1, FlatVector vec2)
	{
		return vec1.x * vec2.y - vec1.y * vec2.x;
	}
	
	//convert to string
	std::string vecToString(FlatVector& a)
	{
		return "x: " + std::to_string(a.x) + "y: " + std::to_string(a.y);
	}
};


namespace vectorFunc
{

}
