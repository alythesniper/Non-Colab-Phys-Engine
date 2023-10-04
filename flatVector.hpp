#pragma once
#include <typeinfo>
#include <exception>
#include <string>

const struct FlatVector {
	
	float x, y;

	FlatVector(float X, float Y)
	{
		x = X;
		y = Y;
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
	FlatVector operator / (const float& b)
	{
		return FlatVector(x / b, y / b);
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


