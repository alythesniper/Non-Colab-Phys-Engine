#pragma once
#include <typeinfo>
#include <exception>
#include <string>

const struct FlatVector {
	
	float x, y;

	FlatVector(float X, float Y);
	
	//overloading basic vector operations
	FlatVector operator + (const FlatVector& b);
	FlatVector operator - (const FlatVector& b);
	FlatVector operator - ();
	FlatVector operator * (const float& b);
	FlatVector operator / (const float& b);
	bool operator == (FlatVector& b);
	float length(FlatVector vec1);
	float distance(FlatVector vec1, FlatVector vec2);
	FlatVector* normalise(FlatVector vec1);
	float dotProd(FlatVector vec1, FlatVector vec2);
	float crossProd(FlatVector vec1, FlatVector vec2);
	
};
